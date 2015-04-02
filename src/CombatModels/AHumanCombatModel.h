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

#ifndef AHUMANCOMBATMODEL_H
#define AHUMANCOMBATMODEL_H
#include "ACombatModelBase.h"
#include "Actions/CombatActions.h"



class AHumanCombatModel
    : public ACombatModelBase {
  friend class PositionChangeAction;
  HitData dealDamage(const AttackData& attack);
  void recieveDamage(const HitData& hit);
  virtual void GenerateAction(int type);
  void handleArmorDamage(APhysicObjectBase* armor, APhysicObjectBase* weapon, double &damage);
  const double HeadHitMultiplier = 3.0;
  const double BodyHitMultiplier = 2.0;
  const double LegsHitMultiplier = 1.0;
  double AttackPosition = 1.0;
  const int ChangePositionIndex = 0;
  const int AttackIndex = 1;
public:
  AHumanCombatModel(ACharacter* mCharacter);
  void subStep(double dt);
  unsigned int GetType() const { return (unsigned int)(CombatModels::Human); }
  int GetAttackPossition() const  { return (int)(AttackPosition); }
  virtual ~AHumanCombatModel() {}

};

#endif // AHUMANCOMBATMODEL_H
