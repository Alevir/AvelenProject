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

#include "AVector2.h"
#include "cmath"

AVector2::AVector2(double ix, double iy) : x(ix), y(iy) {}

AVector2::AVector2(double ix) : x(ix), y (0) {}


void AVector2::Set(double ix, double iy) {
   x = ix;
   y = iy;
}


AVector2& AVector2::operator *= (double f) {
  x*=f;
  y*=f;
  return *this;
}

AVector2& AVector2::operator /= (double f) {
  x/=f;
  y/=f;
  return *this;
}


AVector2& AVector2::operator += (const AVector2 &v) {
  x+=v.x;
  y+=v.y;
  return *this;
}

AVector2& AVector2::operator -= (const AVector2 &v) {
  x-=v.x;
  y-=v.y;
  return *this;
}

double AVector2::GetDistance(const AVector2 &v) {
return sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
}

AVector2 AVector2::operator / (double f) {
  return AVector2(x / f, y / f);
}

AVector2::operator b2Vec2() const {
  return b2Vec2(x, y);
}

AVector2::AVector2() {
  x = 0;
  y = 0;
}

AVector2::AVector2(const b2Vec2& v) {
  x = v.x;
  y = v.y;
}

AVector2& AVector2::Rotate(double angle) {
  double s = sin(angle);
  double c = cos(angle);
  double xt = x;
  xt = x*c - y*s;
  y =  x*s + y*c;
  x = xt;
  return *this;
}

std::ostream& operator << (std::ostream& stream, const AVector2& v) {
  stream << " ( " << v.x << " , " << v.y << " ) " << std::endl;
  return stream;
}

AVector2::operator sf::Vector2f() const {
  return sf::Vector2f(x,y);
}
