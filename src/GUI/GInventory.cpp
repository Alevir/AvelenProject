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

#include <boost/lexical_cast.hpp>
#include <ADebugOutput.h>
#include "GContainerSystem.h"
#include <cstdlib>
#include "ACharacter.h"
#include "GInventory.h"
#include "Global.h"
typedef unsigned int uint;

void GInventory::_setUsed() { csystem->_used = true; }

void GInventory::_invPickObject(ItemButton *b) {
  if(!b->obj) return;
  csystem->sourceStatus = GContainerSystem::InventorySource;
  csystem->invSource = this;
  sourceButton = b;
}




void GInventory::Refresh() {
  for(int i = 0; i < InventorySize; i++) {
    invCells[i]->Refresh();
  }
  if(character->GetSlot(IS_Bag)) {
    objList->show();
    SetBag(character->GetSlot(IS_Bag));
    noBag->hide();
  } else {
    objList->hide();
    noBag->show();
  }
  GContainer::Refresh();
}

GInventory::GInventory(ACharacter* ch, GGUI& iGui, GContainerSystem *iCs)
    : GContainer(ch->Slots[IS_Bag], iGui, iCs), noBag(*mWindow) {
  character = ch;
  sf::Vector2u size = Game::Window->getSize();
  mWindow->load(Game::GuiConfFileName);
  mWindow->setPosition(20, size.y / 3);
  sf::Vector2u panelSize(size.x * 4 / 9, size.y * 2 / 3 - 40);
  mWindow->setSize(panelSize.x, panelSize.y);
  mWindow->hide();
  double contLen = panelSize.x / 2 - 40;
  objList->setSize(contLen, panelSize.y - 120);
  objList->setPosition(panelSize.x / 2 + 20, 20);
  lWeight->setPosition(panelSize.x / 2 + 20, panelSize.y - 90);
  lWeight->setSize(contLen, 30);
  freeSpace->setPosition(panelSize.x / 2 + 20, panelSize.y - 50);
  freeSpace->setSize(contLen, 30);
  noBag->setPosition(panelSize.x / 2 + 20, 20);
  noBag->setSize(contLen, 30);
  noBag->setText("no bag");

  invCells = static_cast<ItemButton::Ptr*>(operator new[](InventorySize * sizeof(ItemButton::Ptr)));
  cellLabels = static_cast<tgui::Label::Ptr*>(operator new[](InventorySize * sizeof(tgui::Label::Ptr)));
  for(int i = 0; i < InventorySize; i++) {
    new ((void*)(invCells + i)) ItemButton::Ptr(*mWindow);
    invCells[i]->Init(this, character->Slots[i], InventorySlot(i));
    invCells[i]->setSize(panelSize.x / 4 - 30, (panelSize.y - 20) * 2 / 3 / InventorySize);
    invCells[i]->setPosition(panelSize.x / 4 + 10, 10 + i*(panelSize.y - 20) / InventorySize);
    new ((void*)(cellLabels + i)) tgui::Label::Ptr(*mWindow);
    cellLabels[i]->setSize(panelSize.x / 4 - 30, (panelSize.y - 20) * 2 / 3 / InventorySize);
    cellLabels[i]->setPosition(20, 10 + i*(panelSize.y - 20) / InventorySize);
    cellLabels[i]->setText(SlotName[i]);
    cellLabels[i]->setTextSize(20);
  }
  Refresh();
}


GInventory::~GInventory() {
  for(int i = 0; i < InventorySize; i ++) {
    invCells[i].~SharedWidgetPtr();
  }
  operator delete[] (invCells);
  for(int i = 0; i < InventorySize; i ++) {
    cellLabels[i].~SharedWidgetPtr();
  }
  operator delete[] (cellLabels);
}


void GInventory::SetBag(APhysicObjectBase* bag) {
  GContainer::_obj = bag;
}

void GInventory::ItemButton::Refresh() {
  obj = _inv->character->GetSlot(_slot);
  setText( obj ? obj->GetTemplateName() : "no item");
}

GInventory::ItemButton::ItemButton() {
  load(Game::GuiConfFileName);
  setTextColor(sf::Color(250,250,250));
}

void GInventory::ItemButton::Init(GInventory* inv, APhysicObjectBase* iObj, InventorySlot sl) {
  _inv = inv;
  obj = iObj;
  _slot = sl;
  setText( obj ? obj->GetTemplateName() : "no item");
}


void GInventory::ItemButton::leftMousePressed(float x, float y) {
  dout << "itemButton pressed!\n";
  _inv->_invPickObject(this);
}


APhysicObjectBase *GInventory::_invPutObject(InventorySlot sl) {
  if(csystem->sourceStatus == GContainerSystem::NoSource) return 0;
  APhysicObjectBase* obj;
  try {
    if(csystem->sourceStatus == GContainerSystem::InventorySource) {
      obj = csystem->invSource->sourceButton->obj;
    } else {
      obj = csystem->trObj;
    }


    if(ACharacter::CheckSlotCompat(obj, sl)) {

      if(csystem->sourceStatus == GContainerSystem::InventorySource) {
        csystem->invSource->character->GetObjectFromInventory(csystem->invSource->sourceButton->_slot);
      } else {
        csystem->contSource->GetObject()->GetObjectFromContainer(obj);
        csystem->contSource->Refresh();
      }



      character->FreeTheSlot(sl);
      character->DirectEquipExc(obj, sl);
    }


  } catch (ACharacter::ExObjectInBag& e) {
    dout << "Object was put in bag.\n";
  } catch (ACharacter::ExObjectDropped& e) {
    dout << "Object dropped!\n";
  }
  Refresh();
  return obj;
}

void GInventory::ItemButton::_leftMouseReleased(float x, float y) {
  if(x < getPosition().x || x > (getPosition().x + getSize().x)
  || y < getPosition().y || y > (getPosition().y + getSize().y)) return;
  _inv->selButton = this;
  obj = _inv->_invPutObject(_slot);



  dout << obj;
  dout << "\n";
  _inv->_clearSource();
  _inv->_setUsed();
  Refresh();


}


void GInventory:: _putObject() {
  selButton = 0;
  for(int i = 0; i < InventorySize; i++) {
    AVector2 v = sf::Mouse::getPosition(*Game::Window);
    v -= mWindow->getPosition();
    std::cout << mWindow->getTitleBarHeight() << "b \n";
    v.x -= mWindow->getBorders().left;
    v.y -= mWindow->getTitleBarHeight();
    invCells[i]->_leftMouseReleased(v.x, v.y);
    if(selButton) return;
    Refresh();
  }
  GContainer::_putObject();

}

void GInventory::_contRefr() { csystem->contSource->Refresh(); }

void GInventory::_invRefr() { csystem->invSource->sourceButton->Refresh(); }

bool GInventory::_isContSource() { return csystem->sourceStatus == GContainerSystem::ContainerSource; }

void GInventory::_clearSource() { csystem->sourceStatus = GContainerSystem::NoSource; }

InventorySlot GInventory::GetSourceSlot() {
return sourceButton->_slot;
}

APhysicObjectBase* GInventory::GetSourceObject() {
  return character->GetSlot(sourceButton->_slot);
}



void GPlayerInventory::_onClose() {
  Hide();
  //mWindow->destroy();
}

void GPlayerInventory::Hide() {
  Gui.OnHideOrClose(this);
  mWindow->hide();
  csystem->CloseAll();
}

GPlayerInventory::GPlayerInventory(ACharacter *ch, GGUI& iGui, GContainerSystem *iCs)
: GInventory(ch, iGui, iCs) {

//panel->bindCallback(&GPlayerInventory::onClose, this, tgui::ChildWindow::Closed);
}

GPlayerInventory::~GPlayerInventory() {
  mWindow->destroy();
}
