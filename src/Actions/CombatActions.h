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

#ifndef COMBATACTIONS_H
#define COMBATACTIONS_H
#include "Box2D/Box2D.h"
#include "Action.h"
#include "ACharacter.h"
#include "const.h"
#include "Common.h"
#include <iostream>

class AHumanCombatModel;

class ACharacter;

class AttackAction
    : public DisposableAction {
protected:
  HitPosition pos;
  ACharacter* character;

  class RayCastCallback
      : public b2RayCastCallback {

    AttackAction* action;
  public:
    RayCastCallback(AttackAction* iAction);
    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction);
  };
  RayCastCallback rcc;
  bool process(b2Fixture* fixture);

public:
  AttackAction(double delay, ACharacter* iCharacter, HitPosition pos);
  virtual void Activate();
  virtual ~AttackAction() {}
};

//смена позиции руки
class PositionChangeAction : public RepeatingAction {
  double position;
  double dPos;
  AHumanCombatModel* cModel;
public:
  double Celerity;
  PositionChangeAction(double period, double celerity, double position, AHumanCombatModel* cModel)
    : RepeatingAction(100 * SECOND, period), position(position), cModel(cModel) {
    Celerity = celerity;
    dPos = period * celerity;
    //std::cout << "moving to " << position << std::endl;
  }
  void Activate();
  virtual ~PositionChangeAction();
};

#endif // COMBATACTIONS_H
