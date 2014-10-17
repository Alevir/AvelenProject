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

#ifndef ASPRITEDATA_H
#define ASPRITEDATA_H
#include "AData.h"

struct ASpriteData : public AData {
  std::string name = "no name";
  std::string imagePath = "no path";
  int top = 0;
  int left = 0;
  int gWidth = 0;
  int gHeight = 0;

  //sprite scaling for a correct display of physic processes
  double xSprSize = 1.0;
  double ySprSize = 1.0;

  void print(std::ostream& stream) const;
};

#endif // ASPRITEDATA_H
