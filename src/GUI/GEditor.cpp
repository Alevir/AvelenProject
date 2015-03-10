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

#include "GEditor.h"
#include "AWorldSFML.h"
#include "ATranslationReader.h"
#include <boost/lexical_cast.hpp>
#include "GContainerSystem.h"
#include <sstream>
#include "Common.h"
#include "Global.h"
#include "ALocationBase.h"
#include "GScriptEditor.hpp"
using namespace std;






GObjectCreationWindow::GObjectCreationWindow(GGUI &gui, AWorldBase* world)
    : GuiWindow(gui),  mWorld(world), templateName(*mWindow), rSingle(*mWindow), rLine(*mWindow), rArea(*mWindow) {
  templateName->load(Game::GuiConfFileName);
  templateName->setSize(200, 20);
  templateName->setPosition(20, 20);

  rSingle->load(Game::GuiConfFileName);
  rSingle->setPosition(20, 70);
  rSingle->setSize(30, 30);
  rSingle->setText(L"Single");
  rSingle->check();
  rLine->load(Game::GuiConfFileName);
  rLine->setPosition(20, 110);
  rLine->setSize(30, 30);
  rLine->setText(L"Line");
  rArea->load(Game::GuiConfFileName);
  rArea->setPosition(20, 150);
  rArea->setSize(30, 30);
  rArea->setText(L"Area");

  std::set<std::string> names;
  Game::ObjectTemplatesContainer->GetAllTemplateNames(names);
  for(std::string str : names) {
    templateName->addItem(str);
  }
  templateName->setSelectedItem(0);
}

void GObjectCreationWindow::Begin() {
  if(mInProcess) return;
  initPos = AWorldSFML::ConvertMouseCurrentPos();
  if(rSingle->isChecked()) {
    ObjectInitArgs args;
    args.x = initPos.x;
    args.y = initPos.y;
    mWorld->FindLocationByCoords(initPos.x, initPos.y)->AddObject(templateName->getSelectedItem(), args);
    mInProcess = false;
    return;
  } else {
    return;
  }
  mInProcess = true;
}

void GObjectCreationWindow::End() {
  if(!mInProcess) return;
  /*if(rLine->isChecked()) {
  }*/
  mInProcess = false;

}



GDropDownMenu::GDropDownMenu(GGUI& gui, AWorldBase* world)
    :  _world(world), _gui(gui), lb(_gui.Gui)
{
  lb->load(Game::GuiConfFileName);
  lb->hide();
  lb->setSize(200, 400);
  lb->setPosition(400, 400);
  lb->bindCallback(&GDropDownMenu::onClick, this, tgui::ListBox::ItemSelected);
}

void GDropDownMenu::AddItem(const sf::String &itemName) {
  lb->addItem(itemName);
}


void GDropDownMenu::onClick() {
  _activateItem(lb->getSelectedItem());
  lb->deselectItem();
}


void GRightClickMenu::_activateItem(const sf::String &itemName) {
  if(itemName == "Add Object") {
    ObjCreation.Gui.Show(&ObjCreation);
  }
}

bool GRightClickMenu::IsCreationMode() {
  return ObjCreation.IsVisible();
}


void GDropDownMenu::Show() {
  _pos = sf::Mouse::getPosition(*Game::Window);
  lb->setPosition(_pos.x, _pos.y);
  lb->show();
}

void GDropDownMenu::Hide() {
  lb->hide();
}



GObjectInfoWindow::GObjectInfoWindow(GInfoManager<APhysicObjectBase, GObjectInfoWindow>* manager, APhysicObjectBase *obj)
    : _manager(manager), GuiWindow(manager->_gui), _obj(obj), lTemplateName(*mWindow), vTemplateName(*mWindow),
  lObjectType(*mWindow), vObjectType(*mWindow),
  lObjectSubType(*mWindow), vObjectSubType(*mWindow),
  lWeight(*mWindow), vWeight(*mWindow) {
  mWindow->setSize(600, 400);
  auto v = sf::Mouse::getPosition(*Game::Window);
  mWindow->setPosition(v.x, v.y);

  mWindow->show();
  const int initPosX = 20;
  const int initPosY = 20;
  const int vertIndent = 20;
  const int labelHeight = 30;
  const int labelLenght = 200;
  const int horIndent = 20;
  const int valLenght = 340;
  int p = 0;
  //_window->bindCallback(&GObjectInfoWindow::_onClose, this, tgui::ChildWindow::Closed);

  ATranslationReader rd(Game::TranslationsPath + Game::Language + "/common");
  ATranslationReader objNames(Game::TranslationsPath + Game::Language + "/objectNames");
  /*lTemplateName->load(paths::GuiConfFileName);
  vTemplateName->load(paths::GuiConfFileName);
  lObjectType->load(paths::GuiConfFileName);
  vObjectType->load(paths::GuiConfFileName);*/
  lTemplateName->setText("Template");
  lTemplateName->setSize(labelLenght, labelHeight);
  lTemplateName->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));
  vTemplateName->setText(_obj->GetTemplateName());
  vTemplateName->setSize(valLenght, labelHeight);
  vTemplateName->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
  p++;

  lObjectType->setText("Type");
  lObjectType->setSize(labelLenght, labelHeight);
  lObjectType->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));
  vObjectType->setText(rd.GetTranslation(APhysicObjectData::ObjectTypeName[_obj->GetTemplateData()->objectType]));
  vObjectType->setSize(valLenght, labelHeight);
  vObjectType->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
  p++;

  if(_obj->GetTemplateData()->objectType == APhysicObjectData::OT_Armor
  || _obj->GetTemplateData()->objectType == APhysicObjectData::OT_Weapon) {
    if(_obj->GetTemplateData()->objectType == APhysicObjectData::OT_Armor) {
      lObjectSubType->setText("Armor Type");
      vObjectSubType->setText(rd.GetTranslation(APhysicObjectData::ArmorTypeName[_obj->GetTemplateData()->armorType]));
    } else {
      lObjectSubType->setText("Weapon Type");
      vObjectSubType->setText(rd.GetTranslation(APhysicObjectData::WeaponTypeName[_obj->GetTemplateData()->weaponType]));
    }
    lObjectSubType->setSize(labelLenght, labelHeight);
    lObjectSubType->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));
    vObjectSubType->setSize(valLenght, labelHeight);
    vObjectSubType->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
    p++;
  }
  lWeight->setText("Weight");
  lWeight->setSize(labelLenght, labelHeight);
  lWeight->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));
  vWeight->setText(boost::lexical_cast<std::string>(_obj->GetWeight()));
  vWeight->setSize(valLenght, labelHeight);
  vWeight->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
}



GCharacterInfoWindow::GCharacterInfoWindow(GInfoManager<ACharacterBase, GCharacterInfoWindow>* manager, ACharacterBase *obj)
    : _manager(manager), GuiWindow(manager->_gui), _obj(obj), lInfo(*mWindow) {
  mWindow->setSize(600, 400);
  auto v = sf::Mouse::getPosition(*Game::Window);
  mWindow->setPosition(v.x, v.y);


  mWindow->show();
  const int initPosX = 20;
  const int initPosY = 20;
  lInfo->load(Game::GuiConfFileName);
  lInfo->setPosition(initPosX, initPosY);
  lInfo->setSize(500, 350);
  std::stringstream info;
  info << "Name:\t" << obj->GetTemplateName() << "\n" << "Race:\t" << obj->GetRace()
  << '\n' << "XP:\t" << obj->GetXP() << '\n' << "HP:\t" << obj->GetHP() << '/' << obj->GetMaxHP() << '\n'
  << "EP:\t" << obj->GetEP() << '/' << obj->GetMaxEP() << '\n' << "Strength:\t" << obj->GetStr()
  << '\n' << "Accuracy:\t" << obj->GetAcc() << '\n' << "Regeneration:\t"
  << obj->GetReg();
  lInfo->setText(info.rdbuf()->str());
  lInfo->setTextSize(30);
  lInfo->setReadOnly();
}


void GObjectInfoWindow::_onClose() {
  _manager->_delete(_obj);
  GuiWindow::_onClose();
}

void GCharacterInfoWindow::_onClose() {
  _manager->_delete(_obj);
  GuiWindow::_onClose();
}



void GLocationCreation::_onCreate() {
  using namespace libconfig;
  Config loc;
  Setting& s = loc.getRoot();
  vector<string> _nameMap;
  _nameMap.push_back("GC1");
  int x, y;
  x = boost::lexical_cast<int> (std::string(vXSize->getText()));
  y = boost::lexical_cast<int> (std::string(vYSize->getText()));

  s.add("XLen", libconfig::Setting::TypeInt) = x;
  s.add("YLen", libconfig::Setting::TypeInt) = y;
  Setting& nlist = s.add("nameMap", Setting::TypeList);
  for(int i = 0; i < _nameMap.size(); i++) {
    Setting& point = nlist.add(Setting::TypeGroup);
    point.add("name", Setting::TypeString) = _nameMap[i];
    point.add("id", Setting::TypeInt) = i;
  }

  stringstream sstr;
  for(int i = 0; i < y; i++) {
    for(int j = 0; j < x; j++) {
      sstr << 0 << ' ';
    }
  }
  string _map = sstr.rdbuf()->str();
  s.add("map", libconfig::Setting::TypeString) = _map;
  s.add("objects", libconfig::Setting::TypeList);

  Setting& chList = s.add("characters", libconfig::Setting::TypeList);
  Setting& group = chList.add(Setting::TypeGroup);
  group.add("templateName", Setting::TypeString) = string("Player");
  group.add("health", Setting::TypeFloat) = 100.0;
  group.add("controller", Setting::TypeInt) = 1;
  group.add("combatModel", Setting::TypeInt) = 0;
  group.add("x", Setting::TypeFloat) = 2.0;
  group.add("y", Setting::TypeFloat) = 2.0;
  group.add("angle", Setting::TypeFloat) = 0.0;
  group.add("z", Setting::TypeFloat) = 1.7;
  group.add("container", libconfig::Setting::TypeList);
  for(unsigned int i = 0; i < InventorySize; i++) {
    group.add(SlotName[i].c_str(), Setting::TypeGroup);
  }

/*
  loc.writeFile("../resources/locations/testGen.cfg");
  _world->ResetCurrentLocation(loc);
  */
  Hide();
}


GLocationCreation::GLocationCreation(GGUI &gui, AWorldBase *world)
    : GuiWindow(gui), _world(world),
    lNewLocName(*mWindow), vNewLocName(*mWindow),
    lXSize(*mWindow), vXSize(*mWindow),
    lYSize(*mWindow), vYSize(*mWindow),
    bCreate(*mWindow)   {
  mWindow->setSize(600, 300);
  auto v = Game::Window->getSize();
  mWindow->setPosition((v.x - 600) / 2, (v.y - 300) / 2 );
  const int initPosX = 20;
  const int initPosY = 20;
  const int vertIndent = 20;
  const int labelHeight = 30;
  const int labelLenght = 270;
  const int horIndent = 20;
  const int valLenght = 270;
  int p = 0;

  vNewLocName->load(Game::GuiConfFileName);
  vXSize->load(Game::GuiConfFileName);
  vYSize->load(Game::GuiConfFileName);
  bCreate->load(Game::GuiConfFileName);

  bCreate->bindCallback(&GLocationCreation::_onCreate, this, tgui::Button::LeftMouseClicked);

  lNewLocName->setText("New location's name");
  lNewLocName->setSize(labelLenght, labelHeight);
  lNewLocName->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));

  vNewLocName->setSize(valLenght, labelHeight);
  vNewLocName->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
  vNewLocName->setText("testGen");
  p++;

  lXSize->setText("Location x size");
  lXSize->setSize(labelLenght, labelHeight);
  lXSize->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));

  vXSize->setSize(valLenght, labelHeight);
  vXSize->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
  vXSize->setText("10");
  p++;

  lYSize->setText("Location y size");
  lYSize->setSize(labelLenght, labelHeight);
  lYSize->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));

  vYSize->setSize(valLenght, labelHeight);
  vYSize->setPosition(initPosX + labelLenght + horIndent, initPosY + p * (labelHeight + vertIndent));
  vYSize->setText("10");
  p++;


  bCreate->setText("Create!");
  bCreate->setSize(labelLenght + horIndent + valLenght, labelHeight * 3);
  bCreate->setPosition(initPosX, initPosY + p * (labelHeight + vertIndent));

}


GRightClickMenu::GRightClickMenu(GGUI &gui, AWorldBase *world)
    : GDropDownMenu(gui, world), ObjCreation(gui, world) {
  AddItem("Add Object");
}


void GRightClickObjectMenu::_activateItem(const sf::String &itemName) {
  if(itemName == "Show info") {
    if(_obj->GetTemplateData()->objectType == APhysicObjectData::OT_Character) {
      _CIM->ShowObjectInfo(dynamic_cast<ACharacterBase*>(_obj));
    } else {
      _IM->ShowObjectInfo(_obj);
    }
    Hide();
  } else
  if(itemName == "Show container") {
    _CS->AddContainer(_obj);
    Hide();
  } else
  if(itemName == "Remove object") {
    _obj->GetLocation()->RemoveAObject(_obj);
    Hide();
  } else
  if(itemName == "Edit scripts") {
    ScriptEditor->SetObject(_obj);
    ScriptEditor->Gui.Show(ScriptEditor);
  }

}

GRightClickObjectMenu::GRightClickObjectMenu(GGUI &gui, AWorldBase *world, GContainerSystem* CS, GInfoManager<APhysicObjectBase, GObjectInfoWindow>* IM, GInfoManager<ACharacterBase, GCharacterInfoWindow> *CIM)
    : GDropDownMenu(gui, world), _CS(CS), _IM(IM), _CIM(CIM) {
  AddItem("Show info");
  AddItem("Show container");
  AddItem("Remove object");
  AddItem("Edit scripts");
}


void RaceEditor::selectRace() {
  const CharacterRace& r = Game::CharacterRaces.find(races->getSelectedItem())->second;
  vAcc->setText(boost::lexical_cast<std::string>(r.Acc));
  vStr->setText(boost::lexical_cast<std::string>(r.Str));
  vReg->setText(boost::lexical_cast<std::string>(r.Reg));
  vAccG->setText(boost::lexical_cast<std::string>(r.AccGain));
  vStrG->setText(boost::lexical_cast<std::string>(r.StrGain));
  vRegG->setText(boost::lexical_cast<std::string>(r.RegGain));
}

RaceEditor::RaceEditor(GGUI &gui) : GuiWindow(gui),
lStr(*mWindow), lAcc(*mWindow), lReg(*mWindow), lStrG(*mWindow), lAccG(*mWindow), lRegG(*mWindow),
vStr(*mWindow), vAcc(*mWindow), vReg(*mWindow), vStrG(*mWindow), vAccG(*mWindow), vRegG(*mWindow),
races(*mWindow) {
  mWindow->setSize(600, 400);
  mWindow->setPosition(200, 200);
  int p = 0;
  int lLen = 100;
  int vLen = 250;
  int lPosx = 210;
  int vPosx = 330;
  int vind = 10;
  int vheight = 25;
  races->load(Game::GuiConfFileName);
  races->setPosition(20, 20);
  races->setSize(170, 360);
  vAcc->load(Game::GuiConfFileName);
  vStr->load(Game::GuiConfFileName);
  vReg->load(Game::GuiConfFileName);
  vAccG->load(Game::GuiConfFileName);
  vStrG->load(Game::GuiConfFileName);
  vRegG->load(Game::GuiConfFileName);
  races->bindCallback(&RaceEditor::selectRace, this, tgui::ListBox::ItemSelected);



  lStr->setPosition(lPosx, 20 + (vheight + vind) * p);
  lStr->setSize(lLen, vheight);
  lStr->setText(L"Str");
  lStr->setTextSize(25);
  vStr->setPosition(vPosx, 20 + (vheight + vind) * p);
  vStr->setSize(vLen, vheight);
  p++;

  lAcc->setPosition(lPosx, 20 + (vheight + vind) * p);
  lAcc->setSize(lLen, vheight);
  lAcc->setText(L"Acc");
  lAcc->setTextSize(25);
  vAcc->setPosition(vPosx, 20 + (vheight + vind) * p);
  vAcc->setSize(vLen, vheight);
  p++;

  lReg->setPosition(lPosx, 20 + (vheight + vind) * p);
  lReg->setSize(lLen, vheight);
  lReg->setText(L"Reg");
  lReg->setTextSize(25);
  vReg->setPosition(vPosx, 20 + (vheight + vind) * p);
  vReg->setSize(vLen, vheight);
  p++;



  lStrG->setPosition(lPosx, 20 + (vheight + vind) * p);
  lStrG->setSize(lLen, vheight);
  lStrG->setText(L"StrGain");
  lStrG->setTextSize(25);
  vStrG->setPosition(vPosx, 20 + (vheight + vind) * p);
  vStrG->setSize(vLen, vheight);
  p++;

  lAccG->setPosition(lPosx, 20 + (vheight + vind) * p);
  lAccG->setSize(lLen, vheight);
  lAccG->setText(L"AccGain");
  lAccG->setTextSize(25);
  vAccG->setPosition(vPosx, 20 + (vheight + vind) * p);
  vAccG->setSize(vLen, vheight);
  p++;

  lRegG->setPosition(lPosx, 20 + (vheight + vind) * p);
  lRegG->setSize(lLen, vheight);
  lRegG->setText(L"RegGain");
  lRegG->setTextSize(25);
  vRegG->setPosition(vPosx, 20 + (vheight + vind) * p);
  vRegG->setSize(vLen, vheight);
  p++;

  Refresh();

}

void RaceEditor::Refresh() {
  races->removeAllItems();
  for(auto p : Game::CharacterRaces) {
    races->addItem(p.second.name);
  }
}





