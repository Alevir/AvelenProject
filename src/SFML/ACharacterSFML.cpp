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
#include "ACharacterSFML.h"
#include "AWorldSFML.h"
#include "AVector2.h"
#include "ALocationSFML.h"
#include "Global.h"


void ACharacterSFML::Display(double dt) {
  APhysicObjectSFML::Display(dt);
  sf::RectangleShape line(sf::Vector2f(100 * HP / maxHP, 5));
  line.setFillColor(sf::Color::Red);
  line.setOrigin(line.getSize().x / 2, line.getSize().y / 2);
  AVector2 v(0, -50.);
  v.Rotate(Game::Window->getView().getRotation() / 180 * M_PI);
  v = v + AVector2(mExtraCoord[0] * PIXELS_IN_METER, - (mExtraCoord[1] * PIXELS_IN_METER));
  line.setPosition(v);
  line.setRotation(Game::Window->getView().getRotation());
  Game::Window->draw(line);
}


ACharacterSFML::ACharacterSFML(const ACharacterData* data, ALocationSFML* loc, const ObjectInitArgs& args)
: APhysicObjectBase(data, loc, args),
APhysicObjectSFML(data, loc, args),
ACharacterBase(data, loc, args) {
}
