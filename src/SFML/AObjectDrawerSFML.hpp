/*
    This file is part of Avelen Project.
    Copyright (c) 2014  Evdokimov Mikhail

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

*/

#ifndef AOBJECTDRAWERSFML_HPP
#define AOBJECTDRAWERSFML_HPP

#include "AObjectDrawer.hpp"
#include "AAnimationScheme.hpp"
#include "const.h"
class AWorldSFML;

class AObjectDrawerSFML : public AObjectDrawer {

  friend class ALocationBase;
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
  virtual ~AObjectDrawerSFML();
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
  AObjectDrawerSFML(ALocationBase* loc, APhysicObjectBase* obj);

  void GetExtrapolatedCoords(ATransform& tr);
  void Extrapolate(double dt);
  void Draw(double dt);

};



#endif // AOBJECTDRAWERSFML_HPP
