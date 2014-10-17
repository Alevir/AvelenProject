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

#ifndef ACHARACTERDATA_HPP
#define ACHARACTERDATA_HPP
#include "APhysicObjectData.h"
#include "Character/Race.h"


class ACharacterData : public APhysicObjectData {
public:
  const CharacterRace* Race;
  double Level = -1;
  double Str = -1;
  double Acc = -1;
  double Reg = -1;
  double MaxHP = -1;
  double MaxEP = -1;
  ACharacterData(libconfig::Setting& data);
};

#endif // ACHARACTERDATA_HPP
