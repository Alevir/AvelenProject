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

#ifndef APHYSICOBJECTSFML_H
#define APHYSICOBJECTSFML_H
#include "APhysicObjectBase.h"

#include "AAnimationScheme.hpp"
#include "const.h"

class AWorldSFML;
class ALocationSFML;
class AAnimationScheme;
class ASprite;

class APhysicObjectSFML
    : virtual public APhysicObjectBase {
  friend class AWorldSFML;
protected:
  ASprite* sprite;
  AAnimation animation;
  AAnimationScheme* scheme = 0;
  AWorldSFML* sfmlWorld;
  const int x = 0;
  const int y = 1;
  const int a = 2;
  bool visible = true;
  double lastVisZ = 0.0;
  virtual ~APhysicObjectSFML();
  bool flickering = false;
  double flickTime = 0.0;

  double mExtraCoord[3];
  double c[3];
  double vc[3];
  double vp[3];
  double ac[3];
  double delta = SECOND / 15.0;
  double deltaAc = SECOND / 15.0;


public:

  double k[3][4];

  APhysicObjectSFML(const APhysicObjectData* templateData, ALocationSFML* loc, const ObjectInitArgs& args);

  virtual void Display(double dt);
  void Extrapolate(double dt);
  void SetFlickering(bool b) {
    flickering = b;
  }


  double GetEX() { return mExtraCoord[0]; }
  double GetEY() { return mExtraCoord[1]; }
  double GetEAngle() { return mExtraCoord[2]; }
};

#endif // APHYSICOBJECTSFML_H
