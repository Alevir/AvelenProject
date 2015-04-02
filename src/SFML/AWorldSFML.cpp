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

#include <SFML/Graphics/RenderTexture.hpp>

#include "ADebugOutput.h"
#include "GUI/GInGame.hpp"
#include "AObjectDrawerSFML.hpp"
using namespace std;
double sz;

sf::Color operator / (const sf::Color& c, int i) {
  return sf::Color(c.r/i, c.g/ i, c.b/i, c.a/i);
}

AWorldSFML::AWorldSFML() : shadowRS(sf::BlendMultiply) {
  using namespace libconfig;
  Config cfg;
  cfg.readFile("test.cfg");
  ParseColor((const char*)cfg.lookup("color"), dayTime);


  //light source texture gen


  ALightSourceSFML::Init();


  mLights.insert(new ALightSourceSFML(20.0, AVector2(3.0, 3.0), sf::Color::Cyan));
  mLights.insert(new ALightSourceSFML(20.0, AVector2(9.0, 9.0), sf::Color(150, 255,140)));
  mLights.insert(new ALightSourceSFML(20.0, AVector2(10.0, 4.0), sf::Color(80,100, 255)));






  ASprite::RecountRenderBorders();
  sz = ASprite::renderRadius * 2 * PIXELS_IN_METER;
  shadowTex.create(sz, sz);

}

AWorldSFML::AWorldSFML(const string &startLocationName) {
  assert(0);
}



AWorldSFML::~AWorldSFML() {
  delete currentLoc;
}

void AWorldSFML::_graphicStep(double dt) {

  if(!mGrPause) {
    for(std::pair<double, APhysicObjectBase*> p : drawContainer) {
      p.second->Extrapolate(dt);
    }
  } else {
    mGrPause = false;
  }

  Camera.Step();
  ASprite::RecountRenderBorders();

  AVector2 c = Camera.GetPosition();
  AVector2 cc = c;
  findLocation(c.x, c.y);
  LoadLocation(int(c.x), int(c.y));
  c = cc;

  listener.setPosition(c.x, 0, c.y);

  for(int q = 0; q < 4; ++q) {
    for(auto& v : mMap[q]) {
      for(auto& l : v) {
        if(l.Loc) l.Loc->Display();
      }
    }
  }


  for(std::pair<double, APhysicObjectBase*> p : drawContainer) {
    p.second->Display(dt);
  }

  for(drawData sprd : mPreparedSprites) {
    sprd.Spr->setPosition(sprd.X, sprd.Y);
    sprd.Spr->setRotation(sprd.A);
    Game::Window->draw(*(sprd.Spr));
  }
  mPreparedSprites.clear();


  shadowTex.clear(dayTime);
  for(ALightSourceBase* l : mLights) {
    auto ll = (ALightSourceSFML*)(l);
    ll->SetShadowTexture(&shadowTex, c);
    ll->Illumine(0);
  }
  shadowTex.display();
  rs.setTexture(shadowTex.getTexture(), true);
  rs.setOrigin(rs.getTextureRect().width / 2, rs.getTextureRect().height / 2);
  rs.setPosition(Game::Window->getView().getCenter());

  Game::Window->draw(rs, shadowRS);
  if(focused) {
    Game::Window->draw(focusedShape);
    focused = false;
  }

  /*sf::CircleShape test;
  test.setRadius(5);
  test.setOrigin(5,5);
  test.setFillColor(sf::Color::White);
  test.setPosition(1.0 * PIXELS_IN_METER, -1.0 * PIXELS_IN_METER);
  Game::Window->draw(test);*/



  if(Interface) {
    Game::Window->setView(Game::Window->getDefaultView());
    Interface->Step(dt);
    Interface->Draw();
  }
}


APhysicObjectBase* AWorldSFML::CreateObject(const APhysicObjectData& data, ALocationBase *loc,  const ObjectInitArgs& args) {
  APhysicObjectBase* obj = createObject(data, loc, args);
  setDrawer(obj, new AObjectDrawerSFML(loc, obj));
  args.Coords ? SetVisible(obj) : SetInvisible(obj);
  drawContainer.insert(std::pair<double, APhysicObjectBase*>(args.Z, obj));
  return obj;
}



void AWorldSFML::RemoveObject(APhysicObjectBase* obj) {
  auto itp = drawContainer.equal_range(obj->GetZ());
  Camera.CheckAndUnbind(obj);
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

void AWorldSFML::SetFocusedObject(APhysicObjectBase* obj) {
  if(!obj) return;
  AObjectDrawerSFML* dr = static_cast<AObjectDrawerSFML*>(getDrawer(obj));
  dr->flickering = true;
}


void AWorldSFML::ParseColor(const char* str, sf::Color& color) {
  size_t l = strlen(str);
  if(l != 8 && l != 6) throw std::logic_error("");
  char s[3] = "00";
  s[0] = str[0];
  s[1] = str[1];
  color.r = strtoul(s, 0, 16);
  s[0] = str[2];
  s[1] = str[3];
  color.g = strtoul(s, 0, 16);
  s[0] = str[4];
  s[1] = str[5];
  color.b = strtoul(s, 0, 16);

  if(l == 8) {
    s[0] = str[6];
    s[1] = str[7];
    color.a = strtoul(s, 0, 16);
  }
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
  //obj->visible = true;
  /*auto itp = drawContainer.equal_range(obj->lastVisZ);
  while(itp.first != itp.second) {
    if(itp.first->second == obj) {
      drawContainer.erase(itp.first);
      drawContainer.insert(std::make_pair(obj->GetZ(), obj));
      break;
    } else {
      itp.first++;
    }
  }*/

  drawContainer.insert(std::make_pair(obj->GetZ(), obj));
}

void AWorldSFML::SetInvisible(APhysicObjectBase *obj) {
  //obj->visible = false;
  //obj->lastVisZ = _obj->GetZ();

  auto itp = drawContainer.equal_range(obj->GetZ());
  while(itp.first != itp.second) {
    if(itp.first->second == obj) {
      drawContainer.erase(itp.first);
      break;
    } else {
      itp.first++;
    }
  }
}

void AWorldSFML::AddMessage(const wstring& m) {
  if(Interface) {
    Interface->AddMessage(m, 3);
  } else {
    AWorldBase::AddMessage(m);
  }
}


void ACamera::Step() {
  if(mObj) {
    sf::View vv = Game::Window->getView();
    ATransform tr; mObj->GetTransformExtr(tr);
    vv.setCenter(tr.X * PIXELS_IN_METER, -tr.Y * PIXELS_IN_METER);
    vv.setRotation(- tr.A * 180 / M_PI);
    Game::Window->setView(vv);
  }
}

void ACamera::BindToObj(APhysicObjectBase* obj) {
  mObj = obj;
}

void ACamera::CheckAndUnbind(APhysicObjectBase* obj) {
  if(obj == mObj) {
    mObj = 0;
  }
}

void ACamera::Unbind() {
  mObj = 0;
}

void ACamera::Move(const AVector2 &shift) {
  if(!mObj) {
    sf::View vv = Game::Window->getView();
    auto v = vv.getCenter();
    vv.setCenter(v.x + shift.x * PIXELS_IN_METER, v.y + shift.y  * PIXELS_IN_METER);
    Game::Window->setView(vv);
  }
}

void ACamera::SetTransform(const ATransform& pos) {
  sf::View vv = Game::Window->getView();
  vv.setCenter(pos.X * PIXELS_IN_METER, - pos.X * PIXELS_IN_METER);
  vv.setRotation(- pos.A * 180.0 / M_PI);
  Game::Window->setView(vv);
}

AVector2 ACamera::GetPosition() {
  AVector2 p = 0;

  sf::View vv = Game::Window->getView();
  p += vv.getCenter();
  p /= PIXELS_IN_METER;
  p.y = -p.y;
  //std::cout << p << "\n";
  return p;
}


sf::Sprite ALightSourceSFML::mLight;
sf::Texture ALightSourceSFML::mLTex;

void ALightSourceSFML::Init() {
  sf::Image im;
  const int size = 256 * Game::LightQuality;
  im.create(size, size, sf::Color(0,0,0,255));
  double r;
  double dist;
  double linLim = 0.5;
  double linLimitInt = (cos(atan((size/2 * linLim /3 / Game::LightQuality))));

  for(int i = 0; i < size; ++i) {
    for(int j = 0; j < size; ++j) {
      dist = sqrt((i - size/2) * (i - size/2) + (j - size/2) * (j - size/2));
      if(dist > double(size/2 - 1)) continue;
      if(dist < size/2 * linLim) {
        r = 255 * (cos(atan((dist/3 / Game::LightQuality))));
      } else {
        r = 255 * linLimitInt * (1.0 - 2*(double(dist) / size * 2 - linLim));
      }
      im.setPixel(i, j, sf::Color(r,r,r,255));
    }
  }
  mLTex.loadFromImage(im);
  mLight.setTexture(mLTex);
  mLight.setOrigin(mLight.getTextureRect().width / 2, mLight.getTextureRect().height / 2);
}

ALightSourceSFML::ALightSourceSFML(double intensity, const AVector2& position, const sf::Color& color)
  : ALightSourceBase(intensity, position, color) {}

void ALightSourceSFML::SetShadowTexture(sf::RenderTexture* shadow, const AVector2& pos) {
  mShadow = shadow;
  mViewPos = pos;
}

void ALightSourceSFML::Illumine(double dt) {
  if(!mShadow) throw std::logic_error("");
  mLight.setColor(mColor);
  auto p = mPos - mViewPos;
  //auto p = mViewPos - mPos;
  p.y = -p.y;
  p *= PIXELS_IN_METER;
  mLight.setPosition(mShadow->getSize().x / 2 + p.x,
                    mShadow->getSize().y / 2 + p.y);
  double sc = mInt / Game::LightQuality;
  mLight.setScale(sc, sc);
  mShadow->draw(mLight, sf::RenderStates(sf::BlendAdd));
  mShadow = 0;
}
