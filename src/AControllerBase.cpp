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

#include "AControllerBase.h"
#include "ACharacterBase.h"
#include "AVector2.h"
#include "Actions/CombatActions.h"
#include "ACombatModelBase.h"
#include "const.h"

double sinp(double x) {
  return 0.6 * sin(0.21 * x) + 1.0;
}

AControllerBase::AControllerBase(ACharacterBase *iNPC) {
  character = iNPC;
  method = sinp;
}

void AControllerBase::Step(double dt) {
  mDT = dt;
  subStep(dt);
  if(mIdle) {
    character->mCurMovState = MoveState::Idle;
  }
  mIdle = true;
}


void AControllerBase::moveForward() {
  double factor = 20;
  mIdle = false;
  character->mCurMovState = MoveState::MoveForward;
  character->ApplyLinearImpulse(AVector2(0, 1 * mDT / 33333.0).Rotate(character->GetAngle()) *= factor/* * method(mT)*/);
}



void AControllerBase::moveBack() {
  double factor = 10;
  character->mCurMovState = MoveState::MoveBack;
  mIdle = false;
  character->ApplyLinearImpulse(AVector2(0, -1 * mDT / 33333.0).Rotate(character->GetAngle()) *= factor/* * method(mT)*/);
}

void AControllerBase::moveRight() {
  double factor = 10;
  character->ApplyLinearImpulse(AVector2(-1 * mDT / 33333.0, 0).Rotate(character->GetAngle()) *= factor/* * method(mT)*/);
}

void AControllerBase::MoveLeft() {
  double factor = 10;
  character->ApplyLinearImpulse(AVector2(1 * mDT / 33333.0, 0).Rotate(character->GetAngle()) *= factor /** method(mT)*/);
}


void AControllerBase::turn(char side){
  if(side) {
    character->ApplyAngularImpulse(0.1 * mDT / 33333.0);
  } else {
    character->ApplyAngularImpulse(-0.1 * mDT / 33333.0);
  }
}

void AControllerBase::turn(int angleMeasure) {
character->ApplyAngularImpulse(0.002 * angleMeasure * mDT / 66666.0);
}

