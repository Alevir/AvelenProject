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

#ifndef GINGAME_HPP
#define GINGAME_HPP
#include "GUI/GCommon.h"
#include "ACharacterBase.h"
#include "ATranslationReader.h"


class GCharacterInfo : public GuiWindowReusable {
  tgui::TextBox::Ptr mInfoName;
  tgui::TextBox::Ptr mInfoValue;
  ACharacterBase* mChar;
  ACharacterBase::Info mInfo;

public:
  GCharacterInfo(GGUI& gui, ACharacterBase* ch);
  void Refresh();

};


class GPlayerInterface {
  ACharacterBase* mPlayer;
  sf::RectangleShape HPState;
  sf::RectangleShape EPState;
  sf::RectangleShape HPStateFrame;
  sf::RectangleShape EPStateFrame;
  sf::Color attPosInd[3];

  GGUI& mGui;
  tgui::Label::Ptr focusedOn;

  class EffectBar {
    std::map<const std::string, tgui::Picture::Ptr> _icons;
    tgui::Picture::Ptr& _getIcon(const Effect* ef);
    GPlayerInterface* mPlI;
  public:
    EffectBar(GPlayerInterface* p);
    void Draw();
  };

  class MessageBar {
    GPlayerInterface* mp;
    tgui::Label::Ptr lMessage;
    double time = 0.0;
  public:
    void AddMessage(const std::wstring& m, double duration);
    MessageBar(GPlayerInterface* p) : mp(p), lMessage(p->mGui.Gui) {
      lMessage->setTextSize(40);
      lMessage->setAutoSize(true);
    }
    void Step(double dt);
  };

  EffectBar efBar;
  MessageBar mesBar;

public:
  GCharacterInfo Info;
  GPlayerInterface(ACharacterBase* player, GGUI& gui);
  void SetFocusedObject(APhysicObjectBase* obj);
  void Step(double dt);
  void Draw();
  void AddMessage(const std::wstring& m, double duration);
  void SetPlayer(ACharacterBase* player) { mPlayer = player; }
};

#endif // GINGAME_HPP

