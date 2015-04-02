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

#include "ACombatModelBase.h"
#include "Action.h"

ACombatModelBase::ACombatModelBase(ACharacter *iCharacter)
    : mCharacter(iCharacter)  {
}


void ACombatModelBase::Step(double dt)
{
  subStep(dt);
  auto it = mActions.begin();
  while(it != mActions.end()) {
    if(*it == 0) {
      it++;
      continue;
    }
    (*it)->GetDuration() -= dt;
    if( (*it)->GetDuration() < 0 ) {
      (*it)->Step(dt);
      delete *it;
      *it = 0;
    } else {
      if((*it)->IsRepeating) {
        (*it)->Step(dt);
      }
    }
    it++;
  }
}

void ACombatModelBase::Attack(ACharacter* target, const AttackData &data) {
  target->mCombatModel->recieveDamage(mCharacter->mCombatModel->dealDamage(data));
}

double ACombatModelBase::decreaseHP(double value) {
  return mCharacter->mHP -= value;
}

double ACombatModelBase::decreaseEP(double value) {
  mCharacter->mEP -= value;
  if(mCharacter->mEP < 0.0) mCharacter->mEP = 0.0;
  return mCharacter->mEP;
}

