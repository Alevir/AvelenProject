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

#ifndef AALCHEMYSYSTEM_HPP
#define AALCHEMYSYSTEM_HPP
#include <string>
#include <map>
#include <set>
#include <libconfig.h++>

struct APotionRecipe {
  std::string name;
  std::map<std::string, double> ingr;
  std::set<std::string> tools;
  APotionRecipe(const libconfig::Setting& s);
};


struct AExtractionRecipe {
  std::string name;   //name of source
  std::map<std::string, double> output;
  std::set<std::string> tools;
  AExtractionRecipe(const libconfig::Setting& s);
};



#endif // AALCHEMYSYSTEM_HPP
