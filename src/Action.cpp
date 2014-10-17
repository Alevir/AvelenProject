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

#include "Action.h"

RepeatingAction::RepeatingAction(double duration, double period) : Action(duration, true) {
  Period = period;
}

void RepeatingAction::Step(double dt) {
  _t+=dt;
  if(_t > Period) {
    Activate();
    _t -= Period;
  }
}


DisposableAction::DisposableAction(double duration) : Action(duration, false) {

}

void DisposableAction::Step(double dt) {
  if(_duration < 0.0) {
    Activate();
  }
}
