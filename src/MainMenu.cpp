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

    Authors of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/


#include "EditorMode.h"
#include "SinglePlayerMode.hpp"
#include "ATranslationReader.h"
using namespace std;


MainMenu::MainMenu()
    : bSinglePlayer(control.Gui),
    bMultiPlayer(control.Gui), bEditor(control.Gui),
    bAbout(control.Gui), bExit(control.Gui) {

  int ButtonWidth = 350;
  int ButtonHeight = 70;
  debug.Switch();

  ATranslationReader tr(Game::TranslationsPath + Game::Language + "/mainMenu");
  dout << tr.GetTranslation(L"sp");

	bSinglePlayer->load(Game::GuiConfFileName);
	bSinglePlayer->setSize(ButtonWidth, ButtonHeight);
	bSinglePlayer->setPosition(300, 200);
	bSinglePlayer->setText(tr.GetTranslation(L"sp"));
	bSinglePlayer->setTextColor(sf::Color(255, 255, 150));
	bSinglePlayer->bindCallback(tgui::Button::LeftMouseClicked);
	bSinglePlayer->setCallbackId(0);

	bMultiPlayer->load(Game::GuiConfFileName);
	bMultiPlayer->setSize(ButtonWidth, ButtonHeight);
	bMultiPlayer->setPosition(300, 300);
	bMultiPlayer->setText(tr.GetTranslation(L"mp"));
	bMultiPlayer->bindCallback(tgui::Button::LeftMouseClicked);
	bMultiPlayer->setCallbackId(1);

	bEditor->load(Game::GuiConfFileName);
	bEditor->setSize(ButtonWidth, ButtonHeight);
	bEditor->setPosition(300, 400);
	bEditor->setText(tr.GetTranslation(L"ed"));
	bEditor->bindCallback(tgui::Button::LeftMouseClicked);
	bEditor->setCallbackId(2);

	bAbout->load(Game::GuiConfFileName);
	bAbout->setSize(ButtonWidth, ButtonHeight);
	bAbout->setPosition(300, 500);
	bAbout->setText(tr.GetTranslation(L"ab"));
	bAbout->bindCallback(tgui::Button::LeftMouseClicked);
	bAbout->setCallbackId(3);

	bExit->load(Game::GuiConfFileName);
	bExit->setSize(ButtonWidth, ButtonHeight);
	bExit->setPosition(300, 600);
	bExit->setText(tr.GetTranslation(L"ex"));
	bExit->bindCallback(tgui::Button::LeftMouseClicked);
	bExit->setCallbackId(4);
}


void MainMenu::_run() {
  while(window->isOpen()) {
    sf::Event event;
    while(window->pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
          window->close();
          break;
      } else
      if (event.type == sf::Event::Resized) {
        sf::View v = window->getView();
        v.setSize(event.size.width, event.size.height);
        window->setView(v);
        continue;
      } else
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
        window->close();
        continue;
      } else
      control.Gui.handleEvent(event);
    }
    tgui::Callback callback;
    while (control.Gui.pollCallback(callback)) {

      switch(callback.id) {

      case 0: {
          SinglePlayerMode spm;
          spm.Run();
        }
        break;
      case 1:
        break;
      case 2: {
        EditorMode em;
        em.Run();
        }
        break;
      case 3:
        break;
      case 4:
        window->close();
        break;
      }
    }

    window->clear();
    control.Gui.draw();
    window->display();
  }
}
