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

#include "EditorMode.h"
#include "ADebugOutput.h"
#include "GUI/GScriptEditor.hpp"




EditorMode::EditorMode()
: GOIM(&world, control), GCIM(&world, control) {
}




void EditorMode::_run() {
  /*ALocationSFML* loc = dynamic_cast<ALocationSFML*>(world.GetCurrentLocation());
  APhysicObjectBase* obj = loc->AddObject("TestObject1", 0, 0, 0, 2.0);*/
  //_world.GetPlayer()->SwitchController();
  //RaceEditor re(gui);
  GScriptEditor scrEd(control);
  GObjectTemplateEditor te(control);
  GRightClickMenu rc(control, &world);
  GLocationCreation locCr(control, &world);
  GEditorContainerSystem GCS(control);
  InfoQueryCallback qc(this);
  ContainerInfoQC contQC(&GCS);
  GRightClickObjectMenu _objMenu(control, &world, &GCS, &GOIM, &GCIM);
  _objMenu.ScriptEditor = &scrEd;
  ObjectMenuQC OMQC(&_objMenu);
  b2AABB aabb;
  double e = 0.02;
  //_world.Camera.BindToObj(dynamic_cast<APhysicObjectSFML*>(_world.GetPlayer()));
  //_world.Camera.Unbind();
  ATransform tr;  world.GetPlayer()->GetTransform(tr);
  tr.A = 0;
  world.Camera.SetTransform(tr);
  while(window->isOpen()) {
    sf::Event event;
    while(window->pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
          window->close();
          break;
      } else
      if (event.type == sf::Event::Resized) {
        sf::View v = window->getView();
        v.setSize(event.size.width, event.size.height);
        window->setView(v);
      } else
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
        window->close();
        break;
      } else
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)) {
        debug.Switch();
      } else
      if(rc.IsCreationMode()) {
        control.Gui.handleEvent(event);
        if(event.key.code == sf::Mouse::Left && sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
          if(event.type == sf::Event::MouseButtonPressed) {
            rc.ObjCreation.Begin();
          } else
          if(event.type == sf::Event::MouseButtonReleased) {
            rc.ObjCreation.End();
          }
        }
      } else {
        if ((event.type == sf::Event::MouseButtonReleased) && (event.key.code == sf::Mouse::Right)) {
          if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            if(!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
              AVector2 mpos = AWorldSFML::ConvertMouseCurrentPos();
              aabb.lowerBound.Set(mpos.x - e, mpos.y - e);
              aabb.upperBound.Set(mpos.x + e, mpos.y + e);
              world.QueryAABB(&qc, aabb);
              if(!qc.ObjFound) {
                rc.Show();
              } else {
                qc.ObjFound = false;
              }
            } else {
              rc.Show();
            }
          } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
            AVector2 mpos = AWorldSFML::ConvertMouseCurrentPos();
            aabb.lowerBound.Set(mpos.x - e, mpos.y - e);
            aabb.upperBound.Set(mpos.x + e, mpos.y + e);
            world.QueryAABB(&contQC, aabb);
            if(contQC.ObjFound) {
              contQC.ObjFound = !contQC.ObjFound;
            }
          } else {
            AVector2 mpos = AWorldSFML::ConvertMouseCurrentPos();
            aabb.lowerBound.Set(mpos.x - e, mpos.y - e);
            aabb.upperBound.Set(mpos.x + e, mpos.y + e);
            world.QueryAABB(&OMQC, aabb);
            //gui.handleEvent(event);
          }
        } else
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::L)
              && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
          control.Show(&locCr);
          dout << "loc cr";
        } else
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::T)
        && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
          te.SwitchVisibility();
        } else
        if ((event.type == sf::Event::MouseButtonPressed) && (event.key.code == sf::Mouse::Left)) {
          control.Gui.handleEvent(event);
          rc.Hide();
          _objMenu.Hide();
        } else
        if ((event.type == sf::Event::MouseButtonReleased) && (event.key.code == sf::Mouse::Left)) {
          control.Gui.handleEvent(event);
          GCS.CheckAndDrop();
        } else {
          control.Gui.handleEvent(event);
        }
      }
    }

    auto mpos = sf::Mouse::getPosition(*Game::Window);
    auto wsize = Game::Window->getSize();
    const double dShift = 0.12;
    AVector2 shift(0);
    if(mpos.x <= 1) {
      shift.x = -dShift;
    } else
    if(mpos.x > wsize.x - 2) {
      shift.x = dShift;
    }

    if(mpos.y <= 1) {
      shift.y = -dShift;
    } else
    if(mpos.y > wsize.y - 2) {
      shift.y = dShift;
    }

    if(shift.x != 0 || shift.y != 0)
      world.Camera.Move(shift);

    window->clear();
    world.LogicStep();

    world.Camera.Step();
    world.GraphicStep();
    control.Gui.draw();

    window->display();
  }

  world.Save("../testSave/editor.save");
}


EditorMode::~EditorMode() {
}
