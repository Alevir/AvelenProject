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

#include <cmath>
#include <iostream>
#include "AWorldBase.h"
#include "APhysicObjectData.h"
#include "APhysicObjectBase.h"
#include "Character/Effects.hpp"
#include "boost/lexical_cast.hpp"
#include "ALocationBase.h"
#include "Global.h"



std::map<std::string, double> APhysicObjectBase::sHMap {
  {"creature", 2.0},
  {"high_object", 1.0},
  {"low_object", 0.5}
};

std::string APhysicObjectBase::ScriptNodeNames[] = {
  "SN_View",
  "SN_Pick"
};



APhysicObjectBase::APhysicObjectBase(const APhysicObjectData& data, ALocationBase* loc, const ObjectInitArgs& args)
    : mTemplateData(data)   {
  mWorld = loc->GetMasterWorldBase();
  mLoc = loc;
  if(args.ID < 0) {
    //throw std::logic_error("unacceptable!");
    mID = mWorld->idManager.AddObject(this);
  } else {
    mID = args.ID;
    mWorld->idManager.AddPair(args.ID, this);
  }


  if(args.UnID == std::numeric_limits<UniqueID>::max()) {
    mUniqueID = mWorld->uniqueIDManager.GetNewID();
  } else {
    mUniqueID = args.UnID;
    mWorld->uniqueIDManager.AddValue(mUniqueID);
  }

  mWeight = mTemplateData.InitWeight;
  if(args.Coords) {
    RestoreBody(this, args.Tr, args.Z);
    /*b2BodyDef body_def;
    body_def.type = mTemplateData.b2type;
    body_def.position.Set(args.X, args.Y);
    body_def.angle = args.Angle;
    mBody = mWorld->physicWorld->CreateBody(&body_def);

    b2FixtureDef fix_def;
    if(mTemplateData.fixtureType == APhysicObjectData::FixtureTypeBox) {
      b2PolygonShape* shape = new b2PolygonShape();
      shape->SetAsBox(mTemplateData.XLength / 2.0 , mTemplateData.YLength / 2.0);
      fix_def.shape = shape;
    } else
    if(mTemplateData.fixtureType == APhysicObjectData::FixtureTypeCircle) {
      b2CircleShape* shape = new b2CircleShape();
      shape->m_radius = mTemplateData.Radius;
      shape->m_p = b2Vec2(0, 0);
      fix_def.shape = shape;
    } else {
      throw std::logic_error(std::string("unknown type in template ") + mTemplateData.templateName);
    }
    fix_def.density = mTemplateData.density;
    fix_def.friction = mTemplateData.friction;
    auto cg = APhysicObjectData::CollisionGroup[data->collGroup];
    fix_def.filter.categoryBits = cg.first;
    fix_def.filter.maskBits = cg.second;
    mBody->CreateFixture(&fix_def);
    delete fix_def.shape;
    mBody->SetUserData(this);
    b2MassData md;
    mBody->GetMassData(&md);
    md.mass = mWeight;
    mBody->SetMassData(&md);*/

  }


  if(mTemplateData.objectType == APhysicObjectData::OT_Armor) {
    mProtection = mTemplateData.Protection;
  }
  mMaxHitPoints = mTemplateData.InitMaxHitPoints;
  mHitPoints = mMaxHitPoints;
  mFreeContSpace = mTemplateData.containerCapacity;
  mScripts.resize(2, "");
}

void APhysicObjectBase::Display(double dt) {
  if(!mDrawer) throw std::logic_error("no drawer");
  if(mContainingObject) return;
  mDrawer->Draw(dt);
}

void APhysicObjectBase::Extrapolate(double dt) {
  if(!mDrawer) throw std::logic_error("no drawer");
  mDrawer->Extrapolate(dt);
  mWasChanged = false;

}

bool APhysicObjectBase::CheckForChanges() {
  return mWasChanged;
}




void APhysicObjectBase::Step(double dt) {
  mWasChanged = true;
}

double APhysicObjectBase::GetSharpness() {
  assert(mTemplateData.objectType == APhysicObjectData::OT_Weapon);
  return mSharpness;
}

double APhysicObjectBase::GetVelocityModulus() {
  if(mBody) {
    return mBody->GetLinearVelocity().Length();
  } else return -1.0;
}

double APhysicObjectBase::GetProtection() {
  assert(mTemplateData.objectType == APhysicObjectData::OT_Armor);
  return mProtection;
}


void APhysicObjectBase::Destroy() {
if(mContainingObject) {
    mContainingObject->mContainer.erase(mContainingObject->mContainer.find(this));
  }
  //? drop all objects
  for(APhysicObjectBase* obj : mContainer) {
    dropObject(obj, GetTransform(), sHMap[obj->GetTemplateData()->collGroup]);
  }
}

void APhysicObjectBase::TakeObjectFromWorld() {
  if(mBody) {
    //? change impulse
    mWorld->physicWorld->DestroyBody(mBody);
    mBody = 0;
  }
  mWorld->SetInvisible(this);
}

void APhysicObjectBase::SetLabel(const std::string& l) {
  mLabelNTr = l;
  Game::Translations->SetFile("objectLabels");
  mLabel = Game::Translations->GetTranslation(l);
}

void APhysicObjectBase::dropObject(APhysicObjectBase* obj, const ATransform& tr, double z) {
  if(!obj->mBody) {
    RestoreBody(obj, tr, z);
  } else {
    obj->SetTransform(tr);
  }
  obj->mContainingObject->ChangeMass(-obj->mWeight);
  obj->mContainingObject = 0;
}

APhysicObjectBase::~APhysicObjectBase() {
  mWorld->idManager.ReleaseID(mID);
  if(mBody) {
    mWorld->physicWorld->DestroyBody(mBody);
  }
}

void APhysicObjectBase::RestoreBody(APhysicObjectBase* obj, ATransform t, double z) {
  b2BodyDef body_def;
  body_def.type = obj->mTemplateData.b2type;
  body_def.position.Set(t.X, t.Y);
  body_def.angle = t.A;
  obj->mBody = obj->mWorld->physicWorld->CreateBody(&body_def);

  b2FixtureDef fixDef;
  if(obj->mTemplateData.fixtureType == APhysicObjectData::FixtureTypeBox) {
    b2PolygonShape* shape = new b2PolygonShape();
    shape->SetAsBox(obj->mTemplateData.XLength / 2.0 , obj->mTemplateData.YLength / 2.0);
    fixDef.shape = shape;
  } else
  if(obj->mTemplateData.fixtureType == APhysicObjectData::FixtureTypeCircle) {
    b2CircleShape* shape = new b2CircleShape();
    shape->m_radius = obj->mTemplateData.Radius;
    shape->m_p = b2Vec2(0, 0);
    fixDef.shape = shape;
  } else {
    throw std::logic_error(std::string("unknown type in template ") + obj->mTemplateData.templateName);
  }

  fixDef.density = obj->mTemplateData.density;
  fixDef.friction = obj->mTemplateData.friction;
  auto cg = APhysicObjectData::CollisionGroup[obj->mTemplateData.collGroup];
  fixDef.filter.categoryBits = cg.first;
  fixDef.filter.maskBits = cg.second;
  obj->mBody->CreateFixture(&fixDef);
  obj->mBody->SetUserData(obj);
  b2MassData md;
  obj->mBody->GetMassData(&md);
  md.mass = obj->mWeight;
  obj->mBody->SetMassData(&md);

  obj->mZ = sHMap[obj->mTemplateData.collGroup];
  obj->mWorld->SetVisible(obj);
}



double APhysicObjectBase::GetZ() {
  return mZ;
}

void APhysicObjectBase::GetTransform(ATransform& tr) const {
  if(mBody) {
    auto v = mBody->GetPosition();
    tr.X = v.x;
    tr.Y = v.y;
    tr.A = mBody->GetAngle();
    return;
  }
  if(!mContainingObject) throw std::logic_error("can't find coords");
  mContainingObject->GetTransform(tr);
}

ATransform APhysicObjectBase::GetTransform() {
  ATransform tr;
  GetTransform(tr);
  return tr;
}

void APhysicObjectBase::GetTransformExtr(ATransform& tr) {
  mDrawer->GetExtrapolatedCoords(tr);
}

AVector2 APhysicObjectBase::GetPosition() {
  ATransform tr; GetTransform(tr);
  return AVector2(tr.X, tr.Y);
}

void APhysicObjectBase::GetVelocity(ATransform& tr) const {
  if(mBody) {
    auto v = mBody->GetLinearVelocity();
    tr.X = v.x;
    tr.Y = v.y;
    tr.A = mBody->GetAngularVelocity();
    return;
  }
  if(!mContainingObject) throw std::logic_error("can't find coords");
    mContainingObject->GetVelocity(tr);
}


double APhysicObjectBase::GetWeight() const {
  return mWeight;
}


double APhysicObjectBase::GetVolume() const {
  return mTemplateData.externalVolume;
}


void APhysicObjectBase::AddObjectToContainer(APhysicObjectBase* obj) {
  if(!obj) return;
  if(obj == this) {
    throw std::logic_error("can't put object inside itself!!!");
  }
  if(obj == mContainingObject) {
    throw std::logic_error("WTF?!");
  }
  std::set<APhysicObjectBase*>::iterator it;
  it = mContainer.find(obj);
  if(it != mContainer.end()) {
    throw std::logic_error("object is already in container");
  }


  if(obj->GetVolume() <= mFreeContSpace) {
    mFreeContSpace -= obj->GetVolume();
  } else {
    throw ExContainerIsFull();
  }

  ChangeMass(obj->mWeight);
  if(obj->mBody) {
    //? change impulse
    mWorld->physicWorld->DestroyBody(obj->mBody);
    obj->mBody = 0;
  }
  mContainer.insert(obj);
  obj->mWorld->SetInvisible(obj);
  obj->mContainingObject = this;
}


APhysicObjectBase* APhysicObjectBase::GetObjectFromContainer(APhysicObjectBase* obj) {
  std::set<APhysicObjectBase*>::iterator it;
  it = mContainer.find(obj);
  if(it == mContainer.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  ChangeMass(-obj->mWeight);
  mFreeContSpace += obj->GetVolume();
  mContainer.erase(it);
  obj->mContainingObject = 0;
  return obj;
}


void APhysicObjectBase::TransferObject(APhysicObjectBase* targetContainer, APhysicObjectBase* object) {
  std::set<APhysicObjectBase*>::iterator it;
  it = mContainer.find(object);
  if(it == mContainer.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  targetContainer->AddObjectToContainer(object);
  mFreeContSpace += object->GetVolume();
  mContainer.erase(it);
  ChangeMass(-object->mWeight);
  object->mContainingObject = targetContainer;

}

void APhysicObjectBase::SetScript(APhysicObjectBase::ScriptNode sn, const std::string& scr) {
  mScripts.at(sn) = scr;
}

void APhysicObjectBase::ExecuteScript(APhysicObjectBase::ScriptNode sn, APhysicObjectBase* triggeringObj) {
  mWorld->GetScriptWrapper().ScrExec.eval("trObj = " + boost::lexical_cast<std::string>(triggeringObj->GetID()));
  mWorld->GetScriptWrapper().ScrExec.eval(mScripts.at(sn));

}

const std::vector<std::string>& APhysicObjectBase::GetScripts() const {
return mScripts;
}

bool APhysicObjectBase::IsVisible() {
  return !mContainingObject;
}


const std::set<APhysicObjectBase*> APhysicObjectBase::GetContainedObjects() const {
  return mContainer;
}


APhysicObjectBase* APhysicObjectBase::DropObjectFromContainer(APhysicObjectBase* obj, const ATransform& shift) {
  std::set<APhysicObjectBase*>::iterator it;
  it = mContainer.find(obj);
  if(it == mContainer.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  mContainer.erase(it);
  mFreeContSpace += obj->GetVolume();

  RestoreBody(obj, GetTransform() += shift, sHMap[obj->GetTemplateData()->collGroup]);
  ChangeMass(-obj->mWeight);

  obj->mWorld->SetVisible(obj);
  obj->mContainingObject = 0;
  return obj;
}

void APhysicObjectBase::ApplyLinearImpulse(double x, double y) {
  mBody->ApplyLinearImpulse(b2Vec2(x,y), mBody->GetWorldCenter(), true);
}

void APhysicObjectBase::ApplyAngularImpulse(double w) {
  mBody->ApplyAngularImpulse(w, true);
}

void APhysicObjectBase::ApplyLinearImpulse(b2Vec2 v) {
  mBody->ApplyLinearImpulse(v, mBody->GetWorldCenter(), true);
}

void APhysicObjectBase::ApplyLinearVelocity(b2Vec2 v) {
  if(mBody) {
    mBody->SetLinearVelocity(v);
  } else throw std::logic_error("");
}

void APhysicObjectBase::ApplyAngularVelocity(double w) {
  if(mBody) {
    mBody->SetAngularVelocity(w);
  } else throw std::logic_error("");
}



void APhysicObjectBase::SetDamping(double d) {
  if(!mBody) return;
  mBody->SetLinearDamping(d);
  mBody->SetAngularDamping(d);
}




void APhysicObjectBase::ChangeMass(double deltaMass) {
  mWeight += deltaMass;
  if(!mBody) {
    if(!mContainingObject) throw std::logic_error("object has no body and pointer to containing object is null at the same time");
    mContainingObject->ChangeMass(deltaMass);
  } else {
    b2MassData md;
    mBody->GetMassData(&md);
    md.mass += deltaMass;
    mBody->SetMassData(&md);
  }
}


bool APhysicObjectBase::SetTransform(const ATransform& tr) {
  if(!mBody) return false;
  mBody->SetTransform(b2Vec2(tr.X, tr.Y), tr.A);
  return true;
}


ATransform&ATransform::operator +=(const ATransform& tr) {
  X+=tr.X;
  Y+=tr.Y;
  A+=tr.A;
}
