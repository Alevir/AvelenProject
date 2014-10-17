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

#ifndef ASPRITECONTAINER_H
#define ASPRITECONTAINER_H


#include <map>
#include <set>
#include <stdexcept>
#include "ASprite.h"


class ASpriteContainer {
  std::map<std::string, ASprite*> _spriteContainer;
  std::map<std::string, ASpriteData> _dataContainer;
  ATextureContainer* TC;
public:

  class exSpriteAddition : public std::logic_error {
  public:
    exSpriteAddition(std::string msg) : std::logic_error(msg) {

    }
  };

  ASpriteContainer(ATextureContainer* ipTC);
  //ASpriteContainer(ATextureContainer* ipTC);
  ASprite* GetSprite(std::string iSpriteName);
  ASprite* AddSprite(const ASpriteData* ipData) throw (exSpriteAddition);
  void AddContent(const std::string& path);

  ~ASpriteContainer();
  void GetSpritesNames(std::set<std::string>& s);

} ;

#endif // ASPRITECONTAINER_H
