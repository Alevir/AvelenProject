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

#ifndef ACHARACTERBASE_H
#define ACHARACTERBASE_H

#include "APhysicObjectBase.h"
class Action;
#include "Character/Race.h"
#include "ACharacterData.hpp"
//#include "ACombatModelBase.h"
class ACombatModelBase;
class AttackAction;
class MovementAction;
class AKeyboardControllerSFML;
#include "Character/Effects.hpp"
/*
class Effect;
class EffectData;*/
#include <map>



enum InventorySlot {
  IS_Head,
  IS_Body,
  IS_Gloves,
  IS_Bracers,
  IS_Legs,
  IS_Greaves,
  IS_Feet,
  IS_Belt,
  IS_Spaulders,
  IS_Bag,
  IS_Quiver,
  IS_RightHand,
  IS_LeftHand,
  InventorySize
};

extern std::string SlotName[];


enum HitPosition {
  HitPositionLegs = 0,
  HitPositionBody,
  HitPositionHead
};


struct AttackData {
  HitPosition Pos;
};

class AControllerBase;

class ACharacterBase
: virtual public APhysicObjectBase {

  friend class AControllerBase;
  friend class AContactListener;
  friend class ACombatModelBase;
  friend class AttackAction;
  friend class MovementAction;
  friend class AKeyboardControllerSFML;
  friend class ALocationBase;
  friend Effect* Effect::CreateEffect(const EffectData &ed, ACharacterBase *ch);

  ACombatModelBase* combatModel = 0;
  AControllerBase* controller = 0;
  APhysicObjectBase* weapon;
  std::set<Action*> actions;
  std::map<std::string, Effect*> effects;
  APhysicObjectBase* DropObjectAfterDeath(APhysicObjectBase* obj, double distX, double distY);

  MovementAction* feetAction;
  double str;
  double acc;
  double reg;
  double XP;
  const CharacterRace* race;
protected:
  virtual void Destroy();
  virtual ~ACharacterBase();
  double HP = 100.0;
  double maxHP = 100.0;
  double EP = 100.0;
  double maxEP = 100.0;


public:
  const std::map<std::string, Effect*>& GetEffects();
  void AddXP(double value);
  APhysicObjectBase* Slots[int(IS_LeftHand) + 1];

  class ex_wrongSlot {
  public:
    InventorySlot TargetSlot;
    ex_wrongSlot(InventorySlot sl) : TargetSlot(sl) { }
  };

  ACharacterBase(const ACharacterData* data, ALocationBase *loc,  const ObjectInitArgs& args);
  void SetController(AControllerBase* iController);
  void SetCombatModel(ACombatModelBase* model);
  AControllerBase* GetController();
  void SetControllerState(bool enabled);

  virtual void Display();
  void Step(double dt);

  //void Equip(APhysicObjectBase* obj);
  void AddAction(Action* action);



  void EquipFromBag(APhysicObjectBase* obj, InventorySlot slot);
  void DirectEquip(APhysicObjectBase* obj, InventorySlot slot);
  void DirectEquipExc(APhysicObjectBase* obj, InventorySlot slot);    // with exceptions
  APhysicObjectBase* GetObjectFromInventory(InventorySlot sl);       // removes object from inventory
  void FreeTheSlot(InventorySlot sl);   //  put object to bag or drop, if there is no bag
  void DropSlot(InventorySlot sl, double dx = 0.0, double dy = 0.0);
  static inline bool CheckSlotCompat(APhysicObjectBase* obj, InventorySlot sl);


  void Print();
  const AControllerBase* GetController() const { return controller; }
  const ACombatModelBase* GetCombatModel() const { return combatModel; }
  bool SwitchController();
  bool AddObjectToBag(APhysicObjectBase* obj);
  void GetObjectFromBag(APhysicObjectBase* obj);

  void AddEffect(const EffectData &ed);
  bool DrinkPotion(APhysicObjectBase* potion);

  double GetStr() { return str; }
  double GetAcc() { return acc; }
  double GetReg() { return reg; }
  double GetXP() { return XP; }
  double GetMaxHP() { return maxHP; }
  double GetMaxEP() { return maxEP; }
  double GetHP() { return HP; }
  double GetEP() { return EP; }
  std::string GetRace() { return race->name; }

  //inventory exceptions
  class ExObjectDropped { };
  class ExObjectInBag { };
  APhysicObjectBase* GetSlot(InventorySlot sl) { return Slots[sl]; }


};

#endif // ACHARACTERBASE_H
