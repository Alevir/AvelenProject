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

#include "AWorldBase.h"
#include <iostream>
#include <fstream>
#include <libconfig.h++>
#include <stdexcept>
#include "ALocationBase.h"
#include "ADebugOutput.h"
#include "AScriptWrapper.hpp"
#include "ACharacterBase.h"
#include "AContactListener.h"
#include <cstdio>
#include "Common.h"
#include "Global.h"
using namespace libconfig;
using namespace std;





AWorldBase::AWorldBase() {
  Config cfg;
   try {
    cfg.readFile("../resources/SPWorldSettings.cfg");
  } catch (libconfig::ParseException& ex) {
    std::cout << "file: " << ex.getFile() << "\nline: " << ex.getLine()
      << "\nerror: " << ex.getError() << endl;
      throw ex;
  }
  cfg.lookupValue("world", WorldPath);
  physicWorld = new b2World(b2Vec2(0,0));
  physicWorld->SetContactListener(new AContactListener());
  _curTimeG = _curTimeP = boost::date_time::microsec_clock<ptime>::local_time();
  scriptWrapper = new AScriptWrapper(this);
}

AWorldBase::~AWorldBase() {
  delete physicWorld;/*
  for(std::string s : mForRemoval) {
    remove(("../workspace/" + s).c_str());
  }
  remove("../workspace/main.map");*/

//delete cl;
}

void AWorldBase::QueryAABB(b2QueryCallback *callback, const b2AABB &aabb) const {
  physicWorld->QueryAABB(callback, aabb);
}

void AWorldBase::RayCast(b2RayCastCallback *callback, const b2Vec2 &point1, const b2Vec2 &point2) const {
physicWorld->RayCast(callback, point1, point2);
}

void AWorldBase::AddMessage(const wstring& m) {
std::wcout << m << std::endl;
}

void AWorldBase::LoadLocation(int x, int y) {
  int ii = 0, jj = 0;
  int iLim = Game::LocLoadNum.y;
  int jLim = Game::LocLoadNum.x;
  for(int i = -iLim; i <= iLim; ++i) {
    for(int j = -jLim; j <= jLim; ++j) {
      if(x > 0 && x + j < 1)  {
        jj = x + j - 1;
      } else
      if(x < 0 && x + j > -1) {
        jj = x + j + 1;
      } else  jj = x + j;

      if(y > 0 && y + i < 1)  {
        ii = y + i - 1;
      } else
      if(y < 0 && y + i > -1) {
        ii = y + i + 1;
      } else  ii = y + i;

      if(jj == x && ii == y)
        currentLoc = getLocation(jj, ii);
      else getLocation(jj, ii);
    }
  }
}

void AWorldBase::Save(const string& path) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile((mapPath + "main.map").c_str());
  auto v = GetPlayer()->GetPosition();
  findLocation(v.x, v.y);
  cfg.lookup("startX") = int(v.x);
  cfg.lookup("startY") = int(v.y);
  cfg.lookup("topID") = (long long int)uniqueIDManager.GetTop();
  cfg.writeFile((mapPath + "main.map").c_str());

  if(path.back() == '/') {
    CopyFile(mapPath + "main.map", path + "main.map");
    for(int i = 0; i < 4; ++i) {
      for(auto& p : mMap[i]) {
        for(auto& q : p) {
          if(q.Loc) {
            q.Loc->Save(mapPath + q.Path);
          }
          CopyFile(mapPath + q.Path, path + q.Path);
        }
      }
    }
  } else {
    for(int i = 0; i < 4; ++i) {
      for(auto& p : mMap[i]) {
        for(auto& q : p) {
          if(q.Loc) {
            q.Loc->Save(mapPath + q.Path);
          }
        }
      }
    }
    PackSave("../workspace/", path);
  }

}

void AWorldBase::Reload(const string& savePath) {
  for(int q = 0; q < 4; ++q) {
    for(auto& v : mMap[q]) {
      for(auto& l : v) {
        if(l.Loc) {
          delete l.Loc;
          l.Loc = 0;
        }
        l.Path.clear();
      }
    }
  }
  idManager.Clear();
  uniqueIDManager.Clear();
  Load(savePath);
}


double AWorldBase::LogicStep() {
  if(!pause) {
    _prevTimeP = _curTimeP;
    _curTimeP = boost::date_time::microsec_clock<ptime>::local_time();
    double delta = (_curTimeP - _prevTimeP).total_microseconds();
    _timeCountP += delta;
    if(_timeCountP > frameLengthP) {
      _logicStep(frameLengthP);
      _timeCountP = 0;
    }
    return frameLengthP;
  } else {
    pause = false;
    return 0;
  }
}

void AWorldBase::GraphicStep() {
  _prevTimeG = _curTimeG;
  static int fpsCount = 0;
  fpsCount++;
  _curTimeG = boost::date_time::microsec_clock<ptime>::local_time();
  double delta = (_curTimeG - _prevTimeG).total_microseconds();
  _timeCountG += delta;
  if(_timeCountG > SECOND) {
    _timeCountG = 0;
    fpsCount = 0;
  }
  //if(_timeCountG > frameLengthG) {
    _graphicStep(/*frameLengthG*/ delta);
 //   _timeCountG -= frameLengthG;
 // }
}

ALocationBase* AWorldBase::getLocation(int x, int y) {
  if(!(x && y)) throw std::logic_error("x or y == 0");
  int q;
  if(x > 0) {
    if(y > 0) { q = 0; }
    else      { q = 1; y = -y; }
  } else {
    if(y < 0) { q = 2; x = -x; y =-y; }
    else      { q = 3; x =-x; }
  }
  try {
    locPair& p = mMap[q].at(y-1).at(x-1);

    if(p.Loc) return p.Loc;
    if(p.Path != "") {
      libconfig::Config c;
      c.readFile((mapPath + p.Path).c_str());
      ContextLocArgs la;
      la.Sx = (x +  (x > 0 ? -1 : 0)) * locX;
      la.Sy = (y +  (y > 0 ? -1 : 0)) * locY;
      la.Lx = x;
      la.Ly = y;
      return p.Loc = newLocation(c, la);
    }
  } catch(std::out_of_range& e) {
    return 0;
  }

  //if(!loc) throw std::logic_error("null location");
  return 0;
}

void AWorldBase:: setLocation(const locPair& locp, int x, int y) {
  if(!(x && y)) throw std::logic_error("x or y == 0");
  int q;
   if(x > 0) {
    if(y > 0) {
      q = 0;
    } else {
      q = 1;
      y = -y;
    }
  } else {
    if(y < 0) {
      q = 2;
      x = -x; y = -y;
    } else {
      q = 3;
      x = -x;
    }
  }
  if(y > mMap[q].size()) mMap[q].resize(y);
  if(x > mMap[q][y-1].size()) mMap[q][y-1].resize(x);
  mMap[q][y-1][x-1] = locp;
}

void AWorldBase::prepareForLoading(const string& path) {
  mForRemoval.clear();
  if(path.back() == '/') {
    char buffer[1];
    std::ifstream input;
    std::ofstream output;
    input.open(path + "main.map");
    std::fstream wMap;
    wMap.open("../workspace/main.map", std::ofstream::trunc | std::ofstream::out | std::ofstream::in);
    do {
      input.read(buffer, sizeof(buffer));
      if (input.good())
      wMap.write(buffer, sizeof(buffer));
    } while (! input.eof());
    input.close();
    wMap.close();

    using namespace libconfig;
    Config cfg;
    cfg.readFile("../workspace/main.map");
    Setting& locs = cfg.lookup("locs");


    for(int i = 0; i < locs.getLength(); ++i) {
      std::string fname = (const char*)(locs[i][2]);
      input.open(path + fname);
      output.open("../workspace/" + fname      , std::ofstream::trunc | std::ofstream::out);
      mForRemoval.push_back(fname);

      do {
        input.read(buffer, sizeof(buffer));
        if (input.good())
        output.write(buffer, sizeof(buffer));
      } while (! input.eof());

      input.close();
      output.close();
    }
  } else {
    UnpackSave(path, "../workspace/");
  }
}

void AWorldBase::Load(const string& savePath) {
  prepareForLoading(savePath);
  using namespace libconfig;
  Config cfg;
  try {
    cfg.readFile("../workspace/main.map");
  } catch(ParseException& e) {
    std::cerr << "\nfile " << e.getFile()
    << "\nline " << e.getLine()
    << "\nerror " << e.getError();
    throw e;
  }

  Setting& root = cfg.getRoot();


  //size of each location
  locX = root["locX"];
  locY = root["locY"];
  int x;
  int y;
  int ix = root["startX"];
  int iy = root["startY"];
  uniqueIDManager.SetTop((long long int)root["topID"]);
  Setting& locs = root["locs"];
  for(int i = 0; i < locs.getLength(); ++i) {
    setLocation(locPair(0, (const char*)(locs[i][2])), locs[i][0], locs[i][1]);
  }
  LoadLocation(ix, iy);
  if(!currentLoc) throw std::logic_error("start location doesnt exist");
}


void AWorldBase::_logicStep(double dt) {
  for(int q = 0; q < 4; ++q) {
    for(auto& v : mMap[q]) {
      for(auto& l : v) {
        if(l.Loc) l.Loc->Step(dt);
      }
    }
  }
  physicWorld->Step(dt/SECOND, 5, 5);
}


void AWorldBase::OneStepPause() {
  pause = true;
}



ALocationBase* AWorldBase::GetCurrentLocation() {
  return currentLoc;
}


void AWorldBase::RemoveObject(APhysicObjectBase *obj) {
  if(obj == player) player = 0;
}

void AWorldBase::findLocation(double& x, double& y) {
  x = floor(x / locX);
  if(x > -1) ++x;
  y = floor(y / locY);
  if(y > -1) ++y;
}


AScriptWrapper& AWorldBase::GetScriptWrapper() {
  return *scriptWrapper;
}
