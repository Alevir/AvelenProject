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
  std::set<unsigned int> mUnused;
  unsigned int mSupValue = 0;
  std::vector<T*> mContainer;

public:

  void Clear() {
    mSupValue = 0;
    mUnused.clear();
    mContainer.clear();
  }

  unsigned int AddObject(T* obj);


  T* GetObjectByID(unsigned int id) {
    return mContainer.at(id);
  }

  void AddPair(unsigned int id, T* obj);

  void ReleaseID(unsigned int id);

  unsigned int GetMinNeverUsedID() {
    return mSupValue;
  }
};




typedef long long unsigned int UniqueID;

class UniqueIDManager {

  struct IDRange {
    mutable UniqueID Lower;
    mutable UniqueID Upper;

    bool operator < (const IDRange& str) const {
      return Lower < str.Lower;
    }

    IDRange(UniqueID lower, UniqueID upper) : Lower(lower), Upper(upper) {
    }

    IDRange(UniqueID value) : Lower(value), Upper(value) {
    }
  };

  UniqueID mTop = 0;
  std::set<IDRange> mUsed;

public:
  void AddValue (UniqueID id) {
    if(mUsed.empty()) {
      mUsed.insert(IDRange(id));
      return;
    }
    std::set<IDRange>::iterator it = mUsed.lower_bound(id);
    if(it == mUsed.end()) {
      --it;
      if(id <= it->Upper) {
        throw std::logic_error("id " + std::to_string(id) + " is already used");
      }
      if(id == it->Upper + 1) {
        ++(it->Upper);
      } else {
        mUsed.insert(id);
      }
      mTop = id;
      return;
    }

    if(it == mUsed.begin()) {
      if(id == it->Lower) {
        throw std::logic_error("id " + std::to_string(id) + " is already used");
      }
      if(id == it->Lower - 1) {
        --(it->Lower);
      } else {
        mUsed.insert(id);
      }
      return;
    }

    auto prev = it;
    --prev;
    if(id == it->Lower || id <= prev->Upper) {
      throw std::logic_error("id " + std::to_string(id) + " is already used");
    }
    if(id == it->Lower - 1) {
      if(id == prev->Upper + 1) {
        prev->Upper = it->Upper;
        mUsed.erase(it);
      } else {
        --(it->Lower);
      }
    } else
    if(id == prev->Upper + 1){
      ++(prev->Upper);
    } else {
      mUsed.insert(id);
    }
  }

  UniqueID GetNewID() {
    if(mUsed.empty()) {
      mUsed.insert(++mTop);
      return mTop;
    }
    if(mTop == (--mUsed.end())->Upper) {
      return mTop = (++((--mUsed.end())->Upper));
    }
    mUsed.insert(++mTop);
    return mTop;
  }

  UniqueID GetTop() { return mTop; }
  void SetTop(UniqueID top) {
    assert(top >= mTop);
    mTop = top;
  }

  void Clear() {
    mUsed.clear();
    mTop = 0;
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
  if(!mUnused.empty()) {
    id = *mUnused.begin();
    mUnused.erase(mUnused.begin());
    mContainer.at(id) = obj;
  } else {
    mContainer.push_back(obj);
    id = mSupValue++;

  }
  return id;
}


template<class T>
void AIDManager<T>::AddPair(unsigned int id, T* obj) {
  if(mContainer.size() > id) {
    if(mContainer[id] || (mUnused.find(id) == mUnused.end())) throw std::logic_error("id is already used");
    mContainer[id] = obj;
    mUnused.erase(mUnused.find(id));
  } else {
    for(int i = mContainer.size(); i < id; ++i) {
      mUnused.insert(i);
    }

    mContainer.resize(id + 1, 0);
    mContainer.back() = obj;
    mSupValue = mContainer.size();
  }
}



template<class T>
void AIDManager<T>::ReleaseID(unsigned int id) {
  if(id == mSupValue-1) {
    mSupValue--;
    mContainer.pop_back();
    return;
  }
  mUnused.insert(id);
  mContainer.at(id) = 0;
}






#endif // AIDMANAGER_H
