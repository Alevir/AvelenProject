/*
    This file is part of Avelen Project.

    Avelen Project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Avelen Project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Avelen Project.  If not, see <http://www.gnu.org/licenses/>.

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Authors of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef AWORLDSFML_H
#define AWORLDSFML_H
class GPlayerInterface;

#include "AWorldBase.h"

#include <map>
#include "ACharacterSFML.h"
class ACharacterSFML;
class ALocationSFML;
class ASprite;
#include <SFML/Audio.hpp>



class ALightSourceSFML : public ALightSourceBase {
  sf::RenderTexture* mShadow = 0;
  AVector2 mViewPos;

  static sf::Sprite mLight;
  static sf::Texture mLTex;
public:
  static void Init();
  ALightSourceSFML(double intensity, const AVector2& position, const sf::Color& color);
  void SetShadowTexture(sf::RenderTexture* shadow, const AVector2& pos);
  virtual void Illumine(double dt);
};

class ACamera {
  APhysicObjectSFML* _obj = 0;
public:
  void Step();
  void BindToObj(APhysicObjectSFML* obj);
  void CheckAndUnbind(APhysicObjectSFML* obj);
  void Unbind();
  void Move(const AVector2& shift);
  void SetPosition(const AVector2& pos);
  AVector2 GetPosition();
};


class AWorldSFML : public AWorldBase {
  friend class ALocationSFML;
  std::multimap<double, APhysicObjectSFML*> drawContainer;
  virtual void _graphicStep(double dt);
  APhysicObjectSFML* CreateObject(std::string iTemplateName, ALocationBase* loc, const ObjectInitArgs& args);
  ACharacterSFML* CreateCharacter(const ACharacterData* data, ALocationBase *loc, const ObjectInitArgs& args);
  virtual void RemoveObject(APhysicObjectBase* obj);
  sf::Listener listener;
  ALocationBase* newLocation(libconfig::Config& locationDesc, const ContextLocArgs& la);

  bool mGrPause = false;

  sf::RenderTexture shadowTex;
  sf::RenderStates shadowRS;
  sf::Color dayTime;
  sf::Sprite rs;
  sf::Texture lightTex;
  sf::Sprite light;
public:
  static void ParseColor(const char* str, sf::Color& color);
  void OneStepPause();

  sf::Sprite focusedShape;
  bool focused = false;

  GPlayerInterface* Interface = 0;
  ACamera Camera;
  AWorldSFML();
  AWorldSFML(const std::string& startLocationName);
  ~AWorldSFML();

  ASprite* GetSprite(std::string spriteName);
  static AVector2 ConvertMouseCurrentPos();
  static AVector2 ConvertMousePos(AVector2 mpos);
  void ResetCurrentLocation(libconfig::Config &cfg);

  virtual void SetVisible(APhysicObjectBase* obj);
  virtual void SetInvisible(APhysicObjectBase* obj);
  void AddMessage(const std::wstring &m);

};

#endif // AWORLDSFML_H
