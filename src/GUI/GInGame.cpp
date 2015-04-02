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
#include "GInGame.hpp"
#include "AHumanCombatModel.h"
#include "Character/Effects.hpp"
#include "ACharacter.h"
#include "Global.h"




GPlayerInterface::GPlayerInterface(ACharacter *player, GGUI& gui)
: mGui(gui), focusedOn(mGui.Gui), efBar(this), mesBar(this), Info(gui, player) {
  mPlayer = player;
  HPState.setPosition(Game::Window->getSize().x * 2.0 / 3.0 - 7, 5);
  EPState.setPosition(Game::Window->getSize().x * 2.0 / 3.0 - 7, 45);
  sf::Color c = sf::Color::Red;
  c.a = 200;
  HPState.setFillColor(c);
  c = sf::Color::Yellow;
  c.a = 200;
  EPState.setFillColor(c);

  HPStateFrame.setSize(sf::Vector2f(Game::Window->getSize().x / 3.0, 30));
  HPStateFrame.setPosition(Game::Window->getSize().x * 2.0 / 3.0 - 7, 5);
  HPStateFrame.setFillColor(sf::Color::Transparent);

  EPStateFrame.setSize(sf::Vector2f(Game::Window->getSize().x / 3.0, 30));
  EPStateFrame.setPosition(Game::Window->getSize().x * 2.0 / 3.0 - 7, 45);
  EPStateFrame.setFillColor(sf::Color::Transparent);
  focusedOn->setPosition(Game::Window->getSize().x / 2, Game::Window->getSize().y - 100);
  focusedOn->setSize(200, 50);
  focusedOn->setAutoSize(true);
  focusedOn->setTextSize(40);
  focusedOn->setBackgroundColor(sf::Color::Transparent);
}

void GPlayerInterface::SetFocusedObject(APhysicObjectBase *obj) {
  if(obj)
    focusedOn->setText(obj->GetTemplateName());
  else
    focusedOn->setText("");
}

void GPlayerInterface::Step(double dt) {
  mesBar.Step(dt);
}

void GPlayerInterface::Draw() {
  HPState.setSize(sf::Vector2f(Game::Window->getSize().x / 3.0 * mPlayer->GetHP() / mPlayer->GetMaxHP(), 30));
  HPStateFrame.setOutlineColor(sf::Color::Black);
  HPStateFrame.setOutlineThickness(4);
  Game::Window->draw(HPStateFrame);
  HPStateFrame.setOutlineColor(sf::Color::White);
  HPStateFrame.setOutlineThickness(2);
  Game::Window->draw(HPStateFrame);
  Game::Window->draw(HPState);


  EPState.setSize(sf::Vector2f(Game::Window->getSize().x / 3.0 * mPlayer->GetEP() / mPlayer->GetMaxEP(), 30));
  EPStateFrame.setOutlineColor(sf::Color::Black);
  EPStateFrame.setOutlineThickness(4);
  Game::Window->draw(EPStateFrame);
  EPStateFrame.setOutlineColor(sf::Color::White);
  EPStateFrame.setOutlineThickness(2);
  Game::Window->draw(EPStateFrame);
  Game::Window->draw(EPState);

  const AHumanCombatModel* cm;
  sf::Color col;
  if( (cm = dynamic_cast<const AHumanCombatModel*>(mPlayer->GetCombatModel()))) {
    for(int i = 0; i < 3; i++) {
      col = sf::Color::Yellow;
      col.a = 120;
      attPosInd[i] = col;
    }
    attPosInd[cm->GetAttackPossition()] = sf::Color::Red;
    sf::RectangleShape attInd;
    attInd.setSize(sf::Vector2f(17, 33));
    attInd.setOutlineThickness(2);
    attInd.setOutlineColor(sf::Color::White);
    int ix = Game::Window->getSize().x / 2.0 + 40;
    int iy = Game::Window->getSize().y / 2.0 - 50;
    for(int i = 0; i < 3; i++) {
      attInd.setPosition(ix, iy + 40 * i);
      attInd.setFillColor(attPosInd[2 - i]);
      Game::Window->draw(attInd);
    }
  }
  efBar.Draw();
  focusedOn->setPosition((Game::Window->getSize().x - focusedOn->getSize().x) / 2, focusedOn->getPosition().y);

}

void GPlayerInterface::AddMessage(const std::wstring& m, double duration) {
  mesBar.AddMessage(m, duration);

}


tgui::Picture::Ptr &GPlayerInterface::EffectBar::_getIcon(const Effect *ef) {
  auto it = _icons.begin();
  it = _icons.find(ef->EffectName);
  if(it == _icons.end()) {
    tgui::Picture::Ptr icon(mPlI->mGui.Gui);
    icon->load(std::string("../resources/textures/effects/") + ef->EffectName + ".png");
    _icons.insert(std::make_pair(ef->EffectName, icon));
    icon->setSize(50, 50);
    return icon;
  }
  return it->second;
}

GPlayerInterface::EffectBar::EffectBar(GPlayerInterface *p) : mPlI(p) {
}

void GPlayerInterface::EffectBar::Draw() {
  for(auto pr : _icons) {
    pr.second->hide();
  }
  int p = 0;
  for(const std::pair<std::string, Effect*> efp : mPlI->mPlayer->GetEffects()) {
    auto ic = _getIcon(efp.second);
    ic->setPosition(20  + p * 60, Game::Window->getSize().y - 60);
    ic->show();
    p++;
  }
}


void GPlayerInterface::MessageBar::AddMessage(const std::wstring& m, double duration) {
  time = duration * SECOND;
  lMessage->setText(m);
  lMessage->setPosition((Game::Window->getSize().x - lMessage->getSize().x) / 2, 200);
}

void GPlayerInterface::MessageBar::Step(double dt) {
  if(time > 0) {
    //std::cerr << "interface: " << time << '\n';
    time -= dt;
    lMessage->show();
  } else {
    lMessage->hide();
  }
}

GCharacterInfo::GCharacterInfo(GGUI& gui, ACharacter* ch) : GuiWindowReusable(gui), mChar(ch),
    mInfoName(*mWindow),  mInfoValue(*mWindow)
{
  auto wSize = Game::Window->getSize();
  const int indent = 5;
  int nameL = wSize.x / 8;
  int valueL = wSize.x / 4;
  int height = wSize.y / 4;
  mInfoName->setReadOnly(true);
  mInfoValue->setReadOnly(true);
  mInfoName->setTextColor(sf::Color::White);
  mInfoValue->setTextColor(sf::Color::White);
  mInfoName->setTextSize(25);
  mInfoValue->setTextSize(25);



  mWindow->setSize(nameL + valueL + 3 * indent, height + 2*indent);
  mInfoName->setPosition(indent, indent);
  mInfoValue->setPosition(indent + nameL + indent, indent);
  mInfoName->setSize(nameL, height);
  mInfoValue->setSize(valueL, height);
  Refresh();

}



void GCharacterInfo::Refresh() {
  ATranslationReader& tr = *Game::Translations;
  tr.SetFile("common");
  mChar->GetInfo(mInfo);
  mInfoName->setText("");
  mInfoValue->setText("");

  mInfoName->addText(tr.GetTranslation(L"Name") + L": \n");
  mInfoValue->addText(mInfo.Name + "\n");

  mInfoName->addText(tr.GetTranslation(L"Race") + L": \n");
  mInfoValue->addText(mInfo.Race + "\n");

  mInfoName->addText(tr.GetTranslation(L"Str") + L": \n");
  mInfoValue->addText(toStr(mInfo.Strength) + "\n");

  mInfoName->addText(tr.GetTranslation(L"Acc") + L": \n");
  mInfoValue->addText(toStr(mInfo.Accuracy) + "\n");

  mInfoName->addText(tr.GetTranslation(L"Reg") + L": \n");
  mInfoValue->addText(toStr(mInfo.Regen) + "\n");

}
