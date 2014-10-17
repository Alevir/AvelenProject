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

#ifndef ASPRITE_H
#define ASPRITE_H
#include "ASpriteData.h"
#include "ATextureContainer.h"
#include "SFML/Graphics.hpp"

class ASpriteContainer;
const double PIXELS_IN_METER = 100.0;

class ASprite : public sf::Sprite {
  friend class ASpriteContainer;
  std::string name;

  ASprite(const ASpriteData* data, ATextureContainer *iTC);

public:
  ASprite() {}
  ASprite(const ASprite& spr, const std::string &newName);
  static double renderRadius;
  static double cdx;
  static double cdy;

  static void RecountRenderBorders();


  //Angle in radians
  void setRotation(double angle);


  virtual ~ASprite() {}

};

#endif // ASPRITE_H
