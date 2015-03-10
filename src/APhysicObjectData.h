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

#ifndef APHYSICOBJECTDATA_H
#define APHYSICOBJECTDATA_H
#include "AData.h"
#include <libconfig.h++>
#include "Box2D/Box2D.h"



struct ALogicMaterial {
  std::string Name;
  double Durability;
  double Resilience;
  ALogicMaterial() : Name("unknown"), Durability(0.0), Resilience(0.0) { }

  ALogicMaterial(const std::string& name, double durability, double resilience)
    : Name(name), Durability(durability), Resilience(resilience) { }
  ALogicMaterial(const ALogicMaterial& m) : Name(m.Name), Durability(m.Durability), Resilience(m.Resilience) {}

};


struct APhysicObjectData : public AData {

  enum FixtureType {
    FixtureTypeBox,
    FixtureTypeCircle,
    FixtureTypeCount
  };



  enum ObjectType {
    OT_Character = 0,
    OT_Weapon,
    OT_Armor,
    OT_Potion,
    OT_Bag,
    OT_Other,
    OT_Bottle,
    OT_Count
  };

  enum ArmorType {
    AT_Head = 0,
    AT_Body,
    AT_Gloves,
    AT_Bracers,
    AT_Legs,
    AT_Greaves,
    AT_Feet,
    AT_Belt,
    AT_Spaulders,
    AT_Count
  };

  enum WeaponType {
    WT_Sword = 0,
    WT_Dagger,
    WT_Axe,
    WT_LongSword,
    WT_Bow,
    WT_Count
  };

  enum DrawType {
    DT_Sprite,
    DT_Animation,
    DT_Scheme
  };

  static std::string FixtureTypeName[];
  static std::string ObjectTypeName[];
  static std::string ArmorTypeName[];
  static std::string WeaponTypeName[];
  static std::string b2TypeName[];



  APhysicObjectData(libconfig::Setting& data);
  APhysicObjectData() {}

  FixtureType fixtureType;
  std::string templateName;

  double density = 0.0;
  double friction = 0.0;
  b2BodyType b2type;

  // box
  double XLength = 0.0;
  double YLength = 0.0;
  // circle
  double Radius = 0.0;
  double InitWeight = -1.0;

  ObjectType objectType = OT_Other;
  ArmorType armorType = AT_Count;
  WeaponType weaponType = WT_Count;
  double InitMaxHitPoints = 1;
  double Protection;
  ALogicMaterial Material;
  void Print(std::ostream& stream) const;
  std::string sprite;
  std::string animation;
  std::string scheme;
  double externalVolume = -1.0;
  double containerCapacity = 0.0; // container capacity
  virtual void ToConfig(libconfig::Setting& s) const;
  DrawType drawType = DT_Sprite;
  double bottleVolume = 0.0;
};

#endif // APHYSICOBJECTDATA_H
