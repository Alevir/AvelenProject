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

void GContainerSystem::_deleteContainer(APhysicObjectBase *objKey) {
  auto it = conts.find(objKey);
  if(it != conts.end()) {
    delete it->second;
    conts.erase(it);
  }
}

void GContainerSystem::_deleteInventory(ACharacterBase *chKey) {
  auto it = invs.find(chKey);
  if(it != invs.end()) {
    delete it->second;
    invs.erase(it);
  }
}

InventorySlot GContainerSystem::getSourceSlot() { return invSource->sourceButton->_slot; }

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
    v.x -= contSource->_obj->GetX();
    v.y -= contSource->_obj->GetY();
    contSource->_obj->DropObjectFromContainer(trObj, v.x, v.y);
    contSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
  if(sourceStatus == InventorySource) {
    auto v = AWorldSFML::ConvertMouseCurrentPos();
    v.x -= invSource->GetCharacter()->GetX();
    v.y -= invSource->GetCharacter()->GetY();
    APhysicObjectBase* obj = invSource->GetCharacter()->GetSlot(getSourceSlot());
    invSource->GetCharacter()->DropSlot(getSourceSlot());
    obj->SetPosition(v);
    invSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
}

void GInGameContainerSystem::_checkAndDrop() {
  if(sourceStatus == ContainerSource) {
    if(contSource->_obj->GetPosition().GetDistance(mPlayer->GetPosition()) >= TransferDistance) {
      throw ExLongDistance();
    }
    AVector2 v(0.0, 0.2);
    v.Rotate(contSource->_obj->GetAngle());

    contSource->_obj->DropObjectFromContainer(trObj, v.x, v.y);
    contSource->Refresh();
    sourceStatus = NoSource;
    return;
  }
  if(sourceStatus == InventorySource) {
    if(invSource->GetCharacter()->GetPosition().GetDistance(mPlayer->GetPosition()) >= TransferDistance) {
      throw ExLongDistance();
    }
    AVector2 v(0.0, 0.2);
    v.Rotate(invSource->GetCharacter()->GetAngle());
    invSource->GetCharacter()->DropSlot(getSourceSlot(), v.x, v.y);
    invSource->Refresh();
    sourceStatus = NoSource;
    }
}

void GInGameContainerSystem::CloseAll() {
  GContainerSystem::CloseAll();
  mPLI->Hide();
}

GInGameContainerSystem::GInGameContainerSystem(GGUI& iGui, ACharacterBase* player)
  : GContainerSystem(iGui), mPlayer(player) {
  mPLI = new GPlayerInventory(mPlayer, iGui, this);
}

GInGameContainerSystem::~GInGameContainerSystem() {
delete mPLI;
}


GContainerSystem::GContainerSystem(GGUI &iGui) : gui(iGui) {
}

GInventory* GContainerSystem::AddInventory(ACharacterBase *ch) {
auto it = invs.find(ch);
  if(it == invs.end()) {
    GInventory* inv = new GInventory(ch, gui, this);
    invs.insert(std::make_pair(ch, inv));
    return inv;
  }
  return it->second;
}


GContainer* GContainerSystem::AddContainer(APhysicObjectBase* obj) {
  auto it = conts.find(obj);
  if(it == conts.end()) {
    GContainer* cont = new GContainer(obj, gui, this);
    conts.insert(std::make_pair(obj, cont));
    return cont;
  }
  return it->second;
}

GContainer *GContainerSystem::GetContainer(APhysicObjectBase *obj) {
  auto it = conts.find(obj);
  if(it != conts.end()) {
    return it->second;
  }
}



GContainerSystem::~GContainerSystem() {
  for(std::pair<ACharacterBase*, GInventory*> inv : invs) {
    delete inv.second;
  }
  for(std::pair<APhysicObjectBase*, GContainer*> c : conts) {
    delete c.second;
    }
}

void GContainerSystem::CloseAll() {
  for(auto p : conts) {
    delete p.second;
  }
  for(auto p : invs) {
    delete p.second;
  }
  conts.clear();
  invs.clear();

}


void GContainerSystem::RemoveInventory(ACharacterBase *ch) {
  auto it = invs.begin();
  if((it = invs.find(ch)) != invs.end()) {
    delete it->second;
    invs.erase(it);
  } else throw std::logic_error("inventory with required id doesn't exist");
}


void GContainerSystem::RemoveContainer(APhysicObjectBase *obj) {
  auto it = conts.begin();
  if((it = conts.find(obj)) != conts.end()) {
    delete it->second;
    conts.erase(it);
  } else throw std::logic_error("container with required id doesn't exist");
}



