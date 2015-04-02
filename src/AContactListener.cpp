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

#include "AContactListener.h"
#include <iostream>
#include "ACharacter.h"
#include <assert.h>

#include <ADebugOutput.h>

void AContactListener::BeginContact(b2Contact *contact) {
  double rest = (contact->GetFixtureA()->GetBody()->GetLinearVelocity() - contact->GetFixtureB()->GetBody()->GetLinearVelocity()).Length();
  //dout << rest; dout << '\n';
  if(rest < 4.0) {
    rest *= 100.0 / 4.0;
    s.setVolume(rest);
  } else {
    s.setVolume(100);
  }


  s.play();
}

void AContactListener::EndContact(b2Contact* contact) {
}

AContactListener::AContactListener() {
  s.openFromFile("../resources/sounds/wood.wav");
}
