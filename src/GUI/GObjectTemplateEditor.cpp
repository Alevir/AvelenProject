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

#include "GObjectTemplateEditor.hpp"
#include <boost/lexical_cast.hpp>
#include "ASpriteContainer.h"

#include "APhysicObjectData.h"
#include "Global.h"


void GObjectTemplateEditor::typeChosen() {
  sf::String str = vType->getSelectedItem();
  if(str == "circle") {
    lRadius->show();  vRadius->show();
    lXLen->hide();    vXLen->hide();
    lYLen->hide();    vYLen->hide();
  } else
  if(str == "box") {
    lRadius->hide();  vRadius->hide();
    lXLen->show();    vXLen->show();
    lYLen->show();    vYLen->show();
    }
}

void GObjectTemplateEditor::objTypeChosen() {
  sf::String str = vObjType->getSelectedItem();
  if(str == "Armor") {
    lArmorType->show(); vArmorType->show();
    lProtection->show(); vProtection->show();
    lWeaponType->hide(); vWeaponType->hide();
  } else {
    lArmorType->hide(); vArmorType->hide();
    lProtection->hide(); vProtection->hide();

    if(str == "Weapon") {
      lWeaponType->show(); vWeaponType->show();
    } else {
      lWeaponType->hide(); vWeaponType->hide();
    }
  }
}

void GObjectTemplateEditor::templateChosen() {
  sf::String strr = objTempl->getSelectedItem();
  if(strr.getSize() == 0) return;
  curTempl = &Game::ObjectTemplatesContainer->GetTemplate(strr);
  vName->setText(curTempl->templateName);
  APhysicObjectData::FixtureType ft = curTempl->fixtureType;
  vType->setSelectedItem(ft);
  if(ft == APhysicObjectData::FixtureTypeBox) {
    vXLen->setText(boost::lexical_cast<std::string>(curTempl->XLength));
    vYLen->setText(boost::lexical_cast<std::string>(curTempl->YLength));
  } else
  if(ft == APhysicObjectData::FixtureTypeCircle) {
    vRadius->setText(boost::lexical_cast<std::string>(curTempl->Radius));
  }
  typeChosen();
  vb2Type->setSelectedItem(curTempl->b2type);
  APhysicObjectData::ObjectType t = curTempl->objectType;
  vObjType->setSelectedItem(t);
  objTypeChosen();
  if(t == APhysicObjectData::OT_Weapon) {
    vWeaponType->setSelectedItem(curTempl->weaponType);
  } else
  if(t == APhysicObjectData::OT_Armor) {
    vArmorType->setSelectedItem(curTempl->armorType);
    vProtection->setText(boost::lexical_cast<std::string>(curTempl->Protection));
  }
  vSprite->setSelectedItem(curTempl->sprite);
  //cout << curTempl->Material.Name;


  //dout << "\n";
  vMaterial->setSelectedItem(curTempl->Material.Name);

  vDensity->setText(boost::lexical_cast<std::string>(curTempl->density));
  vFriction->setText(boost::lexical_cast<std::string>(curTempl->friction));
  vExtVolume->setText(boost::lexical_cast<std::string>(curTempl->externalVolume));
  vContCapacity->setText(boost::lexical_cast<std::string>(curTempl->containerCapacity));
}

void GObjectTemplateEditor::saveTempate() {
  bool isNew = !curTempl;
  if(isNew) {
    curTempl = new APhysicObjectData();
  }


  curTempl->templateName = vName->getText();


  APhysicObjectData::FixtureType ft = (APhysicObjectData::FixtureType)vType->getSelectedItemIndex();
  curTempl->fixtureType = ft;
  if(ft == APhysicObjectData::FixtureTypeBox) {
    curTempl->XLength = boost::lexical_cast<double>((std::string)vXLen->getText());
    curTempl->YLength = boost::lexical_cast<double>((std::string)vYLen->getText());
  } else
  if(ft == APhysicObjectData::FixtureTypeCircle) {
    curTempl->Radius = boost::lexical_cast<double>((std::string)vRadius->getText());
  }
  curTempl->b2type = b2BodyType(vb2Type->getSelectedItemIndex());
  APhysicObjectData::ObjectType t = (APhysicObjectData::ObjectType)vObjType->getSelectedItemIndex();
  curTempl->objectType = t;
  if(t == APhysicObjectData::OT_Weapon) {
    curTempl->weaponType = (APhysicObjectData::WeaponType)vWeaponType->getSelectedItemIndex();
  } else
  if(t == APhysicObjectData::OT_Armor) {
    curTempl->armorType = (APhysicObjectData::ArmorType)vArmorType->getSelectedItemIndex();
    curTempl->Protection = boost::lexical_cast<double>((std::string)vProtection->getText());
  }
  curTempl->sprite = vSprite->getSelectedItem();
  curTempl->Material = Game::LoadedMaterials[vMaterial->getSelectedItem()];



  curTempl->density = boost::lexical_cast<double>((std::string)vDensity->getText());
  curTempl->friction = boost::lexical_cast<double>((std::string)vFriction->getText());
  curTempl->externalVolume = boost::lexical_cast<double>((std::string)vExtVolume->getText());
  curTempl->containerCapacity = boost::lexical_cast<double>((std::string)vContCapacity->getText());

}

void GObjectTemplateEditor::createTemplate() {
  objTempl->deselectItem();
  curTempl = 0;
}

GObjectTemplateEditor::GObjectTemplateEditor(GGUI& gui) : GuiWindow(gui),      objTempl(*mWindow),
lName(*mWindow),        vName(*mWindow),        lType(*mWindow),          vType(*mWindow),
lRadius(*mWindow),      vRadius(*mWindow),
lXLen(*mWindow),        vXLen(*mWindow),        lYLen(*mWindow),          vYLen(*mWindow),
lDensity(*mWindow),     vDensity(*mWindow),     lInitWeight(*mWindow),    vInitWeight(*mWindow),
lFriction(*mWindow),    vFriction(*mWindow),
lExtVolume(*mWindow),   vExtVolume(*mWindow),   lContCapacity(*mWindow),  vContCapacity(*mWindow),
lb2Type(*mWindow),      vb2Type(*mWindow),      lSprite(*mWindow),        vSprite(*mWindow),
lMaterial(*mWindow),    vMaterial(*mWindow),    lObjType(*mWindow),       vObjType(*mWindow),
lWeaponType(*mWindow),  vWeaponType(*mWindow),  lArmorType(*mWindow),     vArmorType(*mWindow),
lProtection(*mWindow),  vProtection(*mWindow),  bCreate(*mWindow),        bSave(*mWindow)
{
  std::set<std::string> ss;
  Game::ObjectTemplatesContainer->GetAllTemplateNames(ss);
  vType->bindCallback(&GObjectTemplateEditor::typeChosen, this, tgui::ComboBox::ItemSelected);
  vObjType->bindCallback(&GObjectTemplateEditor::objTypeChosen, this, tgui::ComboBox::ItemSelected);
  objTempl->bindCallback(&GObjectTemplateEditor::templateChosen, this, tgui::ListBox::ItemSelected);
  bSave->bindCallback(&GObjectTemplateEditor::saveTempate, this, tgui::Button::LeftMouseClicked);

  mWindow->setSize(700,600);
  objTempl->load(Game::GuiConfFileName);
  objTempl->setPosition(20, 20);
  objTempl->setSize(220, 460);
  for(auto s : ss) {
    objTempl->addItem(s);
  }

  const int llen = 130;
  const int vlen = 280;
  const int lpos = 260;
  const int vpos = 410;
  const int vheight = 27;
  const int vind = 5;
  const int tsize = 23;
  const int bsize = 27;
  int p = 0;

  lName->load(Game::GuiConfFileName); vName->load(Game::GuiConfFileName);
  lName->setTextSize(tsize);
  lName->setText(L"Name");
  lName->setPosition(lpos, 20 + (vind + vheight) * p);  lName->setSize(llen, vheight);
  vName->setPosition(vpos, 20 + (vind + vheight) * p);  vName->setSize(vlen, vheight);
  vName->setTextSize(tsize);
  p++;

  lType->load(Game::GuiConfFileName); vType->load(Game::GuiConfFileName);
  lType->setTextSize(tsize);
  lType->setText(L"Fixture Type");
  lType->setPosition(lpos, 20 + (vind + vheight) * p);  lType->setSize(llen, vheight);
  vType->setPosition(vpos, 20 + (vind + vheight) * p);  vType->setSize(vlen, vheight);
  for(int i = 0; i < APhysicObjectData::FixtureTypeCount; i++) {
    vType->addItem(APhysicObjectData::FixtureTypeName[i]);
  }
  p++;


  lRadius->load(Game::GuiConfFileName); vRadius->load(Game::GuiConfFileName);
  lRadius->setTextSize(tsize);
  lRadius->setText(L"Radius");
  lRadius->setPosition(lpos, 20 + (vind + vheight) * p);  lRadius->setSize(llen, vheight);
  vRadius->setPosition(vpos, 20 + (vind + vheight) * p);  vRadius->setSize(vlen, vheight);
  vRadius->setTextSize(tsize);
  lRadius->hide();
  vRadius->hide();

  lXLen->load(Game::GuiConfFileName); vXLen->load(Game::GuiConfFileName);
  lXLen->setTextSize(tsize);
  lXLen->setText(L"XLen");
  lXLen->setPosition(lpos, 20 + (vind + vheight) * p);  lXLen->setSize(llen, vheight);
  vXLen->setPosition(vpos, 20 + (vind + vheight) * p);  vXLen->setSize(vlen, vheight);
  vXLen->setTextSize(tsize);
  lXLen->hide();
  vXLen->hide();
  p++;

  lYLen->load(Game::GuiConfFileName); vYLen->load(Game::GuiConfFileName);
  lYLen->setTextSize(tsize);
  lYLen->setText(L"YLen");
  lYLen->setPosition(lpos, 20 + (vind + vheight) * p);  lYLen->setSize(llen, vheight);
  vYLen->setPosition(vpos, 20 + (vind + vheight) * p);  vYLen->setSize(vlen, vheight);
  vYLen->setTextSize(tsize);
  lYLen->hide();
  vYLen->hide();
  p++;

  lDensity->load(Game::GuiConfFileName); vDensity->load(Game::GuiConfFileName);
  lDensity->setTextSize(tsize);
  lDensity->setText(L"Density");
  lDensity->setPosition(lpos, 20 + (vind + vheight) * p);  lDensity->setSize(llen, vheight);
  vDensity->setPosition(vpos, 20 + (vind + vheight) * p);  vDensity->setSize(vlen, vheight);
  vDensity->setTextSize(tsize);
  p++;

  lInitWeight->load(Game::GuiConfFileName); vInitWeight->load(Game::GuiConfFileName);
  lInitWeight->setTextSize(tsize);
  lInitWeight->setText(L"InitWeight");
  lInitWeight->setPosition(lpos, 20 + (vind + vheight) * p);  lInitWeight->setSize(llen, vheight);
  vInitWeight->setPosition(vpos, 20 + (vind + vheight) * p);  vInitWeight->setSize(vlen, vheight);
  vInitWeight->setTextSize(tsize);
  p++;


  lFriction->load(Game::GuiConfFileName); vFriction->load(Game::GuiConfFileName);
  lFriction->setTextSize(tsize);
  lFriction->setText(L"Friction");
  lFriction->setPosition(lpos, 20 + (vind + vheight) * p);  lFriction->setSize(llen, vheight);
  vFriction->setPosition(vpos, 20 + (vind + vheight) * p);  vFriction->setSize(vlen, vheight);
  vFriction->setTextSize(tsize);
  p++;

  lExtVolume->load(Game::GuiConfFileName); vExtVolume->load(Game::GuiConfFileName);
  lExtVolume->setTextSize(tsize);
  lExtVolume->setText(L"ExtVolume");
  lExtVolume->setPosition(lpos, 20 + (vind + vheight) * p);  lExtVolume->setSize(llen, vheight);
  vExtVolume->setPosition(vpos, 20 + (vind + vheight) * p);  vExtVolume->setSize(vlen, vheight);
  vExtVolume->setTextSize(tsize);
  p++;

  lContCapacity->load(Game::GuiConfFileName); vContCapacity->load(Game::GuiConfFileName);
  lContCapacity->setTextSize(tsize);
  lContCapacity->setText(L"ContCapacity");
  lContCapacity->setPosition(lpos, 20 + (vind + vheight) * p);  lContCapacity->setSize(llen, vheight);
  vContCapacity->setPosition(vpos, 20 + (vind + vheight) * p);  vContCapacity->setSize(vlen, vheight);
  vContCapacity->setTextSize(tsize);
  p++;

  lb2Type->load(Game::GuiConfFileName); vb2Type->load(Game::GuiConfFileName);
  lb2Type->setTextSize(tsize);
  lb2Type->setText(L"b2Type");
  lb2Type->setPosition(lpos, 20 + (vind + vheight) * p);  lb2Type->setSize(llen, vheight);
  vb2Type->setPosition(vpos, 20 + (vind + vheight) * p);  vb2Type->setSize(vlen, vheight);
  vb2Type->addItem("static");  vb2Type->addItem("kinematic"); vb2Type->addItem("dynamic");
  vb2Type->setSelectedItem(0);
  p++;

  lSprite->load(Game::GuiConfFileName); vSprite->load(Game::GuiConfFileName);
  lSprite->setTextSize(tsize);
  lSprite->setText(L"Sprite");
  lSprite->setPosition(lpos, 20 + (vind + vheight) * p);  lSprite->setSize(llen, vheight);
  vSprite->setPosition(vpos, 20 + (vind + vheight) * p);  vSprite->setSize(vlen, vheight);
  ss.clear();
  Game::ObjectSpriteContainer->GetSpritesNames(ss);
  for(const std::string& s : ss) {
    vSprite->addItem(s);
  }
  p++;


  lMaterial->load(Game::GuiConfFileName); vMaterial->load(Game::GuiConfFileName);
  lMaterial->setTextSize(tsize);
  lMaterial->setText(L"Material");
  lMaterial->setPosition(lpos, 20 + (vind + vheight) * p);  lMaterial->setSize(llen, vheight);
  vMaterial->setPosition(vpos, 20 + (vind + vheight) * p);  vMaterial->setSize(vlen, vheight);
  for(auto s : Game::LoadedMaterials) {
    vMaterial->addItem(s.first);
  }
  p++;

  lObjType->load(Game::GuiConfFileName); vObjType->load(Game::GuiConfFileName);
  lObjType->setTextSize(tsize);
  lObjType->setText(L"ObjType");
  lObjType->setPosition(lpos, 20 + (vind + vheight) * p);  lObjType->setSize(llen, vheight);
  vObjType->setPosition(vpos, 20 + (vind + vheight) * p);  vObjType->setSize(vlen, vheight);
  for(int i = 0; i < APhysicObjectData::OT_Count; i++) {
    vObjType->addItem(APhysicObjectData::ObjectTypeName[i]);
  }
  p++;

  lWeaponType->load(Game::GuiConfFileName); vWeaponType->load(Game::GuiConfFileName);
  lWeaponType->setTextSize(tsize);
  lWeaponType->setText(L"WeaponType");
  lWeaponType->setPosition(lpos, 20 + (vind + vheight) * p);  lWeaponType->setSize(llen, vheight);
  vWeaponType->setPosition(vpos, 20 + (vind + vheight) * p);  vWeaponType->setSize(vlen, vheight);
  for(int i = 0; i < APhysicObjectData::WT_Count; i++) {
    vWeaponType->addItem(APhysicObjectData::WeaponTypeName[i]);
  }
  lWeaponType->hide(); vWeaponType->hide();


  lArmorType->load(Game::GuiConfFileName); vArmorType->load(Game::GuiConfFileName);
  lArmorType->setTextSize(tsize);
  lArmorType->setText(L"ArmorType");
  lArmorType->setPosition(lpos, 20 + (vind + vheight) * p);  lArmorType->setSize(llen, vheight);
  vArmorType->setPosition(vpos, 20 + (vind + vheight) * p);  vArmorType->setSize(vlen, vheight);
  for(int i = 0; i < APhysicObjectData::AT_Count; i++) {
    vArmorType->addItem(APhysicObjectData::ArmorTypeName[i]);
  }
  lArmorType->hide(); vArmorType->hide();
  p++;


  lProtection->load(Game::GuiConfFileName); vProtection->load(Game::GuiConfFileName);
  lProtection->setTextSize(tsize);
  lProtection->setText(L"Protection");
  lProtection->setPosition(lpos, 20 + (vind + vheight) * p);  lProtection->setSize(llen, vheight);
  vProtection->setPosition(vpos, 20 + (vind + vheight) * p);  vProtection->setSize(vlen, vheight);
  vProtection->setTextSize(tsize);
  lProtection->hide(); vProtection->hide();
  p++;


  bCreate->load(Game::GuiConfFileName); bSave->load(Game::GuiConfFileName);
  bCreate->setTextSize(bsize);
  bCreate->setText(L"create");
  bCreate->setPosition(lpos, 20 + (vind + vheight) * p);  bCreate->setSize(llen, vheight);
  bSave->setPosition(vpos, 20 + (vind + vheight) * p);  bSave->setSize(vlen, vheight);
  bSave->setTextSize(bsize);
  bSave->setText("save");
  p++;

}
