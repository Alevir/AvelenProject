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

#ifndef ALOCATION_H
#define ALOCATION_H

#include <set>
#include <list>
#include <vector>

#include <libconfig.h++>
#include "AScriptWrapper.hpp"
class AWorldBase;
class AGroundCell;
#include "ACharacter.h"


struct ContextLocArgs {
  double Sx = 0;      //the shifts
  double Sy = 0;
  int Lx = 0;     // position of the location in the world
  int Ly = 0;
};

class ALocationBase {
  friend class AvelenShell;
  friend class AWorldBase;

  void _writeObject(libconfig::Setting& s, APhysicObjectBase* obj);
  void _writeCharacter(libconfig::Setting& s, APhysicObjectBase* obj);
  void _writeContainer(libconfig::Setting& s, APhysicObjectBase* obj);

  void _readObject(libconfig::Setting& s, APhysicObjectBase* obj);
  void _readCharacter(libconfig::Setting& s, ACharacter* obj);
  void _readContainer(libconfig::Setting& s, APhysicObjectBase* obj);

  //load & save



protected:
  double msx;
  double msy;

  int mlx;
  int mly;


  AGroundCell*** mGround;
  static std::map<std::string, AGroundCell*> scriptGroundContainer;
  std::vector<AGroundCell*> mLoadingArray;
  std::map<AGroundCell*, unsigned int> mSavingMap;
  std::vector<std::string> mNameMap;

  int mXLen = 0;
  int mYLen = 0;
  const double cellSize = 1.0;

  std::set<APhysicObjectBase*> mLocalObjects;
  AWorldBase* world;
  void loadLocationFromCfg(libconfig::Config& c, const ContextLocArgs& la = ContextLocArgs());
  //virtual void loadGroundCells(const std::string& iLocationFileName) = 0;
  const std::string savePath = "../resources/locations/";
  ALocationScriptContent mScrCont;


  inline ALocationBase* boundsTest(APhysicObjectBase* obj, int& x, int& y);
  void transferObj(APhysicObjectBase* obj, std::set<APhysicObjectBase*>::iterator& validIt, ALocationBase* dest);
  void transferObj(std::set<APhysicObjectBase*>::iterator& it, ALocationBase* dest);
  void transferCharacter(std::set<APhysicObjectBase*>::iterator& it, ALocationBase* dest);


public:
  int GetXSize() { return mXLen; }
  int GetYSize() { return mYLen; }
  int GetLocPositionX() { return mlx; }
  int GetLocPositionY() { return mly; }
  virtual ~ALocationBase();
  ALocationBase(AWorldBase* iWorld);

  ALocationScriptContent& GetScriptContent() { return mScrCont; }
  void Save(const std::string& iFileName);
  APhysicObjectBase* AddObject(const APhysicObjectData& data, ObjectInitArgs& args);
  APhysicObjectBase* AddObject(const APhysicObjectData& data, APhysicObjectBase* container,  ObjectInitArgs& args);
  APhysicObjectBase* AddObject(const APhysicObjectData& data, ACharacter* character, InventorySlot slot,  ObjectInitArgs& args);
  //ACharacter* AddCharacter(const ACharacterData* data, ObjectInitArgs& args);
  void RemoveAObject(APhysicObjectBase* obj);
  const AGroundCell* GetCell(int x, int y);
  void Step(double dt);
  virtual void Display() = 0;
  void AddGroundCellType(AGroundCell* gc);
  AWorldBase* GetMasterWorldBase() { return world; }

};

#endif // ALOCATION_H
