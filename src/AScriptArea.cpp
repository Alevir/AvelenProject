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

#include "AScriptArea.hpp"
#include "AObjectFilter.hpp"
#include "AWorldBase.h"
#include "AScriptWrapper.hpp"
#include "APhysicObjectBase.h"
#include "Common.h"


void AScriptArea::Save(libconfig::Setting& area) {
  using namespace libconfig;
  area.add("script", Setting::TypeString) = ToCfgFormat(mScript);
  area.add("lowerx", Setting::TypeFloat) = maabb.lowerBound.x;
  area.add("lowery", Setting::TypeFloat) = maabb.lowerBound.y;
  area.add("upperx", Setting::TypeFloat) = maabb.upperBound.x;
  area.add("uppery", Setting::TypeFloat) = maabb.upperBound.y;
  if(qc.mFilter) qc.mFilter->Save(area.add("filter", Setting::TypeGroup));

}

AScriptArea::AScriptArea(AWorldBase* world, AScriptWrapper* scr, const std::string& script, const b2AABB& aabb, AObjectFilter* filter)
: qc(filter), mScript(script), mWorld(world), mScr(scr), maabb(aabb) {

}

void AScriptArea::Step(double dt) {
  qc.mFoundIDs.clear();
  ChaiScript& scr = mScr->ScrExec;
  mWorld->QueryAABB(&qc, maabb);
  if(qc.mFoundIDs.empty()) return;
  scr.eval(std::string("dt = ") + boost::lexical_cast<std::string>(dt / SECOND) + ";");
  scr.eval("IDVector.clear();");
  //std::cout << "foung: " << qc.mFoundIDs.size() << std::endl;
  for(int i = 0; i < qc.mFoundIDs.size(); i++) {
    scr.eval(std::string("IDVector.push_back(") + boost::lexical_cast<std::string>(qc.mFoundIDs[i]) + ");\n");
  }
  scr.eval(mScript);
}




bool AScriptArea::scriptQueryCallback::ReportFixture(b2Fixture* fixture) {
  APhysicObjectBase* obj = static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData());
  if(mFilter) {
    mFilter->Check(obj);
  }
  if(obj)
    mFoundIDs.push_back(obj->GetID());
  return true;
}

AScriptArea::scriptQueryCallback::~scriptQueryCallback() { if(mFilter) delete mFilter; }



