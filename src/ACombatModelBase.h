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

#ifndef ACOMBATMODELBASE_H
#define ACOMBATMODELBASE_H
#include "ACharacterBase.h"

enum class CombatModels : unsigned int {
  Human = 0
};

struct HitData {
  HitPosition position;
  ACharacterBase* attacker;
  double damage = 0.0;
};

class ACombatModelBase {
protected:
  ACharacterBase * const character;
  virtual HitData DealDamage(const AttackData& attack) = 0;
  virtual void RecieveDamage(const HitData& hit) = 0;
  std::vector<Action*> actions;
  virtual void subStep(double dt) = 0;
  double DecreaseHP(double value);
  double DecreaseEP(double value);

  double& getHitPoints(APhysicObjectBase* obj) {
    return obj->HitPoints;
  }

  double& getSharpness(APhysicObjectBase* obj) {
    assert(obj->templateData.objectType == APhysicObjectData::OT_Weapon);
    return obj->Sharpness;
  }

public:
  virtual void GenerateAction(int type) = 0;
  virtual void Step(double dt) final;
  virtual void Attack(ACharacterBase* target, const AttackData& data) final;
  ACombatModelBase(ACharacterBase* iCharacter);
  virtual unsigned int GetType() const = 0;
  virtual ~ACombatModelBase() {}

};

#endif // ACOMBATMODELBASE_H
