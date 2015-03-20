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

    Author of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef GLOBAL_H
#define GLOBAL_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
class CharacterRace;
class ATextureContainer;
class ASpriteContainer;
class AMaterialContainer;
#include "AObjectTemplatesContainer.h"
#include "ACharacterData.hpp"
#include <string>
#include <cstring>
#include "AAnimationScheme.hpp"
#include "AGarbageCollector.hpp"
class ATranslationReader;
class APotionRecipe;
class AExtractionRecipe;

template<class T>
void LoadPaths(T& cont, const std::string& pathsFile) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile((pathsFile).c_str());
  std::string path = pathsFile;
  while(path.back() != '/') {
    path.pop_back();
  }
  Setting& list = cfg.getRoot()["paths"];
  for(int i = 0; i < list.getLength(); ++i) {
    cont->AddContent(path + (const char*)list[i]);
  }
}


//std::string& operator+ (const char* str0, const std::string& str1);
enum class Translations : size_t{
  ObjectLabels = 0,
  Common
};




class Game {
  void loadLogicMaterials(const std::string& iFilePath);
  AAnimationContainer animCont;
public:
  static const std::string ResourcesPath;
  static sf::RenderWindow* Window;
  static std::map<const std::string, const CharacterRace> CharacterRaces;
  static ATextureContainer* TextureContainer;
  static ASpriteContainer* ObjectSpriteContainer;
  static AObjectTemplatesContainer<APhysicObjectData>* ObjectTemplatesContainer;
  static AObjectTemplatesContainer<ACharacterData>* CharacterTemplatesContainer;
  static std::string Language;
  static int LightQuality;
  static std::string GuiConfFileName;
  static std::string DefaultFont;
  static std::string TranslationsPath;
  static std::map<std::string, ALogicMaterial> LoadedMaterials;
  static AAnimationContainer* animContainer;
  static AObjectTemplatesContainer<AAnimationScheme>* AnimationSchemes;
  static AObjectTemplatesContainer<APotionRecipe>* PotionRecipes;
  static AObjectTemplatesContainer<AExtractionRecipe>* ExtractionRecipes;
  static ATranslationReader* Translations;
  static sf::Vector2u LocLoadNum; // defines number of loaded locations near the current location


  Game();
  ~Game();
};



#endif // GLOBAL_H
