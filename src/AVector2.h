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

#ifndef AVECTOR2_H
#define AVECTOR2_H
#include "Box2D/Common/b2Math.h"
#include "SFML/System.hpp"
#include <iostream>

class AVector2 {

public:
  double x;
  double y;
  AVector2();
  AVector2(double x);
  AVector2(double x, double y);
  AVector2(const b2Vec2& v);
  operator sf::Vector2f() const;
  void Set(double x, double y);
  operator b2Vec2() const;
  AVector2& operator *=(double f);
  AVector2& operator /=(double f);
  AVector2& operator +=(const AVector2& v);
  AVector2& operator -=(const AVector2& v);
  double GetDistance(const AVector2& v);
  AVector2 operator / (double f);

  AVector2& Rotate(double angle);

  template<class T>
  AVector2(const sf::Vector2<T>& v) {
    x = v.x;
    y = v.y;
  }

  template<class T>
  operator sf::Vector2<T> () const {
    return sf::Vector2<T>(x, y);
  }


};

std::ostream& operator << (std::ostream& stream, AVector2&);

#endif // AVECTOR2_H
