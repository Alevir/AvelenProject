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

#include "AWorldSFML.h"
#include "ALocationSFML.h"
#include <fstream>
#include "Global.h"

#include "ATextureContainer.h"
#include "ASpriteContainer.h"

#include "ADebugOutput.h"
#include "GUI/GInGame.hpp"
using namespace std;
double sz;

AWorldSFML::AWorldSFML() : shadowRS(sf::BlendMultiply) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile("test.cfg");
  dayTime.a  = (int)cfg.lookup("a");
  dayTime.r  = (int)cfg.lookup("r");
  dayTime.g  = (int)cfg.lookup("g");
  dayTime.b  = (int)cfg.lookup("b");

  ASprite::RecountRenderBorders();
  sz = ASprite::renderRadius * 2 * PIXELS_IN_METER;
  shadowTex.create(sz, sz);
  shadowTex.clear(dayTime);

  lightTex.loadFromFile("light.png");
  light.setTexture(lightTex);
  light.setOrigin(lightTex.getSize().x / 2, lightTex.getSize().y / 2);
  light.setPosition(sz/2, sz/2);
  shadowTex.draw(light, sf::RenderStates(sf::BlendAdd));
  shadowTex.display();



  //light source texture gen
 /* sf::Image im;
  im.create(256, 256, sf::Color(0,0,0,255));
  for(int i = 0; i < 256; ++i) {
    for(int j = 0; j < 256; ++j) {
      double dist = sqrt((i - 128) * (i - 128) + (j - 128) * (j - 128));
      if(dist > 127.0) continue;
      double r = 127 * (1 + cos(dist / 45.0));
      im.setPixel(i, j, sf::Color(r,r,r,255));
    }
  }
  im.saveToFile("light.png");*/
}

AWorldSFML::AWorldSFML(const string &startLocationName) {
  assert(0);
}



AWorldSFML::~AWorldSFML() {
  delete currentLoc;
}

void AWorldSFML::_graphicStep(double dt) {




  if(!mGrPause) {
    for(std::pair<double, APhysicObjectSFML*> p : drawContainer) {
      p.second->Extrapolate(dt);
    }
  } else {
    mGrPause = false;
  }

  Camera.Step();
  ASprite::RecountRenderBorders();
  //rs.setSize(sf::Vector2f(sz, sz));
  rs.setOrigin(sf::Vector2f(sz/2, sz/2));
  rs.setPosition(Game::Window->getView().getCenter());
  rs.setTexture(shadowTex.getTexture());
  auto c = Camera.GetPosition();
  findLocation(c.x, c.y);
  LoadLocation(int(c.x), int(c.y));

  auto v = GetPlayer()->GetPosition();
  listener.setPosition(v.x, 0, v.y);

  for(int q = 0; q < 4; ++q) {
    for(auto& v : mMap[q]) {
      for(auto& l : v) {
        if(l.Loc) l.Loc->Display();
      }
    }
  }


  for(std::pair<double, APhysicObjectSFML*> p : drawContainer) {
    p.second->Display(dt);
  }

  Game::Window->draw(rs, shadowRS);
  



  if(Interface) {
    Game::Window->setView(Game::Window->getDefaultView());
    Interface->Step(dt);
    Interface->Draw();
  }
}


APhysicObjectSFML* AWorldSFML::CreateObject(std::string iTemplateName, ALocationBase *loc,  const ObjectInitArgs& args) {
  APhysicObjectData* data = &Game::ObjectTemplatesContainer->GetTemplate(iTemplateName);
  APhysicObjectSFML* obj = new APhysicObjectSFML(data, static_cast<ALocationSFML*> (loc), args);
  args.Coords ? SetVisible(obj) : SetInvisible(obj);
  drawContainer.insert(std::pair<double, APhysicObjectSFML*>(args.z, obj));
  return obj;
}



ACharacterSFML* AWorldSFML::CreateCharacter(const ACharacterData* chd, ALocationBase *loc, const ObjectInitArgs& args) {
  ACharacterSFML* ch = new ACharacterSFML(chd, static_cast<ALocationSFML*> (loc), args);
  SetVisible(ch);
  drawContainer.insert(std::pair<double, APhysicObjectSFML*>(args.z, ch));
  return ch;
}


void AWorldSFML::RemoveObject(APhysicObjectBase* obj) {
  auto itp = drawContainer.equal_range(obj->GetZ());
  Camera.CheckAndUnbind(dynamic_cast<APhysicObjectSFML*>(obj));
  while(itp.first != itp.second) {
    if(itp.first->second == obj) {
      drawContainer.erase(itp.first);
      break;
    } else {
      itp.first++;
    }
  }
  AWorldBase::RemoveObject(obj);
}

ALocationBase*AWorldSFML::newLocation(libconfig::Config& locationDesc, const ContextLocArgs& la = ContextLocArgs()) {
return new ALocationSFML(locationDesc, this, la);
}

void AWorldSFML::OneStepPause() {
  mGrPause = true;
  AWorldBase::OneStepPause();
}

AVector2 AWorldSFML::ConvertMouseCurrentPos() {
  sf::View v = Game::Window->getView();
  AVector2 vec = 0;
  vec += v.getCenter();
  vec += (AVector2(sf::Mouse::getPosition(*Game::Window)) -= (AVector2(Game::Window->getSize()) /= 2)).Rotate(v.getRotation() / 180 * M_PI);
  vec /= PIXELS_IN_METER;
  vec.y = -vec.y;
  return vec;
}

AVector2 AWorldSFML::ConvertMousePos(AVector2 mpos) {
  sf::View v = Game::Window->getView();
  AVector2 vec = 0;
  vec += v.getCenter();
  //vec -= (AVector2(global::Window->getSize()) /= 2);
  vec += (mpos -= (AVector2(Game::Window->getSize()) /= 2)).Rotate(v.getRotation() / 180 * M_PI);
  vec /= PIXELS_IN_METER;
  vec.y = -vec.y;
  return vec;
}

void AWorldSFML::ResetCurrentLocation(libconfig::Config& cfg) {
  delete currentLoc;
currentLoc = new ALocationSFML(cfg, this);
}

void AWorldSFML::SetVisible(APhysicObjectBase *obj) {
  APhysicObjectSFML* _obj = dynamic_cast<APhysicObjectSFML*>(obj);
  _obj->visible = true;
  auto itp = drawContainer.equal_range(_obj->lastVisZ);
  while(itp.first != itp.second) {
    if(itp.first->second == _obj) {
      drawContainer.erase(itp.first);
      drawContainer.insert(std::make_pair(_obj->GetZ(), _obj));
      break;
    } else {
      itp.first++;
    }
  }

}

void AWorldSFML::SetInvisible(APhysicObjectBase *obj) {
  APhysicObjectSFML* _obj = dynamic_cast<APhysicObjectSFML*>(obj);
  _obj->visible = false;
  _obj->lastVisZ = _obj->GetZ();
}

void AWorldSFML::AddMessage(const wstring& m) {
  if(Interface) {
    Interface->AddMessage(m, 3);
  } else {
    AWorldBase::AddMessage(m);
  }
}


void ACamera::Step() {
  if(_obj) {
    sf::View vv = Game::Window->getView();
    vv.setCenter(_obj->GetEX() * PIXELS_IN_METER, -_obj->GetEY() * PIXELS_IN_METER);
    vv.setRotation(- _obj->GetEAngle() * 180 / M_PI);
    Game::Window->setView(vv);
  }
}

void ACamera::BindToObj(APhysicObjectSFML *obj) {
  _obj = obj;
}

void ACamera::CheckAndUnbind(APhysicObjectSFML *obj) {
  if(obj == _obj) {
    _obj = 0;
  }
}

void ACamera::Unbind() {
  _obj = 0;
}

void ACamera::Move(const AVector2 &shift) {
  if(!_obj) {
    sf::View vv = Game::Window->getView();
    auto v = vv.getCenter();
    vv.setCenter(v.x + shift.x * PIXELS_IN_METER, v.y + shift.y  * PIXELS_IN_METER);
    Game::Window->setView(vv);
  }
}

void ACamera::SetPosition(const AVector2 &pos) {
  sf::View vv = Game::Window->getView();
  vv.setCenter(pos.x * PIXELS_IN_METER, - pos.y * PIXELS_IN_METER);
  Game::Window->setView(vv);
}

AVector2 ACamera::GetPosition() {
  AVector2 p;
  sf::View vv = Game::Window->getView();
  p = vv.getCenter();
  p.x /= PIXELS_IN_METER;
  p.y = -p.y / PIXELS_IN_METER;
  return p;
}
