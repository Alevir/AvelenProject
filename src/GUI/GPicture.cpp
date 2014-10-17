/*
    This file is part of Avelen Project.
    Copyright (c) 2014  Evdokimov Mikhail

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

*/


#include "GPicture.hpp"




void GPicture::SetSprite(const sf::Sprite &spr) {
  m_Texture.sprite = spr;
}



sf::Sprite GPicture::GetSprite() {
  return m_Texture.sprite;
}

bool GPicture::load(const std::string &filename) {
  throw std::logic_error("method load() called");
}

void GPicture::setSize(float width, float height) {
  double r;
  if((m_Texture.sprite.getTextureRect().width / m_Texture.sprite.getTextureRect().height) < (width / height)) {
    r = height / m_Texture.sprite.getTextureRect().height;
  } else {
    r = width / m_Texture.sprite.getTextureRect().width;
  }
  m_Texture.sprite.setScale(r , r);
}

