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

#ifndef ACONTROLLERBASE_H
#define ACONTROLLERBASE_H
class ACharacterBase;

enum class Controllers : unsigned int {
  NoController = 0,
  KeyboardController = 1
};

class AControllerBase {


  double (*method)(double);
  int dirSum;
  int dirCount;
  double t = 0.0;
protected:

  const char RIGHT = 0;
  const char LEFT = 1;
  ACharacterBase* character;
  void MoveForward();
  void MoveBack();
  void MoveRight();
  void MoveLeft();
  void Turn(char side);
  void Turn(int angleMeasure);
  void JumpForward();
  void Attack(int attackType);
  void CheckDirections(double dt);
public:
  virtual bool IsEnabled() { return true; }
  virtual void SetEnabled(bool v) {}
  virtual bool Switch() { return true; }
  virtual unsigned int GetType() const = 0;
  AControllerBase(ACharacterBase* iNPC);
  virtual void Step(double dt) = 0;
  virtual ~AControllerBase() {}


};

#endif // ACONTROLLERBASE_H
