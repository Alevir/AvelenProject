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

#ifndef ACTION_H
#define ACTION_H


class IAction {
public:
  virtual void Activate() = 0;
  virtual void Step(double dt) = 0;
  virtual double& GetDuration() = 0;
};


class Action : public IAction {
protected:
  double mDuration;
public:
  Action(double duration, bool rep) : mDuration(duration), IsRepeating(rep) {}
  const bool IsRepeating;
  double& GetDuration() { return mDuration; }

  void Remove() { mDuration = -1.0; }
  virtual ~Action() {}
};

class DisposableAction : public Action {
protected:
  DisposableAction(double mDuration);
public:
  virtual void Step(double dt);
  virtual ~DisposableAction() {}
};

class RepeatingAction : public Action {
  double mt = 0.0;
public:
  RepeatingAction(double mDuration, double period);
  double Period;
  virtual void Step(double dt);
  virtual ~RepeatingAction() {}

};


/*
class InterruptibleAction
    : public SingleAction {
public:
  InterruptibleAction(double duration) : SingleAction(duration) {}
  virtual void Interrupt() final {
    duration = 0.0;
  }
};

class UninterruptibleAction
    : public SingleAction {
public:
  UninterruptibleAction(double duration) : SingleAction(duration) {}
  virtual void Interrupt() final {
    return;
  }
};*/

#endif // ACTION_H
