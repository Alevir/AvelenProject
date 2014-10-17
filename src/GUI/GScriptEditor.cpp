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
#include "GScriptEditor.hpp"
#include "APhysicObjectBase.h"
#include "Global.h"



void GScriptEditor::scriptChosen() {
  if(!mObj) throw std::logic_error("object is null");
  mCurScr = cName->getSelectedItemIndex();
  script->setText(mObj->GetScripts()[mCurScr]);
}

void GScriptEditor::scriptSave() {
  if(!mObj) throw std::logic_error("object is null");
  mObj->SetScript(APhysicObjectBase::ScriptNode(mCurScr), script->getText());
}

GScriptEditor::GScriptEditor(GGUI& gui)
: GuiWindow(gui), cName(*mWindow), script(*mWindow), bSave(*mWindow) {

  cName->load(Game::GuiConfFileName);
  script->load(Game::GuiConfFileName);
  bSave->load(Game::GuiConfFileName);
  cName->bindCallback(&GScriptEditor::scriptChosen, this, tgui::ComboBox::ItemSelected);
  bSave->bindCallback(&GScriptEditor::scriptSave, this, tgui::Button::LeftMouseClicked);
  const int len = 500;
  const int height = 400;
  mWindow->setSize(len, height);
  cName->setSize(len - 140, 30);
  cName->setPosition(20, 20);
  for(int i = 0; i < APhysicObjectBase::SN_SIZE; ++i) {
    cName->addItem(APhysicObjectBase::ScriptNodeNames[i]);
  }
  bSave->setSize(80, 30);
  bSave->setText("SAVE");
  bSave->setPosition(len - 100, 20);
  script->setSize(len - 40, height - 90);
  script->setPosition(20, 70);
  script->setTextSize(30);
}

void GScriptEditor::SetObject(APhysicObjectBase* obj) {
  mObj = obj;
}
