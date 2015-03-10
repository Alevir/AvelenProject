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

#include "ALocationBase.h"
#include <fstream>
#include <sstream>
#include "AWorldBase.h"
#include "sstream"
#include "AControllerBase.h"
#include "AKeyboardControllerSFML.h"
#include "AHumanCombatModel.h"
#include "ADebugOutput.h"
#include "Global.h"
#include "Character/Effects.hpp"
#include "AObjectFilter.hpp"
#include "AGroundCell.h"
#include "APhysicObjectBase.h"
#include "ACharacterBase.h"
#include "Common.h"
#include <cmath>

using namespace std;
using namespace libconfig;

//static
std::map<std::string, AGroundCell*> ALocationBase::scriptGroundContainer;


void ALocationBase::Step(double dt) {
  ALocationBase* loc;

  auto cit = mLocalCharacters.begin();
  decltype(cit) ceit;
  while(cit != mLocalCharacters.end()) {
    ACharacterBase* obj = *cit;
    if(obj->mHP <= 0.0) {
      world->RemoveObject(obj);
      obj->Destroy();
      ceit = cit;
      cit++;
      mLocalCharacters.erase(ceit);
      delete obj;
      continue;
    }

    double x = obj->GetX();
    double y = obj->GetY();
    if(x > msx && y > msy && x < (mXLen * cellSize + msx) && y < (mYLen * cellSize + msy)) {
      obj->SetDamping(mGround[int(x / cellSize) % mXLen][int(y / cellSize)  % mYLen]->Damping);
    }
    obj->Step(dt);


    int xx, yy;
    loc = boundsTest(obj, xx, yy);
    if(loc == this) {
      ++cit;
      continue;
    }

    if(loc) {
      if(*cit == world->GetPlayer()) {
        world->currentLoc = loc;
      }
      transferCharacter(cit, loc);
      if(cit == mLocalCharacters.end()) break;
      ++cit;
      continue;
    } else {
      AVector2 p(obj->GetPosition());
      if(xx > mlx) {
        p.x = msx + mXLen * cellSize;
      } else
      if(xx < mlx){
        p.x = msx;
      }
      if(yy > mly) {
        p.y = msy + mYLen * cellSize;
      } else
      if(yy < mly) {
        p.y = msy;
      }
      obj->SetPosition(p);
    }
    ++cit;
  }


  auto oit = mLocalObjects.begin();
  decltype(oit) oeit;
  while(oit != mLocalObjects.end()) {
    APhysicObjectBase* obj = *oit;
    if(obj->containingObject) {
     ++oit;
     continue;
    }

    double x = obj->GetX();
    double y = obj->GetY();
    if(x > msx && y > msy && x < (mXLen * cellSize + msx) && y < (mYLen * cellSize + msy)) {
      obj->SetDamping(mGround[int(x / cellSize) % mXLen][int(y / cellSize)  % mYLen]->Damping);
    }
    obj->Step(dt);

    int xx, yy;
    loc = boundsTest(obj, xx, yy);
    if(loc == this) {
      ++oit;
      continue;
    }

    if(loc) {
      transferObj(oit, loc);
      if(oit == mLocalObjects.end()) break;
      ++oit;
      continue;
    } else {
      AVector2 p(obj->GetPosition());
     if(xx > mlx) {
        p.x = msx + mXLen * cellSize;
      } else
      if(xx < mlx){
        p.x = msx;
      }
      if(yy > mly) {
        p.y = msy + mYLen * cellSize;
      } else
      if(yy < mly) {
        p.y = msy;
      }
      obj->SetPosition(p);
    }
    ++oit;

  }
  mScrCont.Step(dt);
}


ALocationBase::ALocationBase(AWorldBase *iWorld) : mScrCont(iWorld, iWorld->scriptWrapper) {
  world = iWorld;
}





const AGroundCell* ALocationBase::GetCell(int x, int y) {
  return mGround[x][y];
}


ALocationBase::~ALocationBase() {
  for(APhysicObjectBase* obj : mLocalObjects) {
    world->RemoveObject(obj);
    delete obj;
  }
  for(ACharacterBase* obj : mLocalCharacters) {
    world->RemoveObject(obj);
    delete obj;
  }
  for(int i = 0; i < mXLen; i++) {
    delete[] mGround[i];
  }
  delete[] mGround;
}



APhysicObjectBase* ALocationBase::AddObject(std::string iTemplateName, ObjectInitArgs& args) {
  APhysicObjectBase* obj;
  args.Coords = true;
  obj = world->CreateObject(iTemplateName, this, args);
  mLocalObjects.insert(obj);
  return obj;
}

APhysicObjectBase* ALocationBase::AddObject(std::string iTemplateName, APhysicObjectBase* container, ObjectInitArgs& args) {
  APhysicObjectBase* obj;
  args.Coords = false;
  obj = world->CreateObject(iTemplateName, this, args);
  container->AddObjectToContainer(obj);
  mLocalObjects.insert(obj);
  return obj;
}

APhysicObjectBase* ALocationBase::AddObject(std::string iTemplateName, ACharacterBase* character, InventorySlot slot, ObjectInitArgs& args) {
  APhysicObjectBase* obj;
  args.Coords = false;
  obj = world->CreateObject(iTemplateName, this, args);
  character->DirectEquip(obj, slot);
  mLocalObjects.insert(obj);
  return obj;
}

ACharacterBase* ALocationBase::AddCharacter(const ACharacterData* chd, ObjectInitArgs& args) {
  ACharacterBase* ch;
  args.Coords = true;
  ch = world->CreateCharacter(chd, this, args);
  mLocalCharacters.insert(ch);
  return ch;
}



void ALocationBase::loadLocationFromCfg(libconfig::Config& loc, const ContextLocArgs& la) {
  msx = la.Sx;
  msy = la.Sy;
  mlx = la.Lx;
  mly = la.Ly;
  if(!(
    loc.lookupValue("XLen", mXLen) &&
    loc.lookupValue("YLen", mYLen)
  )) throw std::logic_error("map size wasn't defined");
  mGround = new AGroundCell**[mXLen];
  for( int i = 0; i <mXLen; i++) {
    mGround[i] = new AGroundCell*[mYLen];
  }


  Setting& nameMap = loc.lookup("nameMap");
  for(int i = 0; i < nameMap.getLength(); i++) {
    AGroundCell* c = scriptGroundContainer[nameMap[i]["name"]];
    mLoadingArray.push_back(c);
    mSavingMap.insert(pair<AGroundCell*, unsigned int>(c, i));
    mNameMap.push_back(nameMap[i]["name"]);
  }
  std::string map = loc.lookup("map");
  std::stringstream sstr;
  sstr << map;
  int g;
  for(int y = 0; y < mYLen; y++) {
    for(int x = 0; x < mXLen; x++) {
      sstr >> g;
      mGround[x][y] =  mLoadingArray[g];
    }
  }

  libconfig::Setting& objects = loc.lookup("objects");
  for(int k = 0; k < objects.getLength(); k++) {
    libconfig::Setting& s = objects[k];
    ObjectInitArgs ia;
    ia.x = (double)s["x"] + msx;
    ia.y = (double)s["y"] + msy;
    ia.angle = s["angle"];
    ia.z = s["z"];
    ia.ID = (int)s["id"];
    ia.UnID = (int)s["scrID"];
    APhysicObjectBase* obj = AddObject((const char*)s["templateName"], ia);
    _readObject(s, obj);
    _readContainer(s["container"], obj);
  }
  Setting& chars = loc.lookup("characters");
  for(int i = 0; i < chars.getLength(); i++) {
    Setting& c = chars[i];
    ACharacterData chd(Game::CharacterTemplatesContainer->GetTemplate((const char*)c["templateName"]));
    c.lookupValue("str", chd.Str);
    c.lookupValue("acc", chd.Acc);
    c.lookupValue("reg", chd.Reg);
    c.lookupValue("maxHP", chd.MaxHP);
    c.lookupValue("maxEP", chd.MaxEP);
    c.lookupValue("lvl", chd.Level);

    ObjectInitArgs ia;
    ia.x = (double)c["x"] + msx;
    ia.y = (double)c["y"] + msy;
    ia.angle = c["angle"];
    ia.z = c["z"];
    ia.ID = (int)c["id"];
    ia.UnID = (int)c["scrID"];

    ACharacterBase* ch = AddCharacter(&chd, ia);
    _readObject(c, ch);
    c.lookupValue("HP", ch->mHP);
    c.lookupValue("EP", ch->mEP);
    //load properties
    //..
    Setting& effs = c["effects"];
    for(int i = 0; i < effs.getLength(); i++) {
      ch->AddEffect(effs[i]);
    }

    switch((unsigned int)c["combatModel"]) {
    case (unsigned int)CombatModels::Human:
      ch->SetCombatModel(new AHumanCombatModel(ch));
      break;
    }

    switch((unsigned int)c["controller"]) {
    case (unsigned int)Controllers::NoController:
      ch->SetController(0);
      break;
    case (unsigned int)Controllers::KeyboardController:
      ch->SetController(new AKeyboardControllerSFML(ch));
      world->player = ch;
      break;
    }

    _readContainer(c["container"], ch);
    for(int i = 0; i < InventorySize; i++) {
      Setting& sc = c[SlotName[i].c_str()];
      string name;
      if(sc.lookupValue("templateName", name)) {
        ia.ID = (int)sc["id"];
        ia.UnID = (int)sc["scrID"];
        ch->Slots[i] = AddObject(name, ch, InventorySlot(i), ia);
        _readObject(sc, ch->Slots[i]);
        _readContainer(sc["container"], ch->Slots[i]);
      }
    }
  }
  Setting& scripts = loc.lookup("scripts");
  Setting& areas = scripts["areas"];
  for(int i = 0; i < areas.getLength(); i++) {
    Setting& a = areas[i];
    b2AABB b;
    b.lowerBound.Set(a["lowerx"], a["lowery"]);
    b.upperBound.Set(a["upperx"], a["uppery"]);

    AObjectFilter* f = 0;
    if(a.exists("filter")) {
      f = AObjectFilter::CreateFilter(a["filter"]);
    }
    mScrCont.AddScriptArea(FromCfgFormat((const char*)a["script"]), b, f);
    }
}

ALocationBase* ALocationBase::boundsTest(APhysicObjectBase* obj, int& x, int& y) {

  x = floor(obj->GetX() / mXLen);
  if(x > -1) ++x;
  y = floor(obj->GetY() / mYLen);
  if(y > -1) ++y;

  return world->getLocation(x, y);
}

void ALocationBase::transferObj(APhysicObjectBase* obj, set<APhysicObjectBase*>::iterator& validIt, ALocationBase* dest) {
  assert(obj);
  auto it = mLocalObjects.find(obj);
  if(it == mLocalObjects.end()) throw std::logic_error("no object");
  dest->mLocalObjects.insert(obj);
  obj->mLoc = dest;
  for(APhysicObjectBase* p : obj->container) {
    transferObj(p, validIt, dest);
  }
  if(validIt == it) --validIt;
  mLocalObjects.erase(it);
}

void ALocationBase::transferObj(set<APhysicObjectBase*>::iterator& it, ALocationBase* dest) {
  APhysicObjectBase* obj = *it;
  set<APhysicObjectBase*>::iterator eit;

  if(it == mLocalObjects.begin()) {
    transferObj(obj, it, dest);
    it = mLocalObjects.begin();
  } else {
    transferObj(obj, it, dest);
  }

}

void ALocationBase::transferCharacter(std::set<ACharacterBase*>::iterator& it, ALocationBase* dest) {
  ACharacterBase* obj = *it;
  set<APhysicObjectBase*>::iterator oit;

  for(int i = 0; i < InventorySize; ++i) {
    if(obj->Slots[i]) {
      oit = mLocalObjects.find(obj->Slots[i]);
      transferObj(obj->Slots[i], oit, dest);
    }
  };
  dest->mLocalCharacters.insert(obj);
  obj->mLoc = dest;
  for(APhysicObjectBase* p : obj->container) {
    oit = mLocalObjects.find(p);
    transferObj(p, oit, dest);
  }
  auto eit = it;
  it++;
  mLocalCharacters.erase(eit);
}


using namespace libconfig;

void ALocationBase::_writeObject(libconfig::Setting& group, APhysicObjectBase* obj) {
  group.add("templateName", Setting::TypeString) = obj->GetTemplateName().c_str();
  group.add("hitPoints", Setting::TypeFloat) = obj->GetHitPoints();
  if(obj->templateData.objectType == APhysicObjectData::OT_Weapon) {
    group.add("sharpness", Setting::TypeFloat) = obj->GetSharpness();
  }
  std::string str = obj->GetNotTranslatedLabel();
  if(str != "") {
    group.add("label", Setting::TypeString) = str;
  }
  group.add("x", Setting::TypeFloat) = obj->GetX() - msx;
  group.add("y", Setting::TypeFloat) = obj->GetY() - msy;
  group.add("angle", Setting::TypeFloat) = obj->GetAngle();
  group.add("z", Setting::TypeFloat) = obj->GetZ();
  group.add("id", Setting::TypeInt) = (int)obj->GetID();
  group.add("scrID", Setting::TypeInt) = (int)obj->GetScriptID();
  const std::vector<std::string>& scrv = obj->GetScripts();
  Setting& s = group.add("scripts", Setting::TypeList);
  for(int i = 0; i < scrv.size(); ++i) {
    s.add(Setting::TypeString) = ToCfgFormat(scrv[i]);
  }
}

void ALocationBase::_readObject(libconfig::Setting& s, APhysicObjectBase* obj) {
  if(obj->templateData.objectType == APhysicObjectData::OT_Weapon) {
    if(s.lookupValue("sharpness", obj->Sharpness)) {
      if(obj->Sharpness > 1.0 || obj->Sharpness < 0.0) throw std::logic_error("invalid sharpness");
    }
  }
  s.lookupValue("hitPoints", obj->HitPoints);
  std::string str;
  s.lookupValue("label", str);
  if(str != "") {
    obj->SetLabel(str);
  }
  if(s.exists("scripts")) {
    libconfig::Setting& scrList = s["scripts"];
    for(int i = 0; i < scrList.getLength(); ++i) {
      obj->SetScript(APhysicObjectBase::ScriptNode(i), FromCfgFormat((const char*)scrList[i]));
    }
  }
}
/*
void ALocationBase::_readCharacter(Setting &s, ACharacterBase *obj) {
  if(s["useTemplate"] == "no") {
    auto it = global::CharacterRaces.find((const char*)s["race"]);
    if(it == global::CharacterRaces.end()) throw std::logic_error("race not found");
    obj->race = *it;
    obj->str = s["str"];
    obj->acc = s["acc"];
    obj->reg = s["reg"];
    obj->lvl = s["lvl"];
  }
}*/

void ALocationBase::_readContainer(libconfig::Setting& cont, APhysicObjectBase* obj) {
  for(int i = 0; i < cont.getLength(); ++i) {
    Setting& s = cont[i];
    ObjectInitArgs ia;
    ia.ID = (int)s["id"];
    ia.UnID = (int)s["scrID"];
    APhysicObjectBase* n = AddObject(s["templateName"], obj, ia);
    _readObject(s, n);
    _readContainer(s["container"], n);
    }
}



void ALocationBase::_writeContainer(libconfig::Setting& cont, APhysicObjectBase* obj) {
  for(APhysicObjectBase* o : obj->container) {
    Setting& group = cont.add(Setting::TypeGroup);
    _writeObject(group, o);
    _writeContainer(group.add("container", libconfig::Setting::TypeList), o);
  }
}


void ALocationBase::Save(const std::string& iFileName) {

try {
  libconfig::Config loc;
  libconfig::Setting& s = loc.getRoot();
  s.add("XLen", libconfig::Setting::TypeInt) = mXLen;
  s.add("YLen", libconfig::Setting::TypeInt) = mYLen;
  Setting& nlist = s.add("nameMap", Setting::TypeList);
  for(int i = 0; i < mNameMap.size(); i++) {
    Setting& point = nlist.add(Setting::TypeGroup);
    point.add("name", Setting::TypeString) = mNameMap[i];
    point.add("id", Setting::TypeInt) = i;
  }

  stringstream sstr;
  string map;
  for(int y = 0; y < mYLen; y++) {
    for(int x = 0; x < mXLen; x++) {
      sstr << ' ' << mSavingMap[mGround[x][y]];
    }
  }
  map = sstr.rdbuf()->str();
  s.add("map", libconfig::Setting::TypeString) = map;
  Setting& objList = s.add("objects", libconfig::Setting::TypeList);

  for(APhysicObjectBase* obj : mLocalObjects) {
    if(!obj->containingObject) {
      Setting& group = objList.add(Setting::TypeGroup);
      _writeObject(group, obj);
      _writeContainer(group.add("container", libconfig::Setting::TypeList), obj);
    }
  }

  Setting& chList = s.add("characters", libconfig::Setting::TypeList);
  for(ACharacterBase* obj : mLocalCharacters) {
    Setting& group = chList.add(Setting::TypeGroup);
    group.add("controller", Setting::TypeInt) = (int)(obj->GetController() ? obj->GetController()->GetType() : 0 );
    group.add("combatModel", Setting::TypeInt) = (int)(obj->GetCombatModel()->GetType());
    _writeObject(group, obj);
    group.add("str", Setting::TypeFloat) = obj->GetStr();
    group.add("acc", Setting::TypeFloat) = obj->GetAcc();
    group.add("reg", Setting::TypeFloat) = obj->GetReg();
    group.add("XP", Setting::TypeFloat) = obj->GetXP();
    group.add("HP", Setting::TypeFloat) = obj->GetHP();
    group.add("EP", Setting::TypeFloat) = obj->GetEP();
    group.add("maxHP", Setting::TypeFloat) = obj->GetMaxHP();
    group.add("maxEP", Setting::TypeFloat) = obj->GetMaxEP();

    _writeContainer(group.add("container", libconfig::Setting::TypeList), obj);
    for(unsigned int i = 0; i < InventorySize; i++) {
      Setting& cell = group.add(SlotName[i].c_str(), Setting::TypeGroup);
      if(obj->Slots[i]) {
        _writeObject(cell, obj->Slots[i]);
        _writeContainer(cell.add("container", libconfig::Setting::TypeList), obj->Slots[i]);
      }
    }
    Setting& effs = group.add("effects", Setting::TypeList);
    for(const std::pair<std::string, Effect*>& p : obj->mEffects) {
      p.second->Write(effs.add(Setting::TypeGroup));
    }
  }
  mScrCont.Save(s);


  loc.writeFile((iFileName).c_str());
  }
  catch(libconfig::SettingTypeException& ex) {
    cerr << "path: " << ex.getPath() << endl;
    cerr << "error: " << ex.what() << endl;
    //throw ex;
  }

}


void ALocationBase::RemoveAObject(APhysicObjectBase* obj) {
  world->RemoveObject(obj);
  ACharacterBase* ch = dynamic_cast<ACharacterBase*>(obj);
  if(ch) {
    mLocalCharacters.erase(mLocalCharacters.find(ch));
  } else {
    mLocalObjects.erase(mLocalObjects.find(obj));
  }
  std::cerr << obj->GetID() << "  " << obj->GetTemplateName() << "\n";
  obj->Destroy();
  delete obj;
}
