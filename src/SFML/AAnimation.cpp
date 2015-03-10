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

#include "AAnimation.hpp"
#include <libconfig.h++>
#include <cstring>
#include <sstream>
#include <iostream>


const AAnimation& AAnimationContainer::GetAnimation(const std::string& name) {
  auto it = _loaded.find(name);
  if(it == _loaded.end()) {
    std::string m;
    m = m + "Animation \"" + name + "\" not found";
    throw std::logic_error(m);
  }
  return it->second;
}

void AAnimationContainer::LoadAnimations(const char* dirPath) {
  using namespace libconfig;
  Config cfg;
  std::string path("");
  path = path + dirPath + "/animations.cfg";
  cfg.readFile(path.c_str());
  Setting& anList = cfg.getRoot()["animations"];
  for(int i = 0; i < anList.getLength(); i++) {
    Setting& s = anList[i];
    std::string type = s["type"];
    if(type == "separated") {
      int l = s["len"];
      AAnimation& a = _loaded[std::string((const char*)s["name"])];
      a._sprites = std::make_shared<std::vector<ASprite> >();
      a._sprites->resize(l);
      if(s.lookupValue("property", a.property)) {
        a.property = a.property / PIXELS_IN_METER;
      }

      for(int j = 0; j < l; j++) {
        sf::Sprite& spr = a._sprites->operator [](j);
        std::stringstream sstr;
        if(j < 10) {
          sstr << '0';
        }
        sstr << j;
        path.clear();
        path = path + dirPath + "/" + (const char*)(s["path"])
             + "/" + (const char*)s["name"] + sstr.rdbuf()->str() + ".png";
             std::cout << path << std::endl;
        spr.setTexture(*TC.GetTexture(path));
        spr.setOrigin(spr.getTexture()->getSize().x / 2, spr.getTexture()->getSize().y / 2);
        spr.setScale((double)s["xSprSize"] / spr.getTexture()->getSize().x * PIXELS_IN_METER,
                      (double)s["ySprSize"] / spr.getTexture()->getSize().y * PIXELS_IN_METER);
      }
    }
  }
}

void AAnimationContainer::GetAnimationsNames(std::set<std::string>& set) {
  for(auto p : _loaded) {
    set.insert(p.first);
  }
}


AAnimation::AAnimation() {
}

ASprite* AAnimation::Step(double dt) {
  curTime += dt / 1000000;
  if(curTime > 2 * length) {
    curTime = 0.0;
  } else
  if(curTime > length) {
    curTime -= length;
  }
  //size_t s = curTime / length * _sprites->size();
  //std::cerr << "index " << s << '\n';
  return &((*_sprites)[size_t(curTime / length * _sprites->size())]);
}

void AAnimation::SetTime(double t) {
  if(t > 1.0 || t < 0.0) throw std::logic_error("wrong time");
  curTime = length * t;
}

void AAnimation::SetLength(double l) {
  curTime = curTime * l / length;
  length = l;
}

void AAnimation::Reset() {
  curTime = 0.0;
}
