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

#ifndef GSCRIPTEDITOR_HPP
#define GSCRIPTEDITOR_HPP

class APhysicObjectBase;
#include <GUI/GCommon.h>

class GScriptEditor : public GuiWindow {
  tgui::ComboBox::Ptr cName;
  tgui::TextBox::Ptr script;
  tgui::Button::Ptr bSave;
  APhysicObjectBase* mObj = 0;
  int mCurScr = 0;

  void scriptChosen();
  void scriptSave();
public:
  GScriptEditor(GGUI& gui);
  void SetObject(APhysicObjectBase* obj);

};

#endif // GSCRIPTEDITOR_HPP
