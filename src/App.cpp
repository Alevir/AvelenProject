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

#include <iostream>
#include <string>
#include <fstream>
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <cmath>
#include "Global.h"

using namespace std;
#include <cmath>
#include "Mode.h"



#ifdef __linux__
#include <X11/Xlib.h>

#endif



int main(int argc, char** argv) {

#ifdef __linux__

  XInitThreads();

#endif
  Game g;
  MainMenu menu;
  menu.Run();
	return 0;
}
