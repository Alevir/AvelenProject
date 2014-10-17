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

#ifndef ASCRIPTWRAPPER_HPP
#define ASCRIPTWRAPPER_HPP
#include <chaiscript/chaiscript.hpp>
//#include <chaiscript/chaiscript_stdlib.hpp>
class AWorldBase;
#include <libconfig.h++>
using namespace chaiscript;

class AScriptWrapper;
class AScriptArea;
class AObjectFilter;
class b2AABB;
#include "ATranslationReader.h"



class ALocationScriptContent {
  std::vector<AScriptArea*> mAreas;
  AWorldBase* mWorld;
  AScriptWrapper* mScr;
public:
  void AddScriptArea(const std::string& script, const b2AABB& aabb, AObjectFilter* filter = 0);
  void Step(double dt);
  ALocationScriptContent(AWorldBase* world, AScriptWrapper* scr) : mWorld(world), mScr(scr) {}
  ~ALocationScriptContent();
  void Save(libconfig::Setting& root) const;
};



class AScriptWrapper {
  AWorldBase* mWorld;
  int getPlayer();
  ATranslationReader mTrans;

public:
  void AddMessage(const std::string& m);
  void ApplyLinearImpulse(unsigned int id, double ix, double  iy);
  std::string GetTemplateName(unsigned int id);
  void AddEffect(unsigned int id, const std::string& effectName, double duration = 0.0, double period = 0.0, double measure = 0.0);
  ChaiScript ScrExec;
  AScriptWrapper(AWorldBase* world);



};

#endif // ASCRIPTWRAPPER_HPP
