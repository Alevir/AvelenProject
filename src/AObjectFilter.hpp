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

#ifndef AOBJECTFILTER_HPP
#define AOBJECTFILTER_HPP
class APhysicObjectBase;
#include <string>
#include <libconfig.h++>
#include <vector>
#include <set>


class AObjectFilter {
protected:
  std::string mName;
public:
  const std::string& GetName() { return mName; }
  static AObjectFilter* CreateFilter(libconfig::Setting& s);
  virtual void Check(APhysicObjectBase*& obj) = 0;  //returns obj if it's OK and 0 otherwise
  virtual void Save(libconfig::Setting& root);
  virtual ~AObjectFilter() {}
};


class OFIsCharacter : public AObjectFilter {
  void Check(APhysicObjectBase*& obj);
public:
  OFIsCharacter() { mName = "IsCharacter"; }
};

class OFSingleAction : public AObjectFilter {
  std::set<unsigned int> mFiltered;
public:
  void Check(APhysicObjectBase*& obj);
  void Save(libconfig::Setting &root);
  OFSingleAction() { mName = "SingleAction"; }
  void SetFiltered(size_t id);
};

class AFilterPipe : public AObjectFilter {
  std::vector<AObjectFilter*> mFilters;
public:
  void AddFilter(AObjectFilter* filter) {
    mFilters.push_back(filter);
  }

  AFilterPipe() { mName = "Pipe"; }
  ~AFilterPipe();

  void Check(APhysicObjectBase*& obj);
  void Save(libconfig::Setting& root);
};


#endif // AOBJECTFILTER_HPP
