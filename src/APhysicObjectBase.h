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

#ifndef APHYSICOBJECT_H
#define APHYSICOBJECT_H
typedef long long unsigned int UniqueID;


//#include "SFML/Graphics.hpp"
#include "AObject.h"
#include <set>
#include <string>

#include "AVector2.h"

#include <cassert>
class AContactListener;
class AWorldBase;
class ALocationBase;
class ACharacterBase;
class ACombatModelBase;
class Effect;
class EffectData;
#include "APhysicObjectData.h"
#include "Box2D/Box2D.h"
#include <limits>


struct ObjectInitArgs {
  bool Coords = false;
  long int ID = -1;
  UniqueID UnID;
  double x;
  double y;
  double z = 0;
  double angle;
  ObjectInitArgs() {
    UnID = std::numeric_limits<UniqueID>::max();
  }
};

class ALightSourceBase;

class APhysicObjectBase : public AObject{
  friend class ALocationBase;
  friend class ACombatModelBase;
  friend class AWorldBase;
  friend class AContactListener;
  friend class ACharacterBase;
  double _freeContSpace;

protected:

  void dropObject(double x, double y, double z = 0.0);

  AWorldBase* world;
  bool wasChanged = true;

  virtual ~APhysicObjectBase();
  void SetDamping(double);
  void ChangeMass(double deltaMass);
  APhysicObjectBase* containingObject = 0;
  void RestoreBody(double iX, double iY, double iAngle, double iZ);
  b2Body* body = 0;
  double weight;
  unsigned int ID;
  UniqueID uniqueID;
  double z;
  double MaxHitPoints;
  double HitPoints;
  double Protection;  // for armor
  double Sharpness = 0.5;   // for weapon
  std::vector<EffectData> mStoredEffects; //for potion

  ALocationBase* mLoc;

  virtual void Destroy();
  APhysicObjectData templateData;
  void TakeObjectFromWorld();
  std::vector<std::string> mScripts;
  std::wstring mLabel;
  std::string mLabelNTr;
  APhysicObjectBase(const APhysicObjectData* data, ALocationBase* loc, const ObjectInitArgs& args);

  ALightSourceBase* mLightSource = 0;

public:



  void SetLabel(const std::string& l);
  std::wstring GetLabel() { return mLabel; }
  std::string GetNotTranslatedLabel() { return mLabelNTr; }

  enum ScriptNode {
    SN_View = 0,
    SN_Pick,
    SN_SIZE
  };

  static std::string ScriptNodeNames[];

  std::string Ingredient;
  double BottleUsed = 0.0;
  double BottleConcentration = 1.0;
  //exceptions
  class ExContainerIsFull {

  };

  const APhysicObjectData* GetTemplateData() {
    return &templateData;
  }
  std::string GetTemplateName() { return templateData.templateName; }

  std::set<APhysicObjectBase*> container;


  bool SetPosition(const AVector2& pos);


  //APhysicObjectBase(const APhysicObjectData &templateData, AWorldBase* iWorld, unsigned int ID);
  void Step(double dt);

  unsigned int GetID() { return ID; }
  unsigned int GetScriptID() { return uniqueID; }
  double GetHitPoints() { return HitPoints; }
  double GetMaxHitPoints() { return MaxHitPoints; }

  double GetSharpness();
  double GetVelocityModulus();
  double GetProtection();
  double GetX();
  double GetY();
  double GetZ();
  double GetAngle();
  double GetWeight();
  double GetVolume();
  AVector2 GetPosition();
  APhysicObjectBase* GetContainingObject() { return containingObject; }

  void ApplyLinearImpulse(double x, double y);
  void ApplyAngularImpulse(double w);
  void ApplyLinearImpulse(b2Vec2 v);

  void ApplyLinearVelocity(b2Vec2 v);
  void ApplyAngularVelocity(double w);
  ALocationBase* GetLocation() { return mLoc; }





  //container
  double GetFreeSpace() {
    return _freeContSpace;
  }

  void AddObjectToContainer(APhysicObjectBase* obj);
  APhysicObjectBase* GetObjectFromContainer(APhysicObjectBase* obj); // without creating new physic body
  APhysicObjectBase* DropObjectFromContainer(APhysicObjectBase* obj, double distX, double distY); //creates physic body
  void TransferObject(APhysicObjectBase* targetContainer, APhysicObjectBase* object);


  void SetScript(ScriptNode sn, const std::string& scr);
  void ExecuteScript(ScriptNode sn, APhysicObjectBase* triggeringObj);
  const std::vector<std::string>& GetScripts();



};

#endif // APHYSICOBJECT_H
