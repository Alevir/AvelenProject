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


#ifndef ADEBUGOUTPUT_H
#define ADEBUGOUTPUT_H
#include <stack>
#include <sstream>
#include <iostream>
class GGUI;

#include "TGUI/TGUI.hpp"
class ADebugOutput : public std::wostream {
  std::wstringstream mSstr;
  tgui::TextBox::Ptr mTBox;
public:
  ADebugOutput(GGUI& gui);
  void Switch() {
    bool v = !mTBox->isVisible();
    v ? mTBox->show() : mTBox->hide();
  }

  template <class T>
  ADebugOutput& operator << (const T& info) {
    mSstr << info;

    mTBox->setText(mSstr.rdbuf()->str());
    return *this;
  }

};
//#else
//#ifdef _AVELEN_SERVER_


//#endif
//#endif




template <class T>
class AStackPointer {
protected:
  std::stack<T*> mStack;
public:
  AStackPointer() {}
  void Pop() {
    mStack.pop();
  }

  void Push(T* value) {
    mStack.push(value);
  }

  T& operator*() {
    return *(mStack.top());
  }
  T* operator -> () {
    return mStack.top();
  }
};

class ADebugInfoStackPointer
    : public AStackPointer<ADebugOutput> {
public:
  template <class T>
  std::wostream& operator << (const T& info) {
    if(mStack.empty()) {
      std::wcout << info;
      return std::wcout;
    }
    (operator *()).operator <<(info);
    return operator*();
  }
};

extern ADebugInfoStackPointer dout;

#endif // ADEBUGOUTPUT_H
