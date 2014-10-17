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

#ifndef SINGLEPLAYERMODE_HPP
#define SINGLEPLAYERMODE_HPP
#include "Mode.h"
#include "GUI/GContainerSystem.h"
#include "GUI/GInGame.hpp"

class SinglePlayerMode : public WorldMode {
  //GInventory* inventory;
  class FocusRayCast : public b2RayCastCallback {
    std::set<APhysicObjectBase*> foundObjects;
    APhysicObjectBase* focusedObject = 0;
    float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction);
    SinglePlayerMode* mode;
  public:
    FocusRayCast(SinglePlayerMode* m) : mode(m) {}
    void Step();
    void ChooseNextObject();
    APhysicObjectBase* GetFocusedObject();
  };
  GInventory* plInv;

public:
  GInGameContainerSystem GCS;
  GPlayerInterface Interface;
  void ResetPlayer();

  SinglePlayerMode();
  ~SinglePlayerMode();
  void _run();

};

#endif // SINGLEPLAYERMODE_HPP
