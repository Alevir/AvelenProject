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

#ifndef ADVRANDOM_H
#define ADVRANDOM_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>



template<class val>
class AdvRandom {
  std::vector<double> marks;
  std::vector<val> values;
public:

  AdvRandom() {
      srand(time(0));
      marks.push_back(0.0);
  }

  void addValue(val v, double chance);
  void changeValue(val v, double shift);
  val getValue();
};


template <class val>
void AdvRandom<val>::addValue(val v, double ch) {
  values.push_back(v);
  marks.push_back(marks.back() + ch);
}


template <class val>
val AdvRandom<val>::getValue() {
    double point = (double) (rand()) / (double)(RAND_MAX) + 0.001 * (rand() % 1000);
    if(point >= 1.0)
        point -= 1.0;
    point *= marks.back();
    for(int k = 1; k<marks.size(); k++) {
        if(point<marks[k])
            return values[k-1];
    }
    throw std::logic_error("error in AdvRandom::getValue");
}



#endif // ADVRANDOM_H
