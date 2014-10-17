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

#ifndef AANIMATION_HPP
#define AANIMATION_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include "ATextureContainer.h"
#include <set>
#include "ASprite.h"

class AAnimationContainer;

class AAnimation {
  friend class AAnimationContainer;
  double length = 1.0;
  double curTime = 0.0;

  std::shared_ptr<std::vector<ASprite> > _sprites;

public:
  AAnimation();
  //AAnimation(const AAnimation& a);
  ASprite* Step(double dt);
  void SetTime(double t);
  void SetLength(double l);
  double GetLength();
  void Reset();
};

class AAnimationContainer {
  ATextureContainer TC;
  std::map<std::string, AAnimation> _loaded;
public:
  const AAnimation& GetAnimation(const std::string& name);
  void LoadAnimations(const char* dirPath);
  void GetAnimationsNames(std::set<std::string>& set);
};

#endif // AANIMATION_HPP
