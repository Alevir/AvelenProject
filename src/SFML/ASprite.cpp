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


void ASprite::SetShadowWidth(size_t w) {
  if(w == mWidth) return;
  mWidth = w;
  sf::Image img0 = getTexture()->copyToImage();
  size_t xl = img0.getSize().x;
  size_t yl = img0.getSize().y;
  sf::Image img;
  for(size_t y = 0; y < yl; ++y) {
    for(size_t x = 0; x < xl; ++x) {
      if(img0.getPixel(x, y).a > 10) {
        img0.setPixel(x, y, sf::Color::White);
      } //else img.setPixel(x, y, sf::Color(0,0,0,0))
    }
  }
  img.create(xl + 2 * w, yl + 2 * w, sf::Color::Transparent);
  img.copy(img0, w, w);
  xl = img.getSize().x;
  yl = img.getSize().y;

  for(size_t y = w; y < yl - w; ++y) {
    for(size_t x = w; x < xl - w; ++x) {
      if(img.getPixel(x,y).a == 255
      && (img.getPixel(x,y+1).a !=255
      || img.getPixel(x-1,y+1).a !=255
      || img.getPixel(x-1,y).a !=255
      || img.getPixel(x-1,y-1).a !=255
      || img.getPixel(x,y-1).a !=255
      || img.getPixel(x+1,y-1).a !=255
      || img.getPixel(x+1,y).a !=255
      || img.getPixel(x+1,y+1).a !=255)
      ) {
        for(size_t k = 1; k < w; ++k) {
          int a = (255 * (w - k)) / w / 1.5;
          if(img.getPixel(x-k, y).a < a) img.setPixel(x-k, y, sf::Color(0, 0, 0, a));
          if(img.getPixel(x-k, y-k).a < a) img.setPixel(x-k, y-k, sf::Color(0, 0, 0, a));
          if(img.getPixel(x, y-k).a < a) img.setPixel(x, y-k, sf::Color(0, 0, 0, a));
          if(img.getPixel(x+k, y-k).a < a) img.setPixel(x+k, y-k, sf::Color(0, 0, 0, a));
          if(img.getPixel(x+k, y).a < a) img.setPixel(x+k, y, sf::Color(0, 0, 0, a));
          if(img.getPixel(x+k, y+k).a < a) img.setPixel(x+k, y+k, sf::Color(0, 0, 0, a));
          if(img.getPixel(x, y+k).a < a) img.setPixel(x, y+k, sf::Color(0, 0, 0, a));
          if(img.getPixel(x-k, y+k).a < a) img.setPixel(x-k, y+k, sf::Color(0, 0, 0, a));
        }
      }
    }
  }

  mShadowTex.loadFromImage(img);
  mShadowSpr.setTexture(mShadowTex, true);
  mShadowSpr.setOrigin(getOrigin().x + w, getOrigin().y + w);
  mShadowSpr.scale(getScale().x, getScale().y);
}


void ASprite::SetShadowColor(const sf::Color& c) {
  mShadowSpr.setColor(c);
}

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
  double a = angle * 180 / M_PI;
  sf::Sprite::setRotation(a);
  mShadowSpr.setRotation(a);
}



void ASprite::RecountRenderBorders() {
  renderRadius = (Game::Window->getSize().x + Game::Window->getSize().y) / PIXELS_IN_METER / 2;
  cdx = Game::Window->getView().getCenter().x / PIXELS_IN_METER;
  cdy = - Game::Window->getView().getCenter().y / PIXELS_IN_METER;
}

void ASprite::setPosition(float x, float y) {
  sf::Sprite::setPosition(x, y);
  mShadowSpr.setPosition(x,y);
}
