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

    Author of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef GCONTAINERSYSTEM_H
#define GCONTAINERSYSTEM_H
#include <set>
#include <map>
class APhysicObjectBase;
class GContainer;
class GInventory;
class GPlayerInventory;
#include <TGUI/TGUI.hpp>
#include "ACharacter.h"
class GGUI;

class GContainerSystem {

protected:
  std::vector<GContainer*> mForRemoval;
  GGUI& gui;
  friend class GContainer;
  friend class GInventory;
  std::map<APhysicObjectBase*, GContainer*> conts;
  std::map<ACharacter*, GInventory*> invs;
  enum SourceStatus {
    NoSource,
    InventorySource,
    ContainerSource
  };

  SourceStatus sourceStatus = NoSource;
  GContainer* contSource;
  GInventory* invSource;
  APhysicObjectBase* trObj = 0;

  /*void _deleteContainer(APhysicObjectBase* objKey);
  void _deleteInventory(ACharacterBase* chKey);*/

  bool _used = false; //were any actions performed or not, needs for dropping
  virtual void _checkAndDrop() = 0;
  InventorySlot getSourceSlot();

public:
  double TransferDistance = 1.0e200;
  void ClearStep();

  void CheckAndDrop();

  GContainerSystem(GGUI& iGui);

  GInventory* AddInventory(ACharacter* ch);
  GContainer* AddContainer(APhysicObjectBase* obj);

  GContainer* GetContainer(APhysicObjectBase* obj);

  void RemoveInventory(ACharacter* ch);
  void RemoveContainer(APhysicObjectBase* obj);

  virtual ~GContainerSystem();

  //exceptions
  class ExLongDistance {

  };

  virtual void CloseAll();
};


class GEditorContainerSystem : public GContainerSystem {
  void _checkAndDrop();
public:
  GEditorContainerSystem(GGUI& iGui)
  : GContainerSystem(iGui) {}



};

class GInGameContainerSystem : public GContainerSystem {
  void _checkAndDrop();
  GPlayerInventory* mPLI = 0;
  friend class GPlayerInventory;
  void removePlayerInventory();

public:
  GInGameContainerSystem(GGUI& iGui);
  void AddPlayerInventory(ACharacter* player);

  GPlayerInventory* GetPlayerInventory() { return mPLI; }
  ~GInGameContainerSystem();

};


#endif // GCONTAINERSYSTEM_H
