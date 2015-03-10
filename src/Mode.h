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

#ifndef MODE_H
#define MODE_H

#include "SFML/Graphics.hpp"

#include "TGUI/TGUI.hpp"
#include "Global.h"



#include "AKeyboardControllerSFML.h"
#include "CombatModels/AHumanCombatModel.h"
#include "ACharacterSFML.h"
#include "ALocationSFML.h"
#include "AWorldSFML.h"
#include "ADebugOutput.h"
#include "GUI/GCommon.h"
//#include "GUI/GInventory.h"


class Mode  {
protected:
  sf::RenderWindow* window;
  tgui::Gui mgui;
  GGUI control;
  ADebugOutput debug;

  static AStackPointer<Mode> _modes;
  virtual void _run() = 0;

public:
  //GMessageManager MessageManager;
  Mode();
  void Run();
  virtual ~Mode();
  static Mode& GetCurrentMode();
};


class MainMenu : public Mode {

  tgui::Button::Ptr bSinglePlayer;
  tgui::Button::Ptr bMultiPlayer;
  tgui::Button::Ptr bEditor;
  tgui::Button::Ptr bAbout;
  tgui::Button::Ptr bExit;
  void _run();

public:
  MainMenu();

};


class WorldMode : public Mode {
protected:
  AWorldSFML world;
public:
  virtual void ResetPlayer()= 0;
  WorldMode();
};



class MultiplayerMode : public Mode {

  void _run();
public:
  MultiplayerMode();


};



#endif // MODE_H
