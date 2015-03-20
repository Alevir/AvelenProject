/*
    This file is part of Avelen Project.
    Copyright (c) 2014  Evdokimov Mikhail

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

*/
#include "AScriptWrapper.hpp"
#include <memory>
#include <cassert>
#include <boost/lexical_cast.hpp>
#include "const.h"
#include "AScriptArea.hpp"
#include "AWorldBase.h"
#include "ACharacterBase.h"
#include "Global.h"



int AScriptWrapper::getPlayer() {
  return mWorld->GetPlayer()->GetID();
}

void AScriptWrapper::AddMessage(const std::string& m) {
  mWorld->AddMessage(mTrans.GetTranslation(m));
}

void AScriptWrapper::ApplyLinearImpulse(unsigned int id, double ix, double iy) {
  auto obj = mWorld->GetObjectByID(id);
  assert(obj);
  obj->ApplyLinearImpulse(ix, iy);
}

std::string AScriptWrapper::GetTemplateName(unsigned int id) {
  APhysicObjectBase* obj = mWorld->GetObjectByID(id);
  assert(obj);
  return obj->GetTemplateName();
}

void AScriptWrapper::AddEffect(unsigned int id, const std::string& effectName, double duration, double period, double measure) {
  ACharacterBase* ch = dynamic_cast<ACharacterBase*>(mWorld->GetObjectByID(id));
  if(ch) {
    EffectData ed;
    ed.Duration = duration * SECOND;
    ed.Period = period * SECOND;
    ed.Measure = measure;
    ed.Name = effectName;
    ch->AddEffect(ed);
  }
}

AScriptWrapper::AScriptWrapper(AWorldBase* world)
: mWorld(world), mTrans(Game::TranslationsPath + Game::Language + "/") {
  mTrans.SetFile("scriptMessages");
  mTrans.AddErrorString = false;
  ScrExec.add(fun(&AScriptWrapper::getPlayer), "GetPlayer");
  ScrExec.add(fun(&AScriptWrapper::ApplyLinearImpulse, this), "ApplyLinearImpulse");
  ScrExec.add(fun(&AScriptWrapper::AddMessage, this), "AddMessage");
  ScrExec.add(fun(&AScriptWrapper::GetTemplateName, this), "GetTemplateName");
  ScrExec.add(fun(&AScriptWrapper::AddEffect, this), "AddEffect");
  ScrExec.eval("var dt;");
  ScrExec.eval("var IDVector = Vector();");
  ScrExec.eval("var trObj");
}

void ALocationScriptContent::AddScriptArea(const std::string& script, const b2AABB& aabb, AObjectFilter* filter) {
  mAreas.push_back(new AScriptArea(mWorld, mScr, script, aabb, filter));
}

void ALocationScriptContent::Step(double dt) {
  for(auto p : mAreas) {
    p->Step(dt);
  }
}

ALocationScriptContent::~ALocationScriptContent() {
  for(auto p : mAreas) {
    delete p;
    }
}

void ALocationScriptContent::Save(libconfig::Setting& root) const {
  using namespace libconfig;
  Setting& s = root.add("scripts", Setting::TypeGroup);
  Setting& ar = s.add("areas", Setting::TypeList);
  for(AScriptArea* p : mAreas) {
    p->Save(ar.add(Setting::TypeGroup));
  }
}




