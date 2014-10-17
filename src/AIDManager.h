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

#ifndef AIDMANAGER_H
#define AIDMANAGER_H
#include <set>
#include <vector>
#include <stdexcept>
template<class T>
class AIDManager {
  std::set<unsigned int> unused;
  unsigned int supValue = 0;
  std::vector<T*> mContainer;

public:

  void Clear() {
    supValue = 0;
    unused.clear();
    mContainer.clear();
  }

  unsigned int AddObject(T* obj);


  T* GetObjectByID(unsigned int id) {
    return mContainer.at(id);
  }

  void AddPair(unsigned int id, T* obj);

  void ReleaseID(unsigned int id);

  unsigned int GetMinNeverUsedID() {
    return supValue;
  }
};



class AIDManagerReduced {
  std::set<unsigned int> unused;
  unsigned int supValue = 0;

public:
  void Clear() {
    unused.clear();
    supValue = 0;
  }

  void AddLoadedID(unsigned int id) {
    if(supValue > id) {
      if(unused.find(id) == unused.end()) throw std::logic_error("id is already used");
      unused.erase(unused.find(id));
    } else {
      for(; supValue <= id; ++supValue) {
        unused.insert(supValue);
      }
    }
  }

  void ReleaseID(unsigned int id) {
    if(id == supValue-1) {
      supValue--;

      return;
    }
    unused.insert(id);
  }

  unsigned int GetNewID() {
    unsigned int id;
    if(!unused.empty()) {
      id = *unused.begin();
      unused.erase(unused.begin());
    } else {
      id = supValue++;
    }
    return id;
  }

};

template<class T>
unsigned int AIDManager<T>::AddObject(T* obj) {
  unsigned int id;
  if(!unused.empty()) {
    id = *unused.begin();
    unused.erase(unused.begin());
    mContainer.at(id) = obj;
  } else {
    mContainer.push_back(obj);
    id = supValue++;

  }
  return id;
}


template<class T>
void AIDManager<T>::AddPair(unsigned int id, T* obj) {
  if(mContainer.size() > id) {
    if(mContainer[id] || (unused.find(id) == unused.end())) throw std::logic_error("id is already used");
    mContainer[id] = obj;
    unused.erase(unused.find(id));
  } else {
    for(int i = mContainer.size(); i < id; ++i) {
      unused.insert(i);
    }

    mContainer.resize(id + 1, 0);
    mContainer.back() = obj;
    supValue = mContainer.size();
  }
}



template<class T>
void AIDManager<T>::ReleaseID(unsigned int id) {
  if(id == supValue-1) {
    supValue--;
    mContainer.pop_back();
    return;
  }
  unused.insert(id);
  mContainer.at(id) = 0;
}






#endif // AIDMANAGER_H
