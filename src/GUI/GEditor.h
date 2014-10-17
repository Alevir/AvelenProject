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

#ifndef GEDITOR_H
#define GEDITOR_H
#include "ADebugOutput.h"

#include "const.h"
#include "SFML/System.hpp"
#include "GObjectTemplateEditor.hpp"
class AWorldBase;
class APhysicObjectBase;
class ACharacterBase;
class EditorMode;
class GContainerSystem;
class GScriptEditor;





class GObjectCreationWindow : public GuiWindow {

  AWorldBase* mWorld;
  tgui::ComboBox::Ptr templateName;
  tgui::RadioButton::Ptr rSingle;
  tgui::RadioButton::Ptr rLine;
  tgui::RadioButton::Ptr rArea;
  AVector2 initPos;
  bool mInProcess = false;



public:
  GObjectCreationWindow(GGUI& gui, AWorldBase* world);
  void Begin();
  void End();
};

template <class type, class infoType>
class GInfoManager;

class GObjectInfoWindow : public GuiWindow {
  friend class GInfoManager<APhysicObjectBase, GObjectInfoWindow>;
  GInfoManager<APhysicObjectBase, GObjectInfoWindow>* _manager;
  APhysicObjectBase* _obj;
  tgui::Label::Ptr lTemplateName;
  tgui::Label::Ptr vTemplateName;
  tgui::Label::Ptr lObjectType;
  tgui::Label::Ptr vObjectType;
  tgui::Label::Ptr lObjectSubType;
  tgui::Label::Ptr vObjectSubType;
  tgui::Label::Ptr lWeight;
  tgui::Label::Ptr vWeight;
  void _onClose();
  GObjectInfoWindow(GInfoManager<APhysicObjectBase, GObjectInfoWindow>* manager, APhysicObjectBase* obj);
};


class GCharacterInfoWindow : public GuiWindow {
  friend class GInfoManager<ACharacterBase, GCharacterInfoWindow>;
  GInfoManager<ACharacterBase, GCharacterInfoWindow>* _manager;
  ACharacterBase* _obj;
  tgui::TextBox::Ptr lInfo;
  void _onClose();
  GCharacterInfoWindow(GInfoManager<ACharacterBase, GCharacterInfoWindow>* manager, ACharacterBase* obj);
};

template <class type, class infoType>
class GInfoManager {
  friend infoType;
  std::map<type*, infoType*> _objInfo;
  AWorldBase* _world;
  GGUI& _gui;

  void _delete(type* obj) {
    auto it = _objInfo.find(obj);
    if(it != _objInfo.end()) {
      delete it->second;
      _objInfo.erase(it);
    }
  }

public:

  GInfoManager(AWorldBase* world, GGUI& gui) : _world(world), _gui(gui) {

  }

  ~GInfoManager() {
    for( std::pair<type*, infoType*> p : _objInfo) {
      delete p.second;
    }
  }

  void ShowObjectInfo(type* obj) {
    infoType* w;
    auto it = _objInfo.find(obj);
    if(it == _objInfo.end()) {
      w = new infoType(this, obj);
      _objInfo.insert(std::make_pair(obj, w));
    }
  }
};


class GDropDownMenu {
protected:
  AWorldBase* _world;
  GGUI& _gui;
  tgui::ListBox::Ptr lb;
  sf::Vector2i _pos;
  void onClick();
  virtual void _activateItem(const sf::String& itemName) = 0;

public:
  GDropDownMenu(GGUI& gui, AWorldBase* world);
  void AddItem(const sf::String& itemName);
  void Show();
  void Hide();
};


class GRightClickMenu : public GDropDownMenu {

  void _activateItem(const sf::String &itemName);
public:
  GObjectCreationWindow ObjCreation;


  bool IsCreationMode();
  GRightClickMenu(GGUI& gui, AWorldBase* world);
};


class GRightClickObjectMenu : public GDropDownMenu {
  void _activateItem(const sf::String &itemName);
  APhysicObjectBase* _obj = 0;
  GContainerSystem* _CS;

  GInfoManager<APhysicObjectBase, GObjectInfoWindow>* _IM;
  GInfoManager<ACharacterBase, GCharacterInfoWindow>* _CIM;

public:
  GScriptEditor* ScriptEditor = 0;
  GRightClickObjectMenu(GGUI& gui, AWorldBase* world, GContainerSystem* CS,
     GInfoManager<APhysicObjectBase, GObjectInfoWindow>*,
     GInfoManager<ACharacterBase, GCharacterInfoWindow>* CIM);
  void SetObject(APhysicObjectBase* obj) { _obj = obj; }
};





class RaceEditor : public GuiWindow {
  tgui::ListBox::Ptr races;
  tgui::Label::Ptr lStr;
  tgui::Label::Ptr lAcc;
  tgui::Label::Ptr lReg;
  tgui::Label::Ptr lStrG;
  tgui::Label::Ptr lAccG;
  tgui::Label::Ptr lRegG;

  tgui::EditBox::Ptr vStr;
  tgui::EditBox::Ptr vAcc;
  tgui::EditBox::Ptr vReg;
  tgui::EditBox::Ptr vStrG;
  tgui::EditBox::Ptr vAccG;
  tgui::EditBox::Ptr vRegG;
  void selectRace();

public:
  RaceEditor(GGUI& gui);
  void Refresh();
};


class GLocationCreation : public GuiWindow {
  AWorldBase* _world;
  tgui::Label::Ptr lNewLocName;
  tgui::EditBox::Ptr vNewLocName;
  tgui::Label::Ptr lXSize;
  tgui::EditBox::Ptr vXSize;
  tgui::Label::Ptr lYSize;
  tgui::EditBox::Ptr vYSize;
  tgui::Button::Ptr bCreate;

  void _onCreate();

public:
  GLocationCreation(GGUI& gui, AWorldBase* world);
};

#endif // GEDITOR_H
