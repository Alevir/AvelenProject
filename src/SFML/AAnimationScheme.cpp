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

#include "AAnimationScheme.hpp"
#include "Global.h"
#include <stdexcept>



void AAnimationScheme::SetState(const std::string& state) {

  auto res = anims.find(state);
  //if(res == anims.end()) throw std::logic_error("there is no " + state + "state");
  if(res == anims.end()) {
    cur = &anims["Idle"];
    curAnimName = "Idle";
    return;
  }
  cur = &(res->second);
  curAnimName = state;
  cur->Reset();
}

AAnimationScheme::AAnimationScheme(const libconfig::Setting& s) {
  using namespace libconfig;
  Setting& l = s["anims"];
  if(l.getLength() < 1) {
    std::string str(""); str = str + "animation scheme " + (const char*)s["name"] + "is empty";
    throw std::logic_error(str);
  }
  for(int i = 0; i < l.getLength(); i++) {
    anims.insert(std::make_pair((const char*)l[i]["name"], Game::animContainer->GetAnimation((const char*)l[i]["anim"])));
  }
  auto p = anims.begin();
  curAnimName = p->first;
  std::cout << "setting default animation " << curAnimName << " in scheme " << (const char*)s["name"] << "\n";
  cur = &p->second;
}

AAnimationScheme::AAnimationScheme(const AAnimationScheme& as)
: anims(as.anims), curAnimName(as.curAnimName) {
  auto p = anims.begin();
  curAnimName = p->first;
  std::cout << "setting default animation " << curAnimName << "\n";
  cur = &p->second;
}

ASprite* AAnimationScheme::Step(double dt) {
  return cur->Step(dt);
}
