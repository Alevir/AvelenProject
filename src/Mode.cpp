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

#include "Mode.h"




AStackPointer<Mode> Mode::_modes;

Mode::Mode() : window(Game::Window), mgui(*window), control(mgui),  debug(control)/*, MessageManager(control)*/ {
  control.Gui.loadWidgetsFromFile(Game::GuiConfFileName);
  control.Gui.setGlobalFont(Game::DefaultFont);
  dout.Push(&debug);
}

void Mode::Run() {
  _modes.Push(this);
  _run();
  _modes.Pop();
  trash.Clean();
}


Mode::~Mode() {
  dout.Pop();
}

Mode& Mode::GetCurrentMode() {
  return *_modes;
}


WorldMode::WorldMode() {
  world.Load(Game::ResourcesPath + world.WorldPath);
  control.SetWorld(&world);
}
