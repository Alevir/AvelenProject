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

    Author of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#ifndef GCONTAINER_H
#define GCONTAINER_H

#include "SFML/Graphics.hpp"

#include "TGUI/TGUI.hpp"
#include "GCommon.h"

class GContainerSystem;
class ACharacterBase;
class APhysicObjectBase;

class GContainer : public GuiWindowBase {
  friend class GContainerSystem;
  friend class GInGameContainerSystem;
  friend class GEditorContainerSystem;
protected:
  APhysicObjectBase* _obj;

  GContainerSystem* csystem;


  class containerWindow : public tgui::ChildWindow {
    GContainer* _cont = 0;

  public:
    typedef tgui::SharedWidgetPtr<containerWindow> Ptr;
    void Init(GContainer* c) {
      _cont = c;
    }

    void leftMouseReleased(float x, float y);
  };

  containerWindow::Ptr mWindow;
  tgui::ListBox::Ptr objList;
  std::vector<APhysicObjectBase*> _content;
  tgui::Label::Ptr lWeight;
  tgui::LoadingBar::Ptr freeSpace;

  virtual void _onClose();
  void _pickObject();
  virtual void _putObject();


  double totalWeight = 0.0;


public:
  void SwitchVisibility();

  void Hide() {
    Gui.OnHideOrClose(this);
    mWindow->hide();
  }

  void Show() {
    mWindow->show();
  }

  APhysicObjectBase* GetObject() { return _obj; }
  GContainer(APhysicObjectBase* obj, GGUI& iGui, GContainerSystem *iCs);
  void Refresh();
  void SetPosition(double x, double y);

  virtual ~GContainer() {
    mWindow->destroy();
  }

  //exceptions


};

#endif // GCONTAINER_H
