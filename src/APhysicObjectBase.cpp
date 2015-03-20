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




std::string APhysicObjectBase::ScriptNodeNames[] = {
  "SN_View",
  "SN_Pick"
};



APhysicObjectBase::APhysicObjectBase(const APhysicObjectData *data, ALocationBase* loc, const ObjectInitArgs& args)
    : templateData(*data)   {
  world = loc->GetMasterWorldBase();
  mLoc = loc;
  z = args.z;
  if(args.ID < 0) {
    //throw std::logic_error("unacceptable!");
    ID = world->idManager.AddObject(this);
  } else {
    ID = args.ID;
    world->idManager.AddPair(args.ID, this);
  }


  if(args.UnID == std::numeric_limits<UniqueID>::max()) {
    uniqueID = world->uniqueIDManager.GetNewID();
  } else {
    uniqueID = args.UnID;
    world->uniqueIDManager.AddValue(uniqueID);
  }

  weight = templateData.InitWeight;
  if(args.Coords) {
    b2BodyDef body_def;
    body_def.type = templateData.b2type;
    body_def.position.Set(args.x, args.y);
    body_def.angle = args.angle;
    body = world->physicWorld->CreateBody(&body_def);

    b2FixtureDef fix_def;
    if(templateData.fixtureType == APhysicObjectData::FixtureTypeBox) {
      b2PolygonShape* shape = new b2PolygonShape();
      shape->SetAsBox(templateData.XLength / 2.0 , templateData.YLength / 2.0);
      fix_def.shape = shape;
    } else
    if(templateData.fixtureType == APhysicObjectData::FixtureTypeCircle) {
      b2CircleShape* shape = new b2CircleShape();
      shape->m_radius = templateData.Radius;
      shape->m_p = b2Vec2(0, 0);
      fix_def.shape = shape;
    } else {
      throw std::logic_error(std::string("unknown type in template ") + templateData.templateName);
    }
    fix_def.density = templateData.density;
    fix_def.friction = templateData.friction;
    auto cg = APhysicObjectData::CollisionGroup[data->collGroup];
    fix_def.filter.categoryBits = cg.first;
    fix_def.filter.maskBits = cg.second;
    body->CreateFixture(&fix_def);
    delete fix_def.shape;
    body->SetUserData(this);
    b2MassData md;
    body->GetMassData(&md);
    md.mass = weight;
    body->SetMassData(&md);

  }


  if(templateData.objectType == APhysicObjectData::OT_Armor) {
    Protection = templateData.Protection;
  }
  MaxHitPoints = templateData.InitMaxHitPoints;
  HitPoints = MaxHitPoints;
  _freeContSpace = templateData.containerCapacity;
  mScripts.resize(2, "");
}




void APhysicObjectBase::Step(double dt) {
  wasChanged = true;
}

double APhysicObjectBase::GetSharpness() {
  assert(templateData.objectType == APhysicObjectData::OT_Weapon);
  return Sharpness;
}

double APhysicObjectBase::GetVelocityModulus() {
  if(body) {
    return body->GetLinearVelocity().Length();
  } else return -1.0;
}

double APhysicObjectBase::GetProtection() {
  assert(templateData.objectType == APhysicObjectData::OT_Armor);
  return Protection;
}


void APhysicObjectBase::Destroy() {
if(containingObject) {
    containingObject->container.erase(containingObject->container.find(this));
  }
  //? drop all objects
  for(APhysicObjectBase* obj : container) {
    obj->dropObject(GetX(), GetY(), GetZ());
    }
}

void APhysicObjectBase::TakeObjectFromWorld() {
  if(body) {
    //? change impulse
    world->physicWorld->DestroyBody(body);
    body = 0;
  }
  world->SetInvisible(this);
}

void APhysicObjectBase::SetLabel(const std::string& l) {
  mLabelNTr = l;
  Game::Translations->SetFile("objectLabels");
  mLabel = Game::Translations->GetTranslation(l);
}

void APhysicObjectBase::dropObject(double x, double y, double z) {
  if(!body) {
    RestoreBody(x, y, 0, z);
  } else {
    SetPosition(AVector2(x,y));
  }
  containingObject->ChangeMass(-weight);
  containingObject = 0;
}

APhysicObjectBase::~APhysicObjectBase() {
  world->idManager.ReleaseID(ID);
  if(body) {
    world->physicWorld->DestroyBody(body);
  }
}

void APhysicObjectBase::RestoreBody(double iX, double iY, double iAngle, double iZ) {
  b2BodyDef body_def;
  body_def.type = templateData.b2type;
  body_def.position.Set(iX, iY);
  body_def.angle = iAngle;
  body = world->physicWorld->CreateBody(&body_def);

  b2PolygonShape shape;
  if(templateData.fixtureType == APhysicObjectData::FixtureTypeBox) {
    shape.SetAsBox(templateData.XLength / 2.0 , templateData.YLength / 2.0);
  } else {
    throw std::logic_error(std::string("unknown type in template ") + templateData.templateName);
  }

  b2FixtureDef fix_def;
  fix_def.shape = &shape;
  fix_def.density = templateData.density;
  fix_def.friction = templateData.friction;
  auto cg = APhysicObjectData::CollisionGroup[templateData.collGroup];
  fix_def.filter.categoryBits = cg.first;
  fix_def.filter.maskBits = cg.second;
  body->CreateFixture(&fix_def);
  body->SetUserData(this);
  b2MassData md;
  body->GetMassData(&md);
  md.mass = weight;
  body->SetMassData(&md);

  z = iZ;
  world->SetVisible(this);
}

double APhysicObjectBase::GetX() {
  if(body) {
    return body->GetPosition().x;
  }
  if(!containingObject) throw std::logic_error("can't find coords");
  return containingObject->GetX();
}

double APhysicObjectBase::GetAngle() {
  if(body) {
    return body->GetAngle();
  }
  if(!containingObject) throw std::logic_error("can't find coords");
  return containingObject->GetAngle();
}


double APhysicObjectBase::GetY() {
    if(body) {
    return body->GetPosition().y;
  }
  if(!containingObject) throw std::logic_error("cant find coords");
  return containingObject->GetY();
}


double APhysicObjectBase::GetZ() {
  return z;
}


double APhysicObjectBase::GetWeight() {
  return weight;
}


double APhysicObjectBase::GetVolume() {
  return templateData.externalVolume;
}


void APhysicObjectBase::AddObjectToContainer(APhysicObjectBase* obj) {
  if(!obj) return;
  if(obj == this) {
    throw std::logic_error("can't put object inside itself!!!");
  }
  if(obj == containingObject) {
    throw std::logic_error("WTF?!");
  }
  std::set<APhysicObjectBase*>::iterator it;
  it = container.find(obj);
  if(it != container.end()) {
    throw std::logic_error("object is already in container");
  }


  if(obj->GetVolume() <= _freeContSpace) {
    _freeContSpace -= obj->GetVolume();
  } else {
    throw ExContainerIsFull();
  }

  ChangeMass(obj->weight);
  if(obj->body) {
    //? change impulse
    world->physicWorld->DestroyBody(obj->body);
    obj->body = 0;
  }
  container.insert(obj);
  obj->world->SetInvisible(obj);
  obj->containingObject = this;
}


APhysicObjectBase* APhysicObjectBase::GetObjectFromContainer(APhysicObjectBase* obj) {
  std::set<APhysicObjectBase*>::iterator it;
  it = container.find(obj);
  if(it == container.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  ChangeMass(-obj->weight);
  _freeContSpace += obj->GetVolume();
  container.erase(it);
  obj->containingObject = 0;
  return obj;
}


void APhysicObjectBase::TransferObject(APhysicObjectBase* targetContainer, APhysicObjectBase* object) {
  std::set<APhysicObjectBase*>::iterator it;
  it = container.find(object);
  if(it == container.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  targetContainer->AddObjectToContainer(object);
  _freeContSpace += object->GetVolume();
  container.erase(it);
  ChangeMass(-object->weight);
  object->containingObject = targetContainer;

}

void APhysicObjectBase::SetScript(APhysicObjectBase::ScriptNode sn, const std::string& scr) {
  mScripts.at(sn) = scr;
}

void APhysicObjectBase::ExecuteScript(APhysicObjectBase::ScriptNode sn, APhysicObjectBase* triggeringObj) {
  world->GetScriptWrapper().ScrExec.eval("trObj = " + boost::lexical_cast<std::string>(triggeringObj->GetID()));
  world->GetScriptWrapper().ScrExec.eval(mScripts.at(sn));

}

const std::vector<std::string>& APhysicObjectBase::GetScripts() {
  return mScripts;
}

APhysicObjectBase* APhysicObjectBase::DropObjectFromContainer(APhysicObjectBase* obj, double distX, double distY) {
  std::set<APhysicObjectBase*>::iterator it;
  it = container.find(obj);
  if(it == container.end()) {
    throw std::logic_error("required object doesn't exist");
  }
  container.erase(it);
  _freeContSpace += obj->GetVolume();
  static std::map<std::string, double> hmap {
    {"creature", 2.0},
    {"high_object", 1.0},
    {"low_object", 0.5}
  };
  obj->RestoreBody(GetX() + distX, GetY() + distY, GetAngle(), hmap[obj->GetTemplateData()->collGroup]);
  ChangeMass(-obj->weight);

  obj->world->SetVisible(obj);
  obj->containingObject = 0;
  return obj;
}

void APhysicObjectBase::ApplyLinearImpulse(double x, double y) {
  body->ApplyLinearImpulse(b2Vec2(x,y), body->GetWorldCenter(), true);
}

void APhysicObjectBase::ApplyAngularImpulse(double w) {
  body->ApplyAngularImpulse(w, true);
}

void APhysicObjectBase::ApplyLinearImpulse(b2Vec2 v) {
body->ApplyLinearImpulse(v, body->GetWorldCenter(), true);
}

void APhysicObjectBase::ApplyLinearVelocity(b2Vec2 v) {
  if(body) {
    body->SetLinearVelocity(v);
  } else throw std::logic_error("");
}

void APhysicObjectBase::ApplyAngularVelocity(double w) {
  if(body) {
    body->SetAngularVelocity(w);
  } else throw std::logic_error("");
}



void APhysicObjectBase::SetDamping(double d) {
  body->SetLinearDamping(d);
  body->SetAngularDamping(d);
}


AVector2 APhysicObjectBase::GetPosition() {
  return AVector2(GetX(), GetY());
}


void APhysicObjectBase::ChangeMass(double deltaMass) {
  weight += deltaMass;
  if(!body) {
    if(!containingObject) throw std::logic_error("object has no body and pointer to containing object is null at the same time");
    containingObject->ChangeMass(deltaMass);
  } else {
    b2MassData md;
    body->GetMassData(&md);
    md.mass += deltaMass;
    body->SetMassData(&md);
  }
}


bool APhysicObjectBase::SetPosition(const AVector2& pos) {
  if(!body) return false;
  body->SetTransform(pos, body->GetAngle());
  return true;
}
