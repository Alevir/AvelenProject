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

#include "AKeyboardControllerSFML.h"
#include <iostream>
#include "Actions/CombatActions.h"
#include <cmath>
#include "CombatModels/AHumanCombatModel.h"
#include "Global.h"

AKeyboardControllerSFML::AKeyboardControllerSFML(ACharacter *iNPC) : AControllerBase(iNPC) {

}

const int Strike = 3;
const int HighHit = 2;
const int MediumHit = 1;
const int LowHit = 0;

/*
MovementAction* AKeyboardControllerSFML::mActions[] = {

};*/

void AKeyboardControllerSFML::subStep(double dt) {
  if(!mEnabled) return;
  double MouseClickRelaxTime = SECOND / 4.0;
  double KeyRelaxTime = SECOND / 4.0;
  static double t = 0;
  static double t1 = 0;
  t += dt;
  t1 += dt;
  static int x = Game::Window->getSize().x / 2/* + Game::Window->getPosition().x*/;
  int dx = sf::Mouse::getPosition(*Game::Window).x;
  if(dx < 0) dx = 0;
  if(dx > Game::Window->getSize().x) dx = Game::Window->getSize().x;
  dx = x - dx;
  sf::Mouse::setPosition(sf::Vector2i(x, 500), *Game::Window);

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
    moveForward();
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    moveBack();
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    moveRight();
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    MoveLeft();
  }

  if(dx != 0) {
    turn((int)(dx /** Abs(dx) / 400*/));
  }

  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    turn(RIGHT);
  }
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    turn(LEFT);
  }

  if(sf::Mouse::isButtonPressed(sf::Mouse::Left) && t > MouseClickRelaxTime) {
    t = 0;
    character->mCombatModel->GenerateAction(Strike);
  }
  if(t1 > KeyRelaxTime) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R) ) {
      t1 = 0;
      character->mCombatModel->GenerateAction(HighHit);
    } else
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
      t1 = 0;
      character->mCombatModel->GenerateAction(MediumHit);
    } else
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::V)) {
      t1 = 0;
      character->mCombatModel->GenerateAction(LowHit);
    }
  }

}
