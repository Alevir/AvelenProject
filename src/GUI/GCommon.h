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

#ifndef GCOMMON_H
#define GCOMMON_H
#include "TGUI/TGUI.hpp"
#include "AVector2.h"
#include <set>
#include "AObject.h"
class AWorldBase;
class GuiWindowBase;
class WorldMode;


class GGUI {
protected:
  std::set<GuiWindowBase*> mWin;
  AWorldBase* mWorld = 0;

public:
  enum GuiMode {
    GM_InGame,
    GN_Editor
  };

  GuiMode mGM = GM_InGame;
  tgui::Gui& Gui;
  GGUI(tgui::Gui& gui, AWorldBase* world = 0);
  void Show(GuiWindowBase* w);
  bool OnHideOrClose(GuiWindowBase* w); //return value: is w the last window
  void SetWorld(AWorldBase* world) { mWorld = world; }
  void HandleEvent(sf::Event& event);
};


class GuiWindowBase : public RemovableObject {
  friend class GGUI;
protected:
  virtual void Show() = 0;
public:
  GGUI& Gui;
  virtual void Hide() = 0;
  GuiWindowBase(GGUI& gui) : Gui(gui) {}

};

class GuiWindow : public GuiWindowBase {
protected:
  AVector2 _initPos;
  tgui::ChildWindow::Ptr mWindow;
  virtual void _onClose();
  void Show();
public:
  GuiWindow(GGUI& gui);

  void Hide();
  void SwitchVisibility();
  bool IsVisible() { return mWindow->isVisible(); }
  virtual ~GuiWindow() {}
};



class GMessageManager;

class GMessageWindow : public GuiWindow {
  tgui::Label::Ptr mes;
  tgui::Button::Ptr bOK;
  void _onClose();
  GMessageManager* _mm;

public:
  ~GMessageWindow();
  GMessageWindow(GGUI& gui, const std::wstring& message, GMessageManager* mm);
};


class GMessageManager {
  friend class GMessageWindow;
  std::set<GMessageWindow*> _ms;
  void RemoveMessage(GMessageWindow*mw);
  GGUI& _gui;

public:
  GMessageManager(GGUI& gui);
  void AddMessage(const std::wstring& message);
};

class GSaveWindow : public GuiWindow {
  AWorldBase* mWorld;
  tgui::ListBox::Ptr saves;
  tgui::EditBox::Ptr saveName;
  tgui::Button::Ptr bSave;
  tgui::ChildWindow::Ptr overwriteW;
  tgui::Label::Ptr overwriteMessage;
  tgui::Button::Ptr bOverwrite;
  void save();
  void overwriteClose();
  void overwriteSave();
  void saveSelected();
  void _onClose();
public:
  GSaveWindow(GGUI& gui, AWorldBase* world);
};

class GLoadWindow : public GuiWindow {
  AWorldBase* mWorld;
  tgui::ListBox::Ptr loadList;
  tgui::Button::Ptr bLoad;
  WorldMode* mWM;
  void load();
public:
  GLoadWindow(GGUI& gui, AWorldBase* world, WorldMode* wm);
};

#endif // GCOMMON_H
