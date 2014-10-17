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

#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include "Action.h"
#include <libconfig.h++>
class ACharacterBase;


struct EffectData {
  std::string Name = "";
  double Duration = 0.0;
  double Measure = 0.0;
  double Period = 0.0;
  EffectData(libconfig::Setting& s);
  EffectData() {}

};



class Effect : public IAction {
protected:
  ACharacterBase* _ch;
public:
  static Effect* CreateEffect(const EffectData& ed, ACharacterBase* ch);
  virtual bool Add(const EffectData& ed) = 0;
  //virtual operator EffectData() = 0;
  const std::string EffectName;
  Effect( const std::string& effectName, ACharacterBase* ch);
  virtual void Write(libconfig::Setting& group) = 0;
  virtual ~Effect() {}
  //virtual operator EffectData();

};

class DisposableEffect : public DisposableAction, public Effect {
public:
  double& GetDuration() { return Action::GetDuration(); }
  void Step(double dt);
  DisposableEffect(double _duration, const std::string& effectName, ACharacterBase* ch);
};

class RepeatingEffect : public RepeatingAction, public Effect  {
public:
  double& GetDuration() { return Action::GetDuration(); }
  void Step(double dt) { RepeatingAction::Step(dt); }
  RepeatingEffect(double _duration, double period, const std::string& effectName, ACharacterBase* ch);

};

class EfRepValueChange : public RepeatingEffect {
  void Activate();
  double& _value;
  double _measure;
public:
  void Write(libconfig::Setting& group);
  bool Add(const EffectData &ed);
  EfRepValueChange(double _duration, double period, const std::string& effectName,
              ACharacterBase* ch, double &changedValue, double measure);
  //operator EffectData();
};


#endif // EFFECTS_HPP
