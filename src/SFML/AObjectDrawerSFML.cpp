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

#include "AObjectDrawerSFML.hpp"
#include "APhysicObjectBase.h"
#include "AWorldSFML.h"
#include "Global.h"
#include "ASpriteContainer.h"



AObjectDrawerSFML::AObjectDrawerSFML(ALocationBase* loc, APhysicObjectBase* obj)
  : AObjectDrawer(loc, obj) {
  switch(mObj->GetTemplateData()->drawType) {
  case APhysicObjectData::DT_Sprite:
    sprite = Game::ObjectSpriteContainer->GetSprite(mObj->GetTemplateData()->sprite);
    break;
  case APhysicObjectData::DT_Animation:
    animation = Game::animContainer->GetAnimation(mObj->GetTemplateData()->animation);
    break;
  case APhysicObjectData::DT_Scheme:
    scheme = new AAnimationScheme(Game::AnimationSchemes->GetTemplate(mObj->GetTemplateData()->scheme));
  }
  sfmlWorld = static_cast<AWorldSFML*>(mObj->GetWorld());
}

void AObjectDrawerSFML::GetExtrapolatedCoords(ATransform& tr) {
  tr.X = c[0];
  tr.Y = c[1];
  tr.A = c[2];
}

void AObjectDrawerSFML::Extrapolate(double dt) {
  deltaAc += dt;
  if(!visible) return;
  if(mObj->CheckForChanges()) {
    delta = deltaAc;
    deltaAc = 0.0;
    ATransform t = mObj->GetTransform();
    c[x] = mExtraCoord[x] = t.X;
    c[y] = mExtraCoord[y] = t.Y;
    c[a] = mExtraCoord[a] = t.A;
    for(int i = 0; i < 3; i++) {
      vp[i] = vc[i];
    }
    mObj->GetVelocity(t);
    vc[x] = t.X;
    vc[y] = t.Y;
    vc[a] = t.A;
    for(int i = 0; i < 3; i++) {
      ac[i] = (vc[i] - vp[i]) / delta;
    }
  } else {
    for(size_t i = 0; i < 3; i++) {
      mExtraCoord[i] = c[i] + vc[i] * deltaAc / SECOND + ac[i] * deltaAc * deltaAc  / SECOND / 2;
    }
  }
}

void AObjectDrawerSFML::Draw(double dt) {
  if(!visible) return;
  if( mExtraCoord[x] < ASprite::cdx - ASprite::renderRadius || mExtraCoord[x] > ASprite::cdx + ASprite::renderRadius
    || mExtraCoord[y] < ASprite::cdy - ASprite::renderRadius || mExtraCoord[y] > ASprite::cdy + ASprite::renderRadius) return;

  ASprite* spr;
  switch(mObj->GetTemplateData()->drawType) {
  case APhysicObjectData::DT_Sprite:
    spr = sprite;
    break;
  case APhysicObjectData::DT_Animation:
    spr = animation.Step(dt);
    break;
  case APhysicObjectData::DT_Scheme:
    spr = scheme->Step(dt);
  }

  spr->SetShadowWidth(10);
  spr->setPosition(mExtraCoord[x] * PIXELS_IN_METER, - mExtraCoord[y] * PIXELS_IN_METER);
  spr->setRotation( -mExtraCoord[a]);


  if(flickering) {
    flickTime += dt;
    sf::Sprite& s = sfmlWorld->focusedShape;
    sf::Image im = spr->getTexture()->copyToImage();
    int x = im.getSize().x;
    int y = im.getSize().y;
    for(int i = 0; i < x; ++i) {
      for(int j = 0; j < y; ++j) {
        if(im.getPixel(i,j).a > 10) {
          im.setPixel(i,j,sf::Color(255, 255, 255, 255 * (sin(flickTime / 2.0e5) + 1) / 2)) ;
        }
      }
    }
    static sf::Texture tx;
    tx.loadFromImage(im);
    s.setTexture(tx,true);
    s.setOrigin(spr->getOrigin());
    s.setScale(spr->getScale());
    s.setPosition(spr->getPosition());
    s.setRotation(spr->getRotation());
    flickering = false;
    sfmlWorld->focused = true;
  }
  sf::Color cc = sf::Color::Black;
  cc.a = (sfmlWorld->dayTime.r/6 + sfmlWorld->dayTime.g/6 + sfmlWorld->dayTime.b/6) + 128;
  spr->SetShadowColor(cc);
  Game::Window->draw(spr->GetShadow());
  //Game::Window->draw(*spr);
  sfmlWorld->mPreparedSprites.push_back(AWorldSFML::drawData(spr,
                                                             mExtraCoord[x] * PIXELS_IN_METER, - mExtraCoord[y] * PIXELS_IN_METER, -mExtraCoord[a]));
}

AObjectDrawerSFML::~AObjectDrawerSFML() {
  if(mObj->GetTemplateData()->drawType == APhysicObjectData::DT_Scheme) {
    delete scheme;
  }
}
