/*
    This file is part of Avelen Project.
    Copyright (c) 2014  Evdokimov Mikhail

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

*/
#include "Global.h"
#include <libconfig.h++>
#include <ADebugOutput.h>
#include "icon.h"
#include "ALocationSFML.h"
#include "Character/Race.h"
#include "ASpriteContainer.h"
#include "APhysicObjectData.h"
#include "AAlchemySystem.hpp"
#include "ATranslationReader.h"
#include "Common.h"

using namespace std;

std::string Game::GuiConfFileName;
std::string Game::DefaultFont;
std::string Game::TranslationsPath;

sf::RenderWindow* Game::Window;
std::map<const std::string, const CharacterRace> Game::CharacterRaces;
ATextureContainer* Game::TextureContainer;
ASpriteContainer* Game::ObjectSpriteContainer;
AObjectTemplatesContainer<APhysicObjectData>*  Game::ObjectTemplatesContainer;
AObjectTemplatesContainer<ACharacterData>*  Game::CharacterTemplatesContainer;
std::map<std::string, ALogicMaterial> Game::LoadedMaterials;

AAnimationContainer* Game::animContainer;
AObjectTemplatesContainer<AAnimationScheme>* Game::AnimationSchemes;
AObjectTemplatesContainer<APotionRecipe>* Game::PotionRecipes;
AObjectTemplatesContainer<AExtractionRecipe>* Game::ExtractionRecipes;

ATranslationReader* Game::Translations;
sf::Vector2u Game::LocLoadNum;
int Game::LightQuality;

const std::string Game::ResourcesPath = "../resources/";


std::string Game::Language;


void Game::loadLogicMaterials(const string &iFilePath) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile(iFilePath.c_str());
  Setting& ms = cfg.lookup("materials");
  for(int i = 0; i < ms.getLength(); i++) {
    Setting& m = ms[i];
    LoadedMaterials[m["name"]] = ALogicMaterial(m["name"], m["durability"], m["resilience"]);
  }

}

void LoadRaces(const char* filename) {
  libconfig::Config cfg;
  try {
    cfg.readFile(filename);
  } catch (libconfig::ParseException& ex) {
    dout << "file: " << ex.getFile() << "\nline: " << ex.getLine()
      << "\nerror: " << ex.getError() << endl;
    return;
  }
  libconfig::Setting& s = cfg.lookup("races");
  for(int i = 0; i < s.getLength(); i++) {
    Game::CharacterRaces.insert(std::pair<const std::string, const CharacterRace>((const char*)s[i]["name"], CharacterRace(s[i])));
  }
}

Game::Game() {

  libconfig::Config cfg;
  try {
    cfg.readFile("../resources/mainSettings.cfg");
  } catch (libconfig::ParseException& ex) {
    dout << "file: " << ex.getFile() << "\nline: " << ex.getLine()
      << "\nerror: " << ex.getError() << endl;
    return;
  }
  libconfig::Setting& res = cfg.lookup("window.resolution");
  libconfig::Setting& locs = cfg.lookup("world");
  LocLoadNum.x = locs["x"];
  LocLoadNum.y = locs["y"];

  sf::VideoMode vm;
  vm.width = res["width"];
  vm.height = res["height"];
  GuiConfFileName = (const char*)cfg.lookup("guiConfFileName");
  sf::ContextSettings cs;


  string vsync;
  cfg.lookupValue("window.antialiasingLevel", cs.antialiasingLevel);
  cfg.lookupValue("window.VSync", vsync);
  LightQuality = cfg.lookup("window.LightQuality");

  std::cout << cs.antialiasingLevel << '\n';
  string name;
  res.lookupValue("name", name);
  Window = new sf::RenderWindow(vm, name, 0/* sf::Style::None*/, cs);
  Window->setIcon(gimp_image.width, gimp_image.height, gimp_image.pixel_data);
  Window->setActive();
  Window->setVerticalSyncEnabled(vsync == "yes");


  libconfig::Setting& font = cfg.lookup("defaultFont");
  //gui::DefaultFont.loadFromFile((const char*)font["path"]);
  DefaultFont = (const char*)font["path"];
  TranslationsPath = (const char*)cfg.lookup("translationsPath");
  Language = (const char*)cfg.lookup("language");
  loadLogicMaterials("../resources/logicMaterials.cfg");
  TextureContainer = new ATextureContainer();
  ObjectSpriteContainer = new ASpriteContainer(TextureContainer);
  LoadPaths(ObjectSpriteContainer, "../resources/spritePaths.cfg");
  ALocationSFML::LoadGroundCells("../resources/groundCells.cfg");
  LoadRaces("../resources/races.cfg");
  ObjectTemplatesContainer = new AObjectTemplatesContainer<APhysicObjectData>();
  LoadPaths(ObjectTemplatesContainer, "../resources/objectPaths.cfg");
  CharacterTemplatesContainer = new AObjectTemplatesContainer<ACharacterData>();
  CharacterTemplatesContainer->AddContent("../resources/characters.cfg");


  animContainer = &animCont;
  animCont.LoadAnimations("../resources/animations");

  AnimationSchemes = new AObjectTemplatesContainer<AAnimationScheme>();
  AnimationSchemes->AddContent("../resources/animations/animationSchemes.cfg");

  PotionRecipes = new AObjectTemplatesContainer<APotionRecipe>("recipes");
  PotionRecipes->AddContent("../resources/alchemy/recipes.cfg");

  ExtractionRecipes = new AObjectTemplatesContainer<AExtractionRecipe>("sources");
  ExtractionRecipes->AddContent("../resources/alchemy/extrations.cfg");

  Translations = new ATranslationReader(TranslationsPath + Language + "/");



  std::cout << (const char*) glGetString(GL_SHADING_LANGUAGE_VERSION_ARB) << '\n';
}


Game::~Game() {

  ALocationSFML::Finalize();
  //wObjectTemplatesContainer->Save("../resources/objects.cfg");

  delete Window;
  delete ObjectSpriteContainer;
  delete TextureContainer;
  delete ObjectTemplatesContainer;
  delete CharacterTemplatesContainer;
  delete AnimationSchemes;
  delete PotionRecipes;
  delete ExtractionRecipes;
  delete Translations;

  //ClearDirectory("../workspace/");

}



string& operator +(const char* str0, string& str1){
  str1.insert(0, str0);
  return str1;
}
