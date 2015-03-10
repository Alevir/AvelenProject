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

#ifndef AWORLD_H
#define AWORLD_H

#include <boost/date_time.hpp>
#include <map>
#include <list>
#include <set>
typedef long long unsigned int UniqueID;

#include "const.h"

#include <Box2D/Box2D.h>
#include <libconfig.h++>
class AScriptWrapper;
class ACharacterBase;
class ALocationBase;
class APhysicObjectBase;
class APhysicObjectData;
class AContactListener;
class ACharacterData;
class ContextLocArgs;
class ObjectInitArgs;
#include "AVector2.h"
#include "AIDManager.h"

using boost::posix_time::ptime;





class AWorldBase {
  friend class AvelenShell;
  friend class ALocationBase;
  friend class APhysicObjectBase;

  bool mWasPaused = false;
protected:
  bool pause = false;
  int locX = 0;
  int locY = 0;

  struct locPair {
    locPair() {}
    locPair(ALocationBase* l, const std::string& path) : Loc(l), Path(path) {}
    ALocationBase* Loc = 0;
    std::string Path = "";
  };

  std::vector<std::vector<locPair> > mMap[4];
  std::string mapPath = "../workspace/";
  std::vector<std::string> mForRemoval;


  ALocationBase* getLocation(int x, int y);
  void unloadLocation(int x, int y);
  void setLocation(const locPair& locp, int x, int y);
  void prepareForLoading(const std::string& path);


  virtual ALocationBase* newLocation(libconfig::Config& locationDesc, const ContextLocArgs& la) = 0;


  //physic
  ptime _curTimeP;
  ptime _prevTimeP;  
  double _timeCountP = 0.0;
  double frameLengthP = SECOND / 30.0;

  //graphic
  ptime _curTimeG;
  ptime _prevTimeG;
  double _timeCountG = 0.0;
  double frameLengthG = SECOND / 60.0;

 /* ALogicalThread* pLThread;
  boost::thread* pBoostLThread;*/

  int FPSCount = 0;


  AContactListener* cl;

  void _logicStep(double dt);
  virtual void _graphicStep(double dt) = 0;


  ALocationBase* currentLoc;
  virtual APhysicObjectBase* CreateObject(std::string iTemplateName, ALocationBase* loc, const ObjectInitArgs& args) = 0;
  virtual ACharacterBase* CreateCharacter(const ACharacterData* data, ALocationBase* loc, const ObjectInitArgs& args) = 0;
  virtual void RemoveObject(APhysicObjectBase* obj);

  AIDManager<APhysicObjectBase> idManager;
  UniqueIDManager uniqueIDManager;
  ACharacterBase* player = 0;
  b2World* physicWorld;
  AScriptWrapper* scriptWrapper = 0;

  void findLocation(double& x, double& y);

public:
  const std::string locationsPath = "../resources/locations/";
  AScriptWrapper& GetScriptWrapper();
  double GetLogicFrameLength() { return frameLengthP; }
  ACharacterBase* GetPlayer() { return player; }


  AWorldBase();

  double LogicStep();
  void GraphicStep();


  virtual void OneStepPause();

  virtual void SetVisible(APhysicObjectBase* obj) = 0;
  virtual void SetInvisible(APhysicObjectBase* obj) = 0;

  ALocationBase* GetCurrentLocation();
  virtual ~AWorldBase();
  virtual void ResetCurrentLocation(libconfig::Config &cfg) = 0;
  void QueryAABB(b2QueryCallback* callback, const b2AABB& aabb) const;
  void RayCast(b2RayCastCallback* callback, const b2Vec2& point1, const b2Vec2& point2) const;
  APhysicObjectBase* GetObjectByID(unsigned int id) { return idManager.GetObjectByID(id); }
  virtual void AddMessage(const std::wstring& m);

  void LoadLocation(int x, int y);
  ALocationBase* FindLocationByCoords(double x, double y) {
    AVector2 v;
    findLocation(v.x, v.y);
    return getLocation(int(v.x), int(v.y));
  }



  void Save(const std::string& path);
  void Reload(const std::string& savePath);
  void Load(const std::string& savePath);
  std::string WorldPath; //default path

};

#endif // AWORLD_H
