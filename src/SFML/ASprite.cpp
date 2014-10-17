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

#include "ASprite.h"
#include <cmath>
#include "Global.h"
#ifdef _WIN32
  const double M_PI = 3.141592653589793238462643383279502884197169399375105923078164;
#endif

double ASprite::renderRadius;
double ASprite::cdx;
double ASprite::cdy;

ASprite::ASprite(const ASpriteData* data, ATextureContainer* iTC)
: sf::Sprite (*(iTC->GetTexture(data->imagePath))) {
  if(!(data->gWidth && data->gHeight)) {
    auto v = getTexture()->getSize();
    auto pData = const_cast<ASpriteData*>(data);
    pData->gWidth = v.x;
    pData->gHeight = v.y;
  }
  setTextureRect(sf::IntRect(data->left, data->top, data->gWidth, data->gHeight));
  name = data->name;
  sf::Sprite::setOrigin(data->gWidth / 2, data->gHeight / 2);
  double xSprScale =  data->xSprSize * PIXELS_IN_METER / data->gWidth;
  double ySprScale =  data->ySprSize * PIXELS_IN_METER / data->gHeight;
  if((xSprScale != 1.0) && (ySprScale != 1.0)) {
    sf::Sprite::setScale(xSprScale, ySprScale);
    }
}

ASprite::ASprite(const ASprite &spr, const std::string& newName)
: sf::Sprite(*this->getTexture(), this->getTextureRect()) {
  name = newName;
}

void ASprite::setRotation(double angle) {
  sf::Sprite::setRotation(angle * 180 / M_PI);
}



void ASprite::RecountRenderBorders() {
  renderRadius = (Game::Window->getSize().x + Game::Window->getSize().y) / PIXELS_IN_METER / 2;
  cdx = Game::Window->getView().getCenter().x / PIXELS_IN_METER;
  cdy = - Game::Window->getView().getCenter().y / PIXELS_IN_METER;
}
