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

#ifndef AOBJECTPOTION_HPP
#define AOBJECTPOTION_HPP
#include "ANonCharacterLogicObject.hpp"

class AObjectPotion : public ANonCharacterLogicObject {
  friend class AWorldBase;
protected:
  AObjectPotion(const APhysicObjectData& data, ALocationBase* loc, const ObjectInitArgs& args);
};

#endif // AOBJECTPOTION_HPP
