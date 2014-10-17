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

#ifndef ALOCATIONSFML_H
#define ALOCATIONSFML_H
#include "ALocationBase.h"

class AWorldSFML;

class ALocationSFML : public ALocationBase {
  AWorldSFML* _worldSFML;
public:
  static void LoadGroundCells(const std::string& iLocationFileName);
  static void Finalize();
  //ALocationSFML(std::string iLocationFilename, AWorldSFML* iWorld);  // create from hard disk
  ALocationSFML(libconfig::Config& iLocationFile, AWorldSFML* iWorld, const ContextLocArgs& la = ContextLocArgs());       // create from RAM
  void Display();
  AWorldSFML* GetMasterWorldSFML() { return _worldSFML; }
};

#endif // ALOCATIONSFML_H
