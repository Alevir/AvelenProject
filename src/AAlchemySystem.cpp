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
#include "AAlchemySystem.hpp"




APotionRecipe::APotionRecipe(const libconfig::Setting& s) {
  using namespace libconfig;
  name = (const char*)s["name"];
  Setting& ingrs = s["ingrs"];
  for(int i = 0; i < ingrs.getLength(); i++) {
    Setting& ing = ingrs[i];
    ingr.insert(std::make_pair((const char*)ing["ingr"], (double)ing["portion"]));
  }
  Setting& tls = s["tools"];
  for(int i = 0; i < tls.getLength(); i++) {
    tools.insert((const char*)tls[i]);
  }
}


AExtractionRecipe::AExtractionRecipe(const libconfig::Setting& s) {
  using namespace libconfig;
  name = (const char*)s["name"];
  Setting& out = s["output"];
  for(int i = 0; i < out.getLength(); i++) {
    Setting& t = out[i];
    output.insert(std::make_pair((const char*)t["ingr"], (double)t["portion"]));
  }
  Setting& tls = s["tools"];
  for(int i = 0; i < tls.getLength(); i++) {
    tools.insert((const char*)tls[i]);
  }

}
