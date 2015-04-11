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
#include <map>

#include "AVector2.h"

#include <cassert>
class AContactListener;
class AWorldBase;
class ALocationBase;
class ACharacter;
class ACombatModelBase;
class Effect;
class EffectData;
#include "APhysicObjectData.h"
#include "Box2D/Box2D.h"
#include "AObjectDrawer.hpp"
#include <limits>


struct ATransform {
  ATransform(double x, double y, double a) : X(x), Y(y), A(a) {}
  ATransform(const AVector2& v) : X(v.x), Y(v.y), A(0.0) {}
  ATransform() : X(0.0), Y(0.0), A(0.0) {}
  double X;
  double Y;
  double A;
  ATransform& operator += (const ATransform& tr);
};


struct ObjectInitArgs {
  bool Coords = false;
  long int ID = -1;
  ATransform Tr;
  UniqueID UnID;
  double Z = 0;
  ObjectInitArgs() {
    UnID = std::numeric_limits<UniqueID>::max();
  }
  bool IsCharacter = false;
};

class ALightSourceBase;

class APhysicObjectBase : public AObject{
  friend class ALocationBase;
  friend class ACombatModelBase;
  friend class AWorldBase;
  friend class AContactListener;
  double mFreeContSpace;

  //APhysicObjectBase(const APhysicObjectBase& obj) {}
protected:

  void dropObject(APhysicObjectBase* obj, const ATransform& t, double z = 0.0);


  bool mWasChanged = true;

  virtual ~APhysicObjectBase();
  void SetDamping(double);
  void ChangeMass(double deltaMass);

  void RestoreBody(APhysicObjectBase* obj, ATransform t, double z);

  double mWeight;
  unsigned int mID;
  UniqueID mUniqueID;
  double mZ;
  double mMaxHitPoints;


  ALocationBase* mLoc;

  virtual void Destroy();

  std::vector<std::string> mScripts;
  std::wstring mLabel;
  std::string mLabelNTr;
  APhysicObjectBase(const APhysicObjectData& data, ALocationBase* loc, const ObjectInitArgs& args);

  ALightSourceBase* mLightSource = 0;

  AObjectDrawer* mDrawer = 0;
  std::set<APhysicObjectBase*> mContainer;

  static std::map<std::string, double> sHMap;
  //bool visible = false;
public:

  double mHitPoints;
  double mProtection;  // for armor
  double mSharpness = 0.5;   // for weapon
  std::vector<EffectData> mStoredEffects; //for potion
  b2Body* mBody = 0;
  AWorldBase* mWorld;

  APhysicObjectData mTemplateData;
  virtual void TakeObjectFromWorld();

  const std::set<APhysicObjectBase*> GetContainedObjects() const;

  void Display(double dt);
  void Extrapolate(double dt);

  //bool WasChanged();  //for extrapolation
  APhysicObjectBase* mContainingObject = 0;


  AWorldBase* GetWorld() { return mWorld; }
  void SetLabel(const std::string& l);
  std::wstring GetLabel() const { return mLabel; }
  std::string GetNotTranslatedLabel() const { return mLabelNTr; }

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
    return &mTemplateData;
  }
  std::string GetTemplateName() const { return mTemplateData.templateName; }




  bool SetTransform(const ATransform& tr);


  //APhysicObjectBase(const APhysicObjectData &templateData, AWorldBase* iWorld, unsigned int ID);
  virtual void Step(double dt);

  unsigned int GetID() { return mID; }
  unsigned int GetScriptID() { return mUniqueID; }
  double GetHitPoints() { return mHitPoints; }
  double GetMaxHitPoints() { return mMaxHitPoints; }

  double GetSharpness();
  double GetVelocityModulus();
  double GetProtection();

  double GetZ();
  void GetTransform(ATransform& tr) const;
  ATransform GetTransform();
  void GetTransformExtr(ATransform& tr);  //extrapolated
  AVector2 GetPosition();

  void GetVelocity(ATransform& tr) const;
  double GetWeight() const;
  double GetVolume() const;
  APhysicObjectBase* GetContainingObject() { return mContainingObject; }

  void ApplyLinearImpulse(double x, double y);
  void ApplyAngularImpulse(double w);
  void ApplyLinearImpulse(b2Vec2 v);

  void ApplyLinearVelocity(b2Vec2 v);
  void ApplyAngularVelocity(double w);
  ALocationBase* GetLocation() { return mLoc; }





  //container
  double GetFreeSpace() const {
    return mFreeContSpace;
  }

  void AddObjectToContainer(APhysicObjectBase* obj);
  APhysicObjectBase* GetObjectFromContainer(APhysicObjectBase* obj); // without creating new physic body
  APhysicObjectBase* DropObjectFromContainer(APhysicObjectBase* obj, const ATransform& shift); //creates physic body
  void TransferObject(APhysicObjectBase* targetContainer, APhysicObjectBase* object);


  void SetScript(ScriptNode sn, const std::string& scr);
  void ExecuteScript(ScriptNode sn, APhysicObjectBase* triggeringObj);
  const std::vector<std::string>& GetScripts() const;


  bool IsVisible();
  //bool CheckForChanges();
};

#endif // APHYSICOBJECT_H
