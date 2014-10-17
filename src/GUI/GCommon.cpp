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
#include "GCommon.h"
#include "Global.h"
#include "AWorldBase.h"
#include "ACharacterBase.h"
#include "Common.h"
#include "Mode.h"

void GuiWindow::_onClose() {
  mWindow->hide();
  Gui.OnHideOrClose(this);
}

GuiWindow::GuiWindow(GGUI& gui) : GuiWindowBase(gui), mWindow(gui.Gui) {
mWindow->hide();
mWindow->bindCallback(&GuiWindow::_onClose, this, tgui::ChildWindow::Closed);
  mWindow->load(Game::GuiConfFileName);
  mWindow->setPosition(200, 200);
  mWindow->setSize(400, 400);
  _initPos = sf::Mouse::getPosition(*Game::Window);
}

void GuiWindow::Show() {
  mWindow->show();
}

void GuiWindow::Hide() {
  mWindow->hide();
  Gui.OnHideOrClose(this);
}

void GuiWindow::SwitchVisibility() {
if(mWindow->isVisible()) mWindow->hide();
  else mWindow->show();
}



void GMessageWindow::_onClose() {
  _mm->RemoveMessage(this);
}

GMessageWindow::~GMessageWindow() {
  mWindow->destroy();
}



GMessageWindow::GMessageWindow(GGUI& gui, const std::wstring &message, GMessageManager *mm) : GuiWindow(gui), mes(*mWindow), bOK(*mWindow), _mm(mm) {
  mes->setText(message);
  mes->setPosition(0, 0);
  mes->setTextSize(30);
  mes->setSize(mWindow->getSize().x, mWindow->getSize().y - 60 - mWindow->getTitleBarHeight());
  bOK->setSize(80, 40);
  bOK->setPosition(mWindow->getSize().x / 2 - 40, mWindow->getSize().y - 50);
  bOK->bindCallback(&GMessageWindow::_onClose, this, tgui::Button::LeftMouseClicked);
  bOK->setText("OK");
  bOK->setTextSize(30);
}


void GMessageManager::RemoveMessage(GMessageWindow * mw) {
  auto it = _ms.find(mw);
  if(it == _ms.end()) return;
  delete *it;
  _ms.erase(it);
}

GMessageManager::GMessageManager(GGUI &gui) : _gui(gui) {

}

void GMessageManager::AddMessage(const std::wstring &message) {
  GMessageWindow* mes = new GMessageWindow(_gui, message, this);
  _ms.insert(mes);
}


void GSaveWindow::save() {
  std::string name = saveName->getText();
  if(!name.empty()) {
    bool overwrite = false;
    std::vector<std::string> v;
    GetAllFilenamesInDir("../testSave/", v);
    for(std::string& s : v) {
      if(name == s) {
        overwrite = true;
        break;
      }
    }
    if(overwrite) {
      overwriteW->show();
    } else {
      mWorld->Save("../testSave/" + name);
      _onClose();
    }
  } else {
    _onClose();
  }
}


void GSaveWindow::overwriteClose() {
  overwriteW->hide();
}

void GSaveWindow::overwriteSave() {
  mWorld->Save("../testSave/" + saveName->getText());
  _onClose();
}

void GSaveWindow::saveSelected() {
saveName->setText(saves->getSelectedItem());
}

void GSaveWindow::_onClose() {
  overwriteW->hide();
  GuiWindow::_onClose();
}


GSaveWindow::GSaveWindow(GGUI& gui, AWorldBase* world)
: GuiWindow(gui), mWorld(world), saves(*mWindow), saveName(*mWindow), bSave(*mWindow),
overwriteW(*mWindow), overwriteMessage(*overwriteW), bOverwrite(*overwriteW) {
  const int wl = 400;
  const int wh = 500;
  saveName->load(Game::GuiConfFileName);
  bSave->load(Game::GuiConfFileName);
  saves->load(Game::GuiConfFileName);
  overwriteW->load(Game::GuiConfFileName);
  overwriteMessage->load(Game::GuiConfFileName);
  bOverwrite->load(Game::GuiConfFileName);
  mWindow->setSize(wl, wh);
  saveName->setPosition(20, 20);
  const int snlen = 250;
  saveName->setSize(snlen, 30);
  bSave->setPosition(40 + snlen, 20);
  bSave->setSize(wl - 60 - snlen, 30);
  bSave->setText(L"Save");
  saves->setPosition(20, 70);
  saves->setSize(wl - 40, wh - 90);
  bSave->bindCallback(&GSaveWindow::save, this, tgui::Button::LeftMouseClicked);
  overwriteW->bindCallback(&GSaveWindow::overwriteClose, this, tgui::ChildWindow::Closed);
  bOverwrite->bindCallback(&GSaveWindow::overwriteSave, this, tgui::Button::LeftMouseClicked);
  saves->bindCallback(&GSaveWindow::saveSelected, this, tgui::ListBox::ItemSelected);
  const int owl = 200;
  const int owh = 150;
  overwriteW->setSize(owl, owh);
  overwriteW->setPosition((wl - owl) / 2, (wh - owh) / 2);
  bOverwrite->setSize(owl - 40, 30);
  bOverwrite->setPosition(20, owh - 50);
  bOverwrite->setText(L"Yes");
  overwriteMessage->setText("Overwrite?");
  overwriteMessage->setPosition(20, 20);
  overwriteMessage->setSize(owl - 40, owh - 70);
  overwriteW->hide();



  std::vector<std::string> v;
  GetAllFilenamesInDir("../testSave/", v);
  const std::string e(".save");
  for(std::string& s : v) {
    bool isSave = true;
    for(int i = 1; i <= e.size(); ++i) {
      if(s[s.size() - i] != e[e.size() - i]) {
        isSave = false;
        break;
      }
    }
    if(isSave) {
      saves->addItem(s);
    }
  }
}


void GLoadWindow::load() {
  mWorld->Reload("../testSave/" + loadList->getSelectedItem());
  mWM->ResetPlayer();
  _onClose();
}

GLoadWindow::GLoadWindow(GGUI& gui, AWorldBase* world, WorldMode* wm)
: GuiWindow(gui), mWorld(world), loadList(*mWindow), bLoad(*mWindow), mWM(wm) {
  const int wl = 400;
  const int wh = 500;
  mWindow->setSize(wl, wh);
  loadList->load(Game::GuiConfFileName);
  loadList->setPosition(20, 20);
  loadList->setSize(wl - 40, wh - 140);
  bLoad->load(Game::GuiConfFileName);
  bLoad->setPosition(20, wh - 120);
  bLoad->setSize(wl - 40, 100);
  bLoad->setText(L"Load");
  bLoad->bindCallback(&GLoadWindow::load, this, tgui::Button::LeftMouseClicked);
  std::vector<std::string> v;
  GetAllFilenamesInDir("../testSave/", v);
  const std::string e(".save");
  for(std::string& s : v) {
    bool isSave = true;
    for(int i = 1; i <= e.size(); ++i) {
      if(s[s.size() - i] != e[e.size() - i]) {
        isSave = false;
        break;
      }
    }
    if(isSave) {
      loadList->addItem(s);
    }
  }

}


GGUI::GGUI(tgui::Gui& gui, AWorldBase* world) : Gui(gui), mWorld(world) {

}

void GGUI::Show(GuiWindowBase* w) {
  if(mGM == GM_InGame && mWin.empty() && mWorld) {
    mWorld->GetPlayer()->SwitchController();
    Game::Window->setMouseCursorVisible(true);
  }
  mWin.insert(w);
  w->Show();
}

bool GGUI::OnHideOrClose(GuiWindowBase* w) {
  auto it = mWin.find(w);
  if(it == mWin.end()) throw std::logic_error("window not found");
  mWin.erase(it);
  if(mGM == GM_InGame && mWin.empty() && mWorld) {
    mWorld->GetPlayer()->SwitchController();
    Game::Window->setMouseCursorVisible(false);
  }
  return mWin.empty();
}

void GGUI::HandleEvent(sf::Event& event) {
  if(!mWin.empty()) {
    Gui.handleEvent(event);
  }
}
