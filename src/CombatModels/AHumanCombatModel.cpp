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

#include "AHumanCombatModel.h"
#include <iostream>
#include "AdvRandom.h"
#include "ADebugOutput.h"


AHumanCombatModel::AHumanCombatModel(ACharacterBase *character)
    : ACombatModelBase(character) {
  mActions.resize(2, 0);
}

void AHumanCombatModel::subStep(double dt) {
  
}

HitData AHumanCombatModel::dealDamage(const AttackData &attack) {
  HitData d;
  d.damage = mCharacter->Slots[IS_RightHand]->GetWeight() * 2.0 * mCharacter->GetStr() * mCharacter->GetEP() / mCharacter->GetMaxEP();
  //dout << d.damage;
  //dout << "\n";
  d.attacker = mCharacter;
  d.position = attack.Pos;
  return d;
}

void AHumanCombatModel::handleArmorDamage(APhysicObjectBase* armor, APhysicObjectBase* weapon, double& damage) {
  if(!weapon)  return;
  if(!armor || getHitPoints(armor) < 0.00001) {
    damage *= 1 + getSharpness(weapon);
    return;
  }
  double& armorHP = getHitPoints(armor);
  double& weaponSharpness = getSharpness(weapon);
  if(armorHP > 0.00001) {
    double durDiff = weapon->GetTemplateData()->Material.Durability - armor->GetTemplateData()->Material.Durability;
    if(durDiff > 0) {
      armorHP -= damage * (1 + weaponSharpness * durDiff ) * (1 - armor->GetTemplateData()->Material.Resilience);
    }
  }
  if(armorHP > 0.00001) {
    damage *= 1 - armor->GetProtection() * armorHP / armor->GetMaxHitPoints();
  } else {
    damage =  damage - armorHP * weaponSharpness;
    armorHP = 0.0;
  }
}


void AHumanCombatModel::recieveDamage(const HitData &hit) {
  double damage = hit.damage;
  if(hit.position == HitPositionHead) {
    damage *= HeadHitMultiplier;
    handleArmorDamage(mCharacter->Slots[IS_Head], hit.attacker->Slots[IS_RightHand], damage);
  } else
  if(hit.position == HitPositionBody) {
    damage *= BodyHitMultiplier;
    AdvRandom<InventorySlot> r;
    r.addValue(IS_Body, 1.0);
    r.addValue(IS_Gloves, 0.5);
    r.addValue(IS_Bracers, 0.5);
    r.addValue(IS_Spaulders, 0.5);
    handleArmorDamage(mCharacter->Slots[r.getValue()], hit.attacker->Slots[IS_RightHand], damage);
  } else
  if(hit.position == HitPositionLegs) {
    damage *= LegsHitMultiplier;
    AdvRandom<InventorySlot> r;
    r.addValue(IS_Legs, 1.0);
    r.addValue(IS_Greaves, 0.5);
    r.addValue(IS_Feet, 0.5);
    handleArmorDamage(mCharacter->Slots[r.getValue()], hit.attacker->Slots[IS_RightHand], damage);
  }
  decreaseHP(damage);
  dout << "damaged ";
  dout << damage;
  dout << '\n';
}

const int HighHit = 2;
const int MediumHit = 1;
const int LowHit = 0;

void AHumanCombatModel::GenerateAction(int type) {
  if (type < 0) return;
  if(type <= 2) {
    if(mActions[ChangePositionIndex] != 0) {
      delete mActions[ChangePositionIndex];
    }
    mActions[ChangePositionIndex] = new PositionChangeAction(1.0 / 15.0, 0.9, int(type) + 0.5, this);
  } else
  if(type == 3 && mActions[AttackIndex] == 0) {
    dout << "Attacking\n";
    if(AttackPosition > 2.99) AttackPosition = 2.0;
    if(AttackPosition < 0.0) AttackPosition = 0.001;
    mActions[AttackIndex] = new AttackAction(2 * SECOND * mCharacter->Slots[IS_RightHand]->GetWeight() / mCharacter->GetStr(), mCharacter, HitPosition(int(AttackPosition)));
  }
  decreaseEP(3);

}
