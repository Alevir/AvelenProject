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


#include "ACharacter.h"
#include "AControllerBase.h"
//#include "AWorldNS.h"
#include "AWorldBase.h"
#include "Action.h"
#include "ACombatModelBase.h"
#include "Character/Effects.hpp"
#include "ADebugOutput.h"
#include "ALocationBase.h"


std::string MoveStateNames[] = {
  "Idle",
  "MoveForward",
  "MoveBack",
  "MoveRight",
  "MoveLeft",
  "Attack"
};

std::string SlotName[] = {
  "Head",
  "Body",
  "Gloves",
  "Bracers",
  "Legs",
  "Greaves",
  "Feet",
  "Belt",
  "Spaulders",
  "Bag",
  "Quiver",
  "RightHand",
  "LeftHand"
  };

void ACharacter::SetController(AControllerBase *iController) {
  if(mController) {
    delete mController;
  }
  mController = iController;
}


AControllerBase* ACharacter::GetController() {
  return mController;
}


bool ACharacter::SwitchController() {
  return mController->Switch();
}

bool ACharacter::AddObjectToBag(APhysicObjectBase *obj) {
  if(!Slots[IS_Bag]) {
    if(obj->GetTemplateData()->objectType == APhysicObjectData::OT_Bag) {
      obj->TakeObjectFromWorld();
      DirectEquip(obj, IS_Bag);
      return true;
    } else {
      return false;
    }
  }
  Slots[IS_Bag]->AddObjectToContainer(obj);
  return true;
}

void ACharacter::GetObjectFromBag(APhysicObjectBase *obj) {
  if(!Slots[IS_Bag]) {
    dout << "no bag!";
    return;
  }
  GetObjectFromContainer(obj);
}

void ACharacter::AddEffect(const EffectData& ed) {
  auto it = mEffects.begin();
  it = mEffects.find(ed.Name);
  if(it != mEffects.end()) {
    it->second->Add(ed);
    return;
  }
  Effect* ef = Effect::CreateEffect(ed, this);
  mEffects.insert(std::make_pair(ef->EffectName, ef));
}


bool ACharacter::DrinkPotion(APhysicObjectBase *potion) {

  if(potion->mTemplateData.objectType != APhysicObjectData::OT_Potion) return false;
  for(EffectData& ef : potion->mStoredEffects) {
    AddEffect(ef);
  }
  mLoc->RemoveAObject(potion);
  return true;
}


void ACharacter::Step(double dt) {
  APhysicObjectBase::Step(dt);
  if(mHP < mMaxHP)
    mHP += 0.05 * mReg * mWorld->GetLogicFrameLength() / SECOND;
  if(mEP < mMaxEP)
    mEP += mReg * mWorld->GetLogicFrameLength() / SECOND;

  if(mController) {
    mController->Step(dt);
  }
  if(mCombatModel) {
    mCombatModel->Step(dt);
  }
  auto it = mActions.begin();
  decltype(it) eit;
  while(it != mActions.end()) {
    (*it)->GetDuration() -= dt;
    if( (*it)->GetDuration() < 0 ) {
      eit = it;
      it++;
      (*eit)->Activate();
      //std::cout << "Activated \n";
      //delete *eit;
      mActions.erase(eit);
    } else {
      it++;
    }
  }
  auto efit = mEffects.begin();
  decltype(efit) eefit;
  while(efit != mEffects.end()) {
    //std::cerr << efit->second->EffectName + ": " << efit->second->GetDuration() << '\n';
    efit->second->GetDuration() -= dt;
    efit->second->Step(dt);
    if( efit->second->GetDuration() < 0 ) {
      eefit = efit;
      efit++;
      eefit->second->Activate();
      mEffects.erase(eefit);
      delete eefit->second;
    } else {
      efit++;
    }
  }
  if(mHP > mMaxHP) mHP = mMaxHP;
  if(mEP > mMaxEP) mEP = mMaxEP;

}

ACharacter::ACharacter(const ACharacterData& data, ALocationBase* loc, const ObjectInitArgs& args)
    : mRace(data.Race), APhysicObjectBase(data, loc, args){
  for(int i = 0; i < InventorySize; i++) {
    Slots[i] = 0;
  }

  (data.Level < 0.0) ? (mXP = 0.0) : (mXP = data.Level);
  (data.Str < 0.0) ? (mStr = mRace->Str + mRace->StrGain * mXP) : (mStr = data.Str);
  (data.Acc < 0.0) ? (mAcc = mRace->Acc + mRace->AccGain * mXP) : (mAcc = data.Acc);
  (data.Reg < 0.0) ? (mReg = mRace->Reg + mRace->RegGain * mXP) : (mReg = data.Reg);
  (data.MaxHP < 0.0) ? (mMaxHP = mRace->InitHP + mStr) : (mMaxHP = data.MaxHP);
  (data.MaxEP < 0.0) ? (mMaxEP = mRace->InitEP + mStr) : (mMaxEP = data.MaxEP);
  mHP = mMaxHP;
  mEP = mMaxEP;

}

void ACharacter::Display() {

}


void ACharacter::Destroy() {
  for(int i = 0; i < InventorySize; i++) {
    if(Slots[i]) {
      DropObjectAfterDeath(Slots[i], 0, 0);
    }
  }
  APhysicObjectBase::Destroy();
}


ACharacter::~ACharacter() {

  if(mCombatModel) delete mCombatModel;
  if(mController) delete mController;
  for(auto p : mEffects) {
    delete p.second;
  }

}

void ACharacter::GetInfo(ACharacter::Info& info) {
  info.Strength = mStr;
  info.Accuracy = mAcc;
  info.Regen = mReg;
  info.HP = mHP;
  info.MaxHP = mMaxHP;
  info.EP = mEP;
  info.MaxEP = mMaxEP;
  info.Name = "unnamed";
  info.Race = GetRace();
}

const std::map<std::string, Effect *> &ACharacter::GetEffects() {
  return mEffects;
}

void ACharacter::AddXP(double value) {
  mStr += mRace->StrGain * value;
  mAcc += mRace->AccGain * value;
  mReg += mRace->RegGain * value;
  mMaxHP += mRace->StrGain * value;
  mMaxEP += mRace->StrGain * value;
}


void ACharacter::EquipFromBag(APhysicObjectBase* obj, InventorySlot slot) {
  if(!Slots[IS_Bag]) {
    dout << "no bag!";
    return;
  }
  if(Slots[slot] != 0 ||
    (obj->mTemplateData.objectType == APhysicObjectData::OT_Armor && slot != obj->mTemplateData.armorType)
  || (obj->mTemplateData.objectType == APhysicObjectData::OT_Weapon && slot != IS_RightHand && slot != IS_LeftHand))
  {
    dout << "slot is busy or wrong\n";
    return;
  }
  obj->mContainingObject = this;
  Slots[slot] = obj;
}




void ACharacter::DirectEquip(APhysicObjectBase *obj, InventorySlot slot) {
  if(Slots[slot] != 0 || !CheckSlotCompat(obj, slot)) {
    if(!AddObjectToBag(obj)) {
      dropObject(obj, GetTransform());
      dout << "slot is busy or wrong, no bag, object was dropped\n";
      return;
    }
    dout << "slot is busy or wrong, object was put in bag\n";
    return;
  }
  obj->mContainingObject = this;
  Slots[slot] = obj;
  ChangeMass(obj->GetWeight());
}


void ACharacter::DirectEquipExc(APhysicObjectBase *obj, InventorySlot slot) {
  if(Slots[slot] == obj) return;
  if(Slots[slot] != 0) {
      if(!AddObjectToBag(obj)) {
          dropObject(obj, GetTransform());
          throw ExObjectDropped();
        }
      throw ExObjectInBag();
    }
  obj->mContainingObject = this;
  Slots[slot] = obj;
}

APhysicObjectBase *ACharacter::GetObjectFromInventory(InventorySlot sl) {
  APhysicObjectBase* obj = Slots[sl];
  Slots[sl] = 0;
  return obj;
}

void ACharacter::FreeTheSlot(InventorySlot sl) {
  if(!Slots[sl]) return;
  if(!AddObjectToBag(Slots[sl])) {
    dropObject(Slots[sl], GetTransform());
  }
  Slots[sl] = 0;
}

void ACharacter::DropSlot(InventorySlot sl, const ATransform& tr) {
  if(!Slots[sl]) return;
  dropObject(Slots[sl], GetTransform() += tr);
  Slots[sl] = 0;
}

bool ACharacter::CheckSlotCompat(APhysicObjectBase *obj, InventorySlot sl) {
  if(!obj) return false;
  return (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Armor && sl == obj->GetTemplateData()->armorType)
    || (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Weapon && (sl == IS_RightHand || sl == IS_LeftHand))
    || (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Bag && sl == IS_Bag);
}

/*
void ACharacterBase::Equip(APhysicObjectBase* obj, ) {
}*/

void ACharacter::AddAction(Action *action) {
  mActions.insert(action);
}

void ACharacter::SetCombatModel(ACombatModelBase* model) {
  mCombatModel = model;
}

void ACharacter::SetControllerState(bool enabled) {
  mController->SetEnabled(enabled);
}


void ACharacter::Print() {
/*std::cout << "pointer: " << this << " Health: " << mHP << std::endl;
  for(int i = 0; i < InventorySize; i++) {
    std::cout << SlotName[i] << " ";
    APhysicObjectBase* obj = Slots[i];
    if(obj) {
      std::cout << Slots[i]->GetTemplateName() << ' ';
      if(obj->mTemplateData.objectType == APhysicObjectData::OT_Weapon) {
        std::cout << "\tWeapon" << "\tHitPoints: " << obj->mHitPoints << "\tSharpness: " << obj->mSharpness;
      } else
      if(obj->mTemplateData.objectType == APhysicObjectData::OT_Armor) {
        std::cout << "\tArmor" << "\tHitPoints: " << obj->mHitPoints << "\tProtection: " << obj->mProtection
          << "\tDurability: " << obj->mTemplateData.Material.Durability;
      }
    } else {
      std::cout << "NO ITEM";
    }
    std::cout << std::endl;
  }*/
}

APhysicObjectBase* ACharacter::DropObjectAfterDeath(APhysicObjectBase* obj, double distX, double distY) {
  ATransform tr; GetTransform(tr);
  tr.X += distX;
  tr.Y += distY;
  RestoreBody(obj, tr, GetZ());
  ChangeMass(-obj->mBody->GetMass());
  obj->mWorld->SetVisible(obj);
  obj->mContainingObject = 0;
  return obj;
}


