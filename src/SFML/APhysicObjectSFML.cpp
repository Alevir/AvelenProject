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

#include "APhysicObjectSFML.h"
#include "AWorldSFML.h"
#include "ALocationSFML.h"
#include "Global.h"

#include "ASpriteContainer.h"


APhysicObjectSFML::APhysicObjectSFML(const APhysicObjectData* data, ALocationSFML *loc, const ObjectInitArgs& args)
    : APhysicObjectBase(data, loc, args) {
  sfmlWorld = loc->GetMasterWorldSFML();
  switch(data->drawType) {
  case APhysicObjectData::DT_Sprite:
    sprite = Game::ObjectSpriteContainer->GetSprite(data->sprite);
    break;
  case APhysicObjectData::DT_Animation:
    animation = Game::animContainer->GetAnimation(data->animation);
    break;
  case APhysicObjectData::DT_Scheme:
    scheme = new AAnimationScheme(Game::AnimationSchemes->GetTemplate(data->scheme));
  }
  if(args.Coords) {
    mExtraCoord[0] = body->GetPosition().x;
    mExtraCoord[1] = body->GetPosition().y;
    mExtraCoord[2] = body->GetAngle();
  }
}


/*
APhysicObjectSFML::APhysicObjectSFML(const APhysicObjectData &templateData, AWorldSFML* iWorld, unsigned int ID)
    : APhysicObjectBase(templateData, iWorld, ID) {
  sfmlWorld = iWorld;
  sprite = sfmlWorld->GetSprite(templateData.templateName);
}*/


double MaxObjectHalfSize = 1.0;

void APhysicObjectSFML::Extrapolate(double dt) {
  deltaAc += dt;
  if(!visible) return;
  if(wasChanged) {
    delta = deltaAc;
    deltaAc = 0.0;
    c[x] = mExtraCoord[x] = body->GetPosition().x;
    c[y] = mExtraCoord[y] = body->GetPosition().y;
    c[a] = mExtraCoord[a] = body->GetAngle();
    for(int i = 0; i < 3; i++) {
      vp[i] = vc[i];
    }
    vc[x] = body->GetLinearVelocity().x;
    vc[y] = body->GetLinearVelocity().y;
    vc[a] = body->GetAngularVelocity();
    for(int i = 0; i < 3; i++) {
      ac[i] = (vc[i] - vp[i]) / delta;
    }

    wasChanged = false;
  } else {
    for(int i = 0; i < 3; i++) {
      mExtraCoord[i] = c[i] + vc[i] * deltaAc / SECOND + ac[i] * deltaAc * deltaAc  / SECOND / 2;
     /*std::cout << vc[i] * deltaAc / SECOND << std::endl;
      std::cout << ac[i] * deltaAc * deltaAc / SECOND << std::endl;*/
    }
  }
}

void APhysicObjectSFML::Display(double dt) {
  if(!visible) return;
  if( mExtraCoord[x] < ASprite::cdx - ASprite::renderRadius || mExtraCoord[x] > ASprite::cdx + ASprite::renderRadius
    || mExtraCoord[y] < ASprite::cdy - ASprite::renderRadius || mExtraCoord[y] > ASprite::cdy + ASprite::renderRadius) return;

  ASprite* spr;
  switch(templateData.drawType) {
  case APhysicObjectData::DT_Sprite:
    spr = sprite;
    break;
  case APhysicObjectData::DT_Animation:
    spr = animation.Step(dt);
    break;
  case APhysicObjectData::DT_Scheme:
    spr = scheme->Step(dt);
  }

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

    /*sf::Color c = spr->getColor();
    flickTime -= dt;
    uint8 i = 150 + 105 * (sin(flickTime / 2.0e5) + 1) / 2;
    spr->setColor(sf::Color(i, i, 255));
    Game::Window->draw(*spr);
    spr->setColor(c);*/
    //Game::Window->draw(s);
    flickering = false;
    sfmlWorld->focused = true;
  }
  Game::Window->draw(*spr);


}


APhysicObjectSFML::~APhysicObjectSFML() {
  if(templateData.drawType == APhysicObjectData::DT_Scheme) {
    delete scheme;
  }
}

