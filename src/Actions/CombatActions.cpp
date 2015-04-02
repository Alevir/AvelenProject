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

#include "CombatActions.h"
#include "ACharacter.h"
#include "AWorldBase.h"
#include "ACombatModelBase.h"
#include <iostream>
#include <cassert>
#include "CombatModels/AHumanCombatModel.h"


float32 AttackAction::RayCastCallback::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
  //std::cout << "callback\n";
  action->process(fixture);
  return 0;
}

AttackAction::RayCastCallback::RayCastCallback(AttackAction *iAction) : action(iAction) {}

AttackAction::AttackAction(double delay, ACharacter *iCharacter, HitPosition pos)
    : DisposableAction(delay), pos(pos), rcc(this) {
  character = iCharacter;

}

void AttackAction::Activate() {
  ATransform tr;
  character->GetTransform(tr);
  AVector2 near(0.0, 0.1);
  near.Rotate(tr.A);
  AVector2 far(0.0, 1.5);
  far.Rotate(tr.A);
  far.x += tr.X; far.y += tr.Y;
  near.x += tr.X; near.y += tr.Y;
  character->mWorld->RayCast(&rcc, near, far);
}


bool AttackAction::process(b2Fixture *fixture) {
  ACharacter* c = dynamic_cast<ACharacter*> (static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData()));
  if(!c) return false;
  AttackData a;
  a.Pos = pos;
  character->mCombatModel->Attack(c, a);
  return true;
}


void PositionChangeAction::Activate() {
  double diff = cModel->AttackPosition - position;
  if(Abs(diff) < 0.2) {
    Remove();
    return;
  }
  if(diff > 0) {
    cModel->AttackPosition -= dPos;
  } else {
    cModel->AttackPosition += dPos;
  }
  //std::cout << dPos << std::endl;
}


PositionChangeAction::~PositionChangeAction() { }
