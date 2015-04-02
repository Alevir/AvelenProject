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
//#include "ACharacter.h"
class ALocationSFML;
class ASprite;
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
class ATransform;



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
  APhysicObjectBase* mObj = 0;
public:
  void Step();
  void BindToObj(APhysicObjectBase* obj);
  void CheckAndUnbind(APhysicObjectBase* obj);
  void Unbind();
  void Move(const AVector2& shift);
  void SetTransform(const ATransform& pos);
  AVector2 GetPosition();
};

class AObjectDrawerSFML;

class AWorldSFML : public AWorldBase {
  friend class ALocationSFML;
  friend class APhysicObjectSFML;
  friend class AObjectDrawerSFML;
  std::multimap<double, APhysicObjectBase*> drawContainer;
  virtual void _graphicStep(double dt);
  APhysicObjectBase* CreateObject(const APhysicObjectData& data, ALocationBase* loc, const ObjectInitArgs& args);
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


  struct drawData {
    drawData(ASprite* spr, double x, double y, double a) : Spr(spr), X(x), Y(y), A(a) {}
    ASprite* Spr;
    double X;
    double Y;
    double A;
  };

  std::list<drawData> mPreparedSprites;
public:
  void SetFocusedObject(APhysicObjectBase* obj);
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
