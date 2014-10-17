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

#ifndef GOBJECTTEMPLATEEDITOR_HPP
#define GOBJECTTEMPLATEEDITOR_HPP
#include "GCommon.h"
class APhysicObjectData;

class GObjectTemplateEditor : public GuiWindow {
  APhysicObjectData* curTempl;
  tgui::ListBox::Ptr objTempl;
  tgui::Label::Ptr lName;
  tgui::Label::Ptr lType;
  tgui::Label::Ptr lRadius;
  tgui::Label::Ptr lXLen;
  tgui::Label::Ptr lYLen;
  tgui::Label::Ptr lDensity;
  tgui::Label::Ptr lInitWeight;
  tgui::Label::Ptr lFriction;
  tgui::Label::Ptr lb2Type;
  tgui::Label::Ptr lExtVolume;
  tgui::Label::Ptr lContCapacity;
  tgui::Label::Ptr lSprite;
  tgui::Label::Ptr lObjType;
  tgui::Label::Ptr lMaterial;
  tgui::Label::Ptr lWeaponType;
  tgui::Label::Ptr lArmorType;
  tgui::Label::Ptr lProtection;

  tgui::EditBox::Ptr vName;
  tgui::EditBox::Ptr vRadius;
  tgui::EditBox::Ptr vXLen;
  tgui::EditBox::Ptr vYLen;
  tgui::EditBox::Ptr vDensity;
  tgui::EditBox::Ptr vInitWeight;
  tgui::EditBox::Ptr vFriction;
  tgui::EditBox::Ptr vExtVolume;
  tgui::EditBox::Ptr vContCapacity;
  tgui::EditBox::Ptr vProtection;

  tgui::ComboBox::Ptr vType;
  tgui::ComboBox::Ptr vb2Type;
  tgui::ComboBox::Ptr vSprite;
  tgui::ComboBox::Ptr vObjType;
  tgui::ComboBox::Ptr vMaterial;
  tgui::ComboBox::Ptr vWeaponType;
  tgui::ComboBox::Ptr vArmorType;
  tgui::Button::Ptr bCreate;
  tgui::Button::Ptr bSave;

  void typeChosen();
  void objTypeChosen();
  void templateChosen();
  void saveTempate();
  void createTemplate();

public:
  GObjectTemplateEditor(GGUI& gui);



};

#endif // GOBJECTTEMPLATEEDITOR_HPP
