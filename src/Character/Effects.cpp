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
#include "Effects.hpp"
#include "Global.h"
#include "ASpriteContainer.h"
#include "ADebugOutput.h"
#include "ACharacterBase.h"



RepeatingEffect::RepeatingEffect(double duration, double period, const std::string &effectName, ACharacterBase *ch)
: RepeatingAction(duration, period), Effect(effectName, ch){

}


EfRepValueChange::EfRepValueChange(double duration, double period, const std::string &effectName,
 ACharacterBase *ch, double& changedValue, double measure)
    : RepeatingEffect(duration, period, effectName, ch), _value(changedValue), _measure(measure) {

}

/*EfRepValueChange::operator EffectData() {
  EffectData ed;
  ed.Duration = _duration;
  ed.Measure = _measure;
  ed.Name = EffectName;
  ed.Period = Period;
  return ed;
}*/

bool EfRepValueChange::Add(const EffectData &ed) {
  if(EffectName != ed.Name) return false;
  int acts = mDuration / Period;
  double totalVal = ed.Duration / ed.Period * ed.Measure;
  _measure += totalVal / 2.0 / (double)acts;

  dout << _measure;
  mDuration += totalVal / 2 / _measure * Period;
  return true;
}


void EfRepValueChange::Activate() {
  _value += _measure;
}

void EfRepValueChange::Write(libconfig::Setting &group) {
  using namespace libconfig;
  group.add("name", Setting::TypeString) = EffectName;
  group.add("dur", Setting::TypeFloat) = mDuration;
  group.add("per", Setting::TypeFloat) = Period;
  group.add("measure", Setting::TypeFloat) = _measure;
}



Effect* Effect::CreateEffect(const EffectData& ed, ACharacterBase* ch) {
  if(ed.Name == "PeriodicHPRestore") {
    return new EfRepValueChange(ed.Duration, ed.Period, ed.Name, ch, ch->mHP, ed.Measure);
  } else
  if(ed.Name == "PeriodicEPRestore") {
    return new EfRepValueChange(ed.Duration, ed.Period, ed.Name, ch, ch->mEP, ed.Measure);
  } else
    throw std::logic_error("unknown effect");
}

Effect::Effect(const std::string& effectName, ACharacterBase* ch) : EffectName(effectName) {
  _ch = ch;
}

//Effect::operator EffectData() { return EffectData; }

EffectData::EffectData(libconfig::Setting &s) {
s.lookupValue("name", Name);
  s.lookupValue("per", Period);
  s.lookupValue("dur", Duration);
  s.lookupValue("measure", Measure);
}


void DisposableEffect::Step(double dt) {
  DisposableAction::Step(dt);
}
