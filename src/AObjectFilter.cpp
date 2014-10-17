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
#include "AObjectFilter.hpp"
#include "APhysicObjectBase.h"
#include <sstream>

void OFIsCharacter::Check(APhysicObjectBase*& obj) {
  if(obj->GetTemplateData()->objectType != APhysicObjectData::OT_Character) obj = 0;
}


void OFSingleAction::Check(APhysicObjectBase*& obj) {
  auto it = mFiltered.find(obj->GetScriptID());
  if(it == mFiltered.end()) {
    mFiltered.insert(obj->GetScriptID());
    return;
  }
  obj = 0;
}

void OFSingleAction::Save(libconfig::Setting& root) {
  AObjectFilter::Save(root);
  std::stringstream sstr;
  for(unsigned int i : mFiltered) {
    sstr << i <<' ';
  }
  root.add("filtered", libconfig::Setting::TypeString) = sstr.rdbuf()->str();
}

void OFSingleAction::SetFiltered(size_t id) {
  mFiltered.insert(id);
}


AFilterPipe::~AFilterPipe() {
  for(auto p : mFilters) {
    delete p;
  }
}

void AFilterPipe::Check(APhysicObjectBase*& obj) {
  if(mFilters.empty()) return;
  for(auto p : mFilters) {
    p->Check(obj);
    if(!obj) return;
    }
}

void AFilterPipe::Save(libconfig::Setting& root) {
  using namespace libconfig;
  AObjectFilter::Save(root);
  Setting& s = root.add("pipe", Setting::TypeList);
  for(auto p : mFilters) {
    p->Save(s.add(Setting::TypeGroup));
  }
}


AObjectFilter*AObjectFilter::CreateFilter(libconfig::Setting& s) {
  using namespace libconfig;
  std::string str = (const char*)s["name"];

  if(str == "Pipe") {
    AFilterPipe* f = new AFilterPipe();
    Setting& pipe = s["pipe"];
    for(int i = 0; i < pipe.getLength(); ++i) {
      f->AddFilter(CreateFilter(pipe[i]));
    }
    return f;
  }
  if(str == "IsCharacter") {
    return new OFIsCharacter();
  }
  if(str == "SingleAction") {
    OFSingleAction* f = new OFSingleAction();
    std::stringstream sstr((const char*)s["filtered"]);
    size_t id;
    while(sstr >> id) {
      f->SetFiltered(id);
    }
    return f;
  }
  throw std::logic_error("unknown filter type");
}

void AObjectFilter::Save(libconfig::Setting& root) {
  using namespace libconfig;
  root.add("name", Setting::TypeString) = GetName();
}
