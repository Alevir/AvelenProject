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

#ifndef ACONTROLLERSFML_H
#define ACONTROLLERSFML_H
#include "AControllerBase.h"
#include "AWorldBase.h"
#include <queue>
#include "SFML/System.hpp"
#include "ACharacterSFML.h"


class AKeyboardControllerSFML
    : public AControllerBase {

  bool mEnabled = false;
  void subStep(double dt);

public:
  AKeyboardControllerSFML(ACharacter* iNPC);
  bool Switch() {
    return mEnabled = !mEnabled;
  }

  bool IsEnabled() { return mEnabled; }
  void SetEnabled(bool v) { mEnabled = v; }

  unsigned int GetType() const { return static_cast<unsigned int>(Controllers::KeyboardController); }

};

#endif // ACONTROLLERSFML_H
