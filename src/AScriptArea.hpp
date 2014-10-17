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

#ifndef ASCRIPTAREA_HPP
#define ASCRIPTAREA_HPP
#include <Box2D/Box2D.h>
#include <libconfig.h++>
class AWorldBase;
class AObjectFilter;
class AScriptWrapper;

class AScriptArea {
  class scriptQueryCallback : public b2QueryCallback {
    bool ReportFixture(b2Fixture *fixture);


  public:
    AObjectFilter* mFilter = 0;
    std::vector<int> mFoundIDs;
    scriptQueryCallback(AObjectFilter* filter) : mFilter(filter) {}
    ~scriptQueryCallback();
  };

  scriptQueryCallback qc;
  std::string mScript;
  AWorldBase* mWorld;
  AScriptWrapper* mScr;
  b2AABB maabb;
public:
  void Save(libconfig::Setting& area);
  AScriptArea(AWorldBase* world, AScriptWrapper* scr, const std::string& script, const b2AABB& aabb, AObjectFilter* filter);
  void Step(double dt);
};

#endif // ASCRIPTAREA_HPP
