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
#include "GContainerSystem.h"
#include "ADebugOutput.h"
#include "boost/lexical_cast.hpp"
#include "GCommon.h"
#include "Mode.h"
#include "ADebugOutput.h"
#include "ACharacterBase.h"
#include "GInventory.h"

void GContainer::_onClose() {
  csystem->_deleteContainer(_obj);
  mWindow->destroy();
  Gui.OnHideOrClose(this);
}

void GContainer::_pickObject() {
  dout << "object picked\n";
  csystem->contSource = this;
  csystem->trObj = _content[objList->getSelectedItemIndex()];
  csystem->sourceStatus = GContainerSystem::ContainerSource;

}

void GContainer::_putObject() {
  if(csystem->sourceStatus == GContainerSystem::NoSource) return;
  if(csystem->sourceStatus == GContainerSystem::ContainerSource) {
    if(csystem->contSource == this)  {
      csystem->sourceStatus = GContainerSystem::NoSource;
      csystem->contSource = 0;
      objList->deselectItem();
      csystem->_used = true;
      return;
    }
    if(csystem->contSource->_obj->GetPosition().GetDistance(_obj->GetPosition()) < csystem->TransferDistance) {
      try {
        csystem->contSource->_obj->TransferObject(_obj, csystem->trObj);
      } catch (APhysicObjectBase::ExContainerIsFull) {
        Mode::GetCurrentMode().MessageManager.AddMessage(L"Container is full");
      }
    } else {
      throw GContainerSystem::ExLongDistance();
    }
    /*csystem->contSource->_obj->GetObjectFromContainer(csystem->trObj);
    _obj->AddObjectToContainer(csystem->trObj);*/

    csystem->contSource->Refresh();
    csystem->_used = true;

  } else {
    if(csystem->invSource->character->GetPosition().GetDistance(_obj->GetPosition()) < csystem->TransferDistance) {
      if(csystem->invSource->GetCharacter() == csystem->invSource->GetSourceObject()->GetContainingObject()
      && csystem->invSource->GetSourceObject()->GetTemplateData()->objectType == APhysicObjectData::OT_Bag) {
        csystem->invSource->GetCharacter()->DropSlot(csystem->invSource->GetSourceSlot());
      } else {
        _obj->AddObjectToContainer(csystem->invSource->character->GetObjectFromInventory(csystem->invSource->GetSourceSlot()));
      }
      csystem->invSource->Refresh();
    } else {
      throw GContainerSystem::ExLongDistance();
    }
  }
  csystem->sourceStatus = GContainerSystem::NoSource;
  Refresh();
}

void GContainer::SwitchVisibility() {
if(mWindow->isVisible()) {
    mWindow->hide();
  } else {
    mWindow->show();
  }
}

void GContainer::Refresh() {
objList->removeAllItems();
_content.clear();
totalWeight = 0.0;
for(APhysicObjectBase* o : _obj->container) {
    objList->addItem(o->GetTemplateName());
    _content.push_back(o);
    totalWeight += o->GetWeight();
  }
  lWeight->setText(boost::lexical_cast<std::string>(totalWeight));
  double allSpace = _obj->GetTemplateData()->containerCapacity;
  freeSpace->setMaximum(allSpace * 10000);
  freeSpace->setValue((allSpace - _obj->GetFreeSpace()) * 10000);


}

void GContainer::SetPosition(double x, double y) {
  mWindow->setPosition(x, y);
}


GContainer::GContainer(APhysicObjectBase* obj, GGUI& iGui, GContainerSystem *iCs)
    : GuiWindowBase(iGui), _obj(obj), csystem(iCs), mWindow(Gui.Gui), objList(*mWindow), lWeight(*mWindow), freeSpace(*mWindow) {
  mWindow->load(Game::GuiConfFileName);
  auto v = sf::Mouse::getPosition(*Game::Window);
  freeSpace->load(Game::GuiConfFileName);
  freeSpace->setSize(360, 30);
  mWindow->setTitle(_obj->GetTemplateName());
  mWindow->setTitleAlignment(tgui::ChildWindow::TitleAlignmentCentered);
  mWindow->setPosition(v.x, v.y);
  mWindow->setSize(400, 450);
  mWindow->bindCallback(&GContainer::_onClose, this, tgui::ChildWindow::Closed);
  objList->bindCallback(&GContainer::_pickObject, this, tgui::ChildWindow::LeftMousePressed);
  //reinterpret_cast<containerWindow*>(&*panel)->Init(this);
  mWindow->Init(this);
  objList->load(Game::GuiConfFileName);
  objList->setPosition(20, 20);
  objList->setSize(360, 320);
  lWeight->setSize(360, 30);
  lWeight->setPosition(20, 350);
  lWeight->setText(boost::lexical_cast<std::string>(totalWeight));
  freeSpace->setPosition(20, 410);
  freeSpace->setMinimum(0);
  Refresh();
}




void GContainer::containerWindow::leftMouseReleased(float x, float y) {
  try {
    if(_cont) {
        _cont->_putObject();
      }
  } catch(GContainerSystem::ExLongDistance& e) {
    dout << "container is too far";
    dout << "\n";
    _cont->csystem->_used = true;
    _cont->csystem->sourceStatus = GContainerSystem::NoSource;
  }

  ChildWindow::leftMouseReleased(x, y);
}
