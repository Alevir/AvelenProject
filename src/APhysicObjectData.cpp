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

#include "APhysicObjectData.h"
#include "AWorldBase.h"
#include <stdexcept>
#include "Global.h"


std::string APhysicObjectData::FixtureTypeName[] = {
  "box",
  "circle"
};

std::string APhysicObjectData::ObjectTypeName[] = {
  "Character",
  "Weapon",
  "Armor",
  "Potion",
  "Bag",
  "Other",
  "Bottle"
};


std::string APhysicObjectData::ArmorTypeName[] = {
  "Head",
  "Body",
  "Gloves",
  "Bracers",
  "Legs",
  "Greaves",
  "Feet",
  "Belt",
  "Spaulders",
  "NotArmor"
};

std::string APhysicObjectData::b2TypeName[] = {
	"static",
	"kinematic",
	"dynamic"
};


std::string APhysicObjectData::WeaponTypeName[] = {
  "Sword",
  "Dagger",
  "Axe",
  "LongSword",
  "Bow",
  "NotWeapon"
};


APhysicObjectData::APhysicObjectData(libconfig::Setting &data) : Material(Game::LoadedMaterials[(const char*)data["material"]]) {
  templateName = (const char*)data["name"];



  std::string box2type = data["b2type"];
  if(box2type == "dynamic") {
    b2type = b2_dynamicBody;
  } else if(box2type == "kinematic") {
    b2type = b2_kinematicBody;
  } else if(box2type == "static") {
    b2type = b2_staticBody;
  } else {
    throw std::logic_error("object template constructor: unknown box2d type");
  }


  density = data["density"];
  friction = data["friction"];

  std::string fixture = data["fixtureType"];
  if(fixture == "box") {
    fixtureType = FixtureTypeBox;
    XLength = data["XLength"];
    YLength = data["YLength"];
    InitWeight = density * XLength * YLength;
  } else
  if(fixture == "circle") {
    fixtureType = FixtureTypeCircle;
    Radius = data["radius"];
    InitWeight = density * M_PI * Radius * Radius;
  } else {
    throw std::logic_error("fixture type");
  }


  int type = data["objectType"];
  objectType = static_cast<ObjectType>(type);
  if(objectType == OT_Armor) {
    armorType = static_cast<ArmorType>((int)data["armorType"]);
    Protection = data["protection"];
  } else
  if(objectType == OT_Weapon) {
    weaponType = static_cast<WeaponType>((int)data["weaponType"]);
  }
  InitMaxHitPoints = data["maxHitPoints"];
  data.lookupValue("sprite", sprite);
  if(sprite.size()) {
    drawType = DT_Sprite;
  } else {
    data.lookupValue("animation", animation);
    if(sprite.size()) {
      drawType = DT_Animation;
    } else {
      data.lookupValue("scheme", scheme);
      if(scheme.size()) {
        drawType = DT_Scheme;
      } else throw std::logic_error("no drawing method");
    }
  }

  if(!data.lookupValue("externalVolume",  externalVolume)) {
    if(fixtureType == FixtureTypeBox) {
      externalVolume = XLength * YLength;
    } else
    if(fixtureType == FixtureTypeCircle){
      externalVolume = 4.0 / 3.0 * M_PI * Radius * Radius * Radius;
    } else {
      throw std::logic_error("external volume couldn't be counted");
    }
  }
  data.lookupValue("contCapacity", containerCapacity);



}

void APhysicObjectData::Print(std::ostream& stream) const {
  stream<<"--------AObjectData--------\n";
  stream<<"name: "<<templateName<<'\n';
  //stream<<"type: "<<fixtureType<<'\n';
  stream<<"density: "<<density<<'\n';
  stream<<"friction: "<<friction<<'\n';
  //stream<<"b2type: "<<b2type<<'\n';
  stream<<"XLength: "<<XLength<<'\n';
  stream<<"YLength: "<<YLength<<'\n';
}

void APhysicObjectData::ToConfig(libconfig::Setting &s) const {
  using namespace libconfig;
  s.add("name", Setting::TypeString) = templateName;
  s.add("fixtureType", Setting::TypeString) = APhysicObjectData::FixtureTypeName[fixtureType];
  s.add("b2type", Setting::TypeString) = APhysicObjectData::b2TypeName[b2type];
  s.add("density", Setting::TypeFloat) = density;
  s.add("friction", Setting::TypeFloat) = friction;
  if(fixtureType == FixtureTypeBox) {
    s.add("XLength", Setting::TypeFloat) = XLength;
    s.add("YLength", Setting::TypeFloat) = YLength;
  } else
  if(fixtureType == FixtureTypeCircle) {
    s.add("radius", Setting::TypeFloat) = Radius;
  }
  s.add("objectType", Setting::TypeInt) = objectType;
  switch (objectType) {
  case OT_Armor:
    s.add("armorType", Setting::TypeInt) = armorType;
    s.add("protection", Setting::TypeFloat) = Protection;
    break;
  case OT_Weapon:
    s.add("weaponType", Setting::TypeInt) = weaponType;
    break;
  default:
    break;
  }
  s.add("material", Setting::TypeString) = Material.Name;
  s.add("maxHitPoints", Setting::TypeFloat) = InitMaxHitPoints;
  s.add("externalVolume", Setting::TypeFloat) = externalVolume;
  s.add("contCapacity", Setting::TypeFloat) = containerCapacity;
  s.add("sprite", Setting::TypeString) = sprite;

}
