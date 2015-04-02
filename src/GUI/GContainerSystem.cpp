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

#include "GUI/GContainerSystem.h"
#include "ADebugOutput.h"
#include "AWorldSFML.h"
#include "GInventory.h"
#include "Global.h"

/*
void GContainerSystem::_deleteContainer(APhysicObjectBase *objKey) {
  auto it = conts.find(objKey);
  if(it != conts.end()) {
    _forRemoval.push_back(it->second);
    conts.erase(it);
  }
}

void GContainerSystem::_deleteInventory(ACharacterBase *chKey) {
  auto it = invs.find(chKey);
  if(it != invs.end()) {
    _forRemoval.push_back(it->second);
    invs.erase(it);
  }
}*/

InventorySlot GContainerSystem::getSourceSlot() { return invSource->sourceButton->_slot; }

void GContainerSystem::ClearStep() {
  for(GContainer* c : mForRemoval) {
    delete c;
  }
  mForRemoval.clear();
}

void GContainerSystem::CheckAndDrop() {
if(!_used) {
    _checkAndDrop();
  } else {
    _used = !_used;
  }
}

void GEditorContainerSystem::_checkAndDrop() {
  if(sourceStatus == ContainerSource) {
    auto v = AWorldSFML::ConvertMouseCurrentPos();
    ATransform tr; contSource->_obj->GetTransform(tr);
    v.x -= tr.X;
    v.y -= tr.Y;
    contSource->_obj->DropObjectFromContainer(trObj, v);
    contSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
  if(sourceStatus == InventorySource) {
    auto v = AWorldSFML::ConvertMouseCurrentPos();
    ATransform tr; invSource->GetCharacter()->GetTransform(tr);
    v.x -= tr.X;
    v.y -= tr.Y;
    APhysicObjectBase* obj = invSource->GetCharacter()->GetSlot(getSourceSlot());
    invSource->GetCharacter()->DropSlot(getSourceSlot());
    obj->SetTransform(v);
    invSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
}

void GInGameContainerSystem::_checkAndDrop() {
  if(!mPLI) return;
  ACharacter* mPlayer = mPLI->GetCharacter();
  if(sourceStatus == ContainerSource) {
    if(contSource->_obj->GetPosition().GetDistance(mPlayer->GetPosition()) >= TransferDistance) {
      throw ExLongDistance();
    }
    AVector2 v(0.0, 0.2);
    v.Rotate(contSource->_obj->GetTransform().A);

    contSource->_obj->DropObjectFromContainer(trObj, v);
    contSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
  if(sourceStatus == InventorySource) {
    if(invSource->GetCharacter()->GetPosition().GetDistance(mPlayer->GetPosition()) >= TransferDistance) {
      throw ExLongDistance();
    }
    AVector2 v(0.0, 0.2);
    v.Rotate(invSource->GetCharacter()->GetTransform().A);
    invSource->GetCharacter()->DropSlot(getSourceSlot(), v);
    invSource->Refresh();
    sourceStatus = NoSource;
    }
}

void GInGameContainerSystem::removePlayerInventory() {
  mForRemoval.push_back(mPLI);
  mPLI = 0;
}

GInGameContainerSystem::GInGameContainerSystem(GGUI& iGui)
  : GContainerSystem(iGui) {
}

void GInGameContainerSystem::AddPlayerInventory(ACharacter* player) {
  mPLI = new GPlayerInventory(player, gui, this);
}

GInGameContainerSystem::~GInGameContainerSystem() {
  if(mPLI) {
    delete mPLI;
  }
}


GContainerSystem::GContainerSystem(GGUI &iGui) : gui(iGui) {
}

GInventory* GContainerSystem::AddInventory(ACharacter *ch) {
  auto it = invs.find(ch);
  if(it == invs.end()) {
    GInventory* inv = new GInventory(ch, gui, this);
    invs.insert(std::make_pair(ch, inv));
    return inv;
  }
  gui.Show(it->second);
  return it->second;
}


GContainer* GContainerSystem::AddContainer(APhysicObjectBase* obj) {
  auto it = conts.find(obj);
  if(it == conts.end()) {
    GContainer* cont = new GContainer(obj, gui, this);
    conts.insert(std::make_pair(obj, cont));
    gui.Show(cont);
    return cont;
  }
  //... вывести поверх всех
  return it->second;
}

GContainer *GContainerSystem::GetContainer(APhysicObjectBase *obj) {
  auto it = conts.find(obj);
  if(it != conts.end()) {
    return it->second;
  }
}



GContainerSystem::~GContainerSystem() {
  CloseAll();
}

void GContainerSystem::CloseAll() {
  for(auto p : conts) {
    p.second->Hide();
    delete p.second;
  }
  for(auto p : invs) {
    p.second->Hide();
    delete p.second;
  }
  conts.clear();
  invs.clear();

}


void GContainerSystem::RemoveInventory(ACharacter *ch) {
  auto it = invs.begin();
  if((it = invs.find(ch)) != invs.end()) {
    mForRemoval.push_back(it->second);
    it->second->Hide();
    invs.erase(it);
  } else throw std::logic_error("inventory with required id doesn't exist");
}


void GContainerSystem::RemoveContainer(APhysicObjectBase *obj) {
  auto it = conts.begin();
  if((it = conts.find(obj)) != conts.end()) {
    mForRemoval.push_back(it->second);
    it->second->Hide();
    conts.erase(it);

  } else throw std::logic_error("container with required id doesn't exist");
  //dout << L"WARNING!!! Container with id " + std::to_wstring(obj->GetID()) + L"doesn't exist";
}



