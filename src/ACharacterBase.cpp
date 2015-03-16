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


#include "ACharacterBase.h"
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

void ACharacterBase::SetController(AControllerBase *iController) {
  if(mController) {
    delete mController;
  }
  mController = iController;
}


AControllerBase* ACharacterBase::GetController() {
  return mController;
}


bool ACharacterBase::SwitchController() {
  return mController->Switch();
}

bool ACharacterBase::AddObjectToBag(APhysicObjectBase *obj) {
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

void ACharacterBase::GetObjectFromBag(APhysicObjectBase *obj) {
  if(!Slots[IS_Bag]) {
    dout << "no bag!";
    return;
  }
  GetObjectFromContainer(obj);
}

void ACharacterBase::AddEffect(const EffectData& ed) {
  auto it = mEffects.begin();
  it = mEffects.find(ed.Name);
  if(it != mEffects.end()) {
    it->second->Add(ed);
    return;
  }
  Effect* ef = Effect::CreateEffect(ed, this);
  mEffects.insert(std::make_pair(ef->EffectName, ef));
}


bool ACharacterBase::DrinkPotion(APhysicObjectBase *potion) {
  if(potion->templateData.objectType != APhysicObjectData::OT_Potion) return false;
  for(EffectData& ef : potion->mStoredEffects) {
    AddEffect(ef);
  }
  mLoc->RemoveAObject(potion);
  return true;
}


void ACharacterBase::Step(double dt) {
  APhysicObjectBase::Step(dt);
  if(mHP < mMaxHP)
    mHP += 0.05 * mReg * world->GetLogicFrameLength() / SECOND;
  if(mEP < mMaxEP)
    mEP += mReg * world->GetLogicFrameLength() / SECOND;

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

ACharacterBase::ACharacterBase(const ACharacterData* data, ALocationBase* loc, const ObjectInitArgs& args)
    : mRace(data->Race), APhysicObjectBase(data, loc, args){
  for(int i = 0; i < InventorySize; i++) {
    Slots[i] = 0;
  }

  (data->Level < 0.0) ? (mXP = 0.0) : (mXP = data->Level);
  (data->Str < 0.0) ? (mStr = mRace->Str + mRace->StrGain * mXP) : (mStr = data->Str);
  (data->Acc < 0.0) ? (mAcc = mRace->Acc + mRace->AccGain * mXP) : (mAcc = data->Acc);
  (data->Reg < 0.0) ? (mReg = mRace->Reg + mRace->RegGain * mXP) : (mReg = data->Reg);
  (data->MaxHP < 0.0) ? (mMaxHP = mRace->InitHP + mStr) : (mMaxHP = data->MaxHP);
  (data->MaxEP < 0.0) ? (mMaxEP = mRace->InitEP + mStr) : (mMaxEP = data->MaxEP);
  mHP = mMaxHP;
  mEP = mMaxEP;

}

void ACharacterBase::Display() {

}


void ACharacterBase::Destroy() {
  for(int i = 0; i < InventorySize; i++) {
    if(Slots[i]) {
      DropObjectAfterDeath(Slots[i], 0, 0);
    }
  }
  APhysicObjectBase::Destroy();
}


ACharacterBase::~ACharacterBase() {

  if(mCombatModel) delete mCombatModel;
  if(mController) delete mController;
  for(auto p : mEffects) {
    delete p.second;
  }

}

const std::map<std::string, Effect *> &ACharacterBase::GetEffects() {
  return mEffects;
}

void ACharacterBase::AddXP(double value) {
  mStr += mRace->StrGain * value;
  mAcc += mRace->AccGain * value;
  mReg += mRace->RegGain * value;
  mMaxHP += mRace->StrGain * value;
  mMaxEP += mRace->StrGain * value;
}


void ACharacterBase::EquipFromBag(APhysicObjectBase* obj, InventorySlot slot) {
  if(!Slots[IS_Bag]) {
    dout << "no bag!";
    return;
  }
  if(Slots[slot] != 0 ||
    (obj->templateData.objectType == APhysicObjectData::OT_Armor && slot != obj->templateData.armorType)
  || (obj->templateData.objectType == APhysicObjectData::OT_Weapon && slot != IS_RightHand && slot != IS_LeftHand))
  {
    dout << "slot is busy or wrong\n";
    return;
  }
  obj->containingObject = this;
  Slots[slot] = obj;
}




void ACharacterBase::DirectEquip(APhysicObjectBase *obj, InventorySlot slot) {
  if(Slots[slot] != 0 || !CheckSlotCompat(obj, slot)) {
    if(!AddObjectToBag(obj)) {
      obj->dropObject(GetX(), GetY());
      dout << "slot is busy or wrong, no bag, object was dropped\n";
      return;
    }
    dout << "slot is busy or wrong, object was put in bag\n";
    return;
  }
  obj->containingObject = this;
  Slots[slot] = obj;
  ChangeMass(obj->GetWeight());
}


void ACharacterBase::DirectEquipExc(APhysicObjectBase *obj, InventorySlot slot) {
  if(Slots[slot] == obj) return;
  if(Slots[slot] != 0) {
      if(!AddObjectToBag(obj)) {
          obj->dropObject(GetX(), GetY());
          throw ExObjectDropped();
        }
      throw ExObjectInBag();
    }
  obj->containingObject = this;
  Slots[slot] = obj;
}

APhysicObjectBase *ACharacterBase::GetObjectFromInventory(InventorySlot sl) {
  APhysicObjectBase* obj = Slots[sl];
  Slots[sl] = 0;
  return obj;
}

void ACharacterBase::FreeTheSlot(InventorySlot sl) {
  if(!Slots[sl]) return;
  if(!AddObjectToBag(Slots[sl])) {
    Slots[sl]->dropObject(GetX(), GetY());
  }
  Slots[sl] = 0;
}

void ACharacterBase::DropSlot(InventorySlot sl, double dx, double dy) {
  if(!Slots[sl]) return;
  Slots[sl]->dropObject(GetX() + dx, GetY() + dy);
  Slots[sl] = 0;
}

bool ACharacterBase::CheckSlotCompat(APhysicObjectBase *obj, InventorySlot sl) {
  if(!obj) return false;
  return (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Armor && sl == obj->GetTemplateData()->armorType)
    || (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Weapon && (sl == IS_RightHand || sl == IS_LeftHand))
    || (obj->GetTemplateData()->objectType == APhysicObjectData::OT_Bag && sl == IS_Bag);
}

/*
void ACharacterBase::Equip(APhysicObjectBase* obj, ) {
}*/

void ACharacterBase::AddAction(Action *action) {
  mActions.insert(action);
}

void ACharacterBase::SetCombatModel(ACombatModelBase* model) {
  mCombatModel = model;
}

void ACharacterBase::SetControllerState(bool enabled) {
  mController->SetEnabled(enabled);
}


void ACharacterBase::Print() {
std::cout << "pointer: " << this << " Health: " << mHP << std::endl;
  for(int i = 0; i < InventorySize; i++) {
    std::cout << SlotName[i] << " ";
    APhysicObjectBase* obj = Slots[i];
    if(obj) {
      std::cout << Slots[i]->GetTemplateName() << ' ';
      if(obj->templateData.objectType == APhysicObjectData::OT_Weapon) {
        std::cout << "\tWeapon" << "\tHitPoints: " << obj->HitPoints << "\tSharpness: " << obj->Sharpness;
      } else
      if(obj->templateData.objectType == APhysicObjectData::OT_Armor) {
        std::cout << "\tArmor" << "\tHitPoints: " << obj->HitPoints << "\tProtection: " << obj->Protection
          << "\tDurability: " << obj->templateData.Material.Durability;
      }
    } else {
      std::cout << "NO ITEM";
    }
    std::cout << std::endl;
  }
}

APhysicObjectBase* ACharacterBase::DropObjectAfterDeath(APhysicObjectBase* obj, double distX, double distY) {
  obj->RestoreBody(GetX() + distX, GetY() + distY, GetAngle(), GetZ());
  ChangeMass(-obj->body->GetMass());
  obj->world->SetVisible(obj);
  obj->containingObject = 0;
  return obj;
}


