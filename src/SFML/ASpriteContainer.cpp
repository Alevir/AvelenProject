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

#include "ASpriteContainer.h"

#include <iostream>
#include <fstream>
#include <libconfig.h++>



ASpriteContainer::ASpriteContainer(ATextureContainer *ipTC) {
  TC = ipTC;
}


ASprite* ASpriteContainer::AddSprite(const ASpriteData* ipData)    throw (exSpriteAddition) {

  if(_spriteContainer.count(ipData->name) != 0) {
    throw exSpriteAddition(std::string("Sprite with name \"") + ipData->name + "\" already exists");
  }
  ASprite* spr = new ASprite(ipData, TC);
  _spriteContainer.insert( std::pair<std::string, ASprite* > ( ipData->name, spr));
  return spr;
}


ASprite* ASpriteContainer::GetSprite(std::string iSpriteName) {
  std::map<std::string, ASprite*>::iterator it;
  if((it = _spriteContainer.find(iSpriteName)) == _spriteContainer.end()) {
    std::map<std::string, ASpriteData>::iterator data_it;
    if((data_it = _dataContainer.find(iSpriteName)) == _dataContainer.end()) {
      throw std::logic_error(std::string("Sprite ") + iSpriteName + " not found in ASpriteContainer");
    } else {
      ASprite* spr = new ASprite(&(data_it->second), TC);
      _spriteContainer.insert(std::pair<std::string, ASprite*>(data_it->second.name, spr));
      return spr;
    }
  } else {
    return it->second;
  }
}


void ASpriteContainer::AddContent(const std::string& path) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile(path.c_str());
  std::string p = path;
  while(p.back() != '/') p.pop_back();
  Setting& sprList = cfg.lookup("sprites");
  for(int i = 0; i < sprList.getLength(); i++) {
    Setting& s = sprList[i];
    ASpriteData newSprData;
    newSprData.name = (const char*)s["name"];
    newSprData.imagePath = p + (const char*)s["path"];
    s.lookupValue("left", newSprData.left);
    s.lookupValue("top", newSprData.top);
    s.lookupValue("gHeight", newSprData.gHeight);
    s.lookupValue("gWidth", newSprData.gWidth);
    s.lookupValue("xSprSize", newSprData.xSprSize);
    s.lookupValue("ySprSize", newSprData.ySprSize);
    _dataContainer.insert(std::pair<std::string, ASpriteData>(newSprData.name, newSprData));
    }
}

ASpriteContainer::~ASpriteContainer() {
  for(auto p : _spriteContainer) {
    delete p.second;
    }
}

void ASpriteContainer::GetSpritesNames(std::set<std::string> &s) {
  for(auto p : _dataContainer) {
    s.insert(p.second.name);
  }
}
