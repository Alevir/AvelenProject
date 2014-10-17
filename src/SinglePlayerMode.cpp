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

#include "SinglePlayerMode.hpp"
#include <string>
#include "boost/thread.hpp"
#include "ADebugOutput.h"

#include "GUI/GInGame.hpp"
#include "Effects.hpp"
#include "GUI/GPicture.hpp"
#include "AScriptWrapper.hpp"
#include "AObjectFilter.hpp"
#include "GUI/GInventory.h"
#include "SFML/Audio.hpp"





void SinglePlayerMode::ResetPlayer() {
  ACharacterBase* player = world.GetPlayer();
  GCS.SetPlayer(player);
  world.Camera.BindToObj(dynamic_cast<APhysicObjectSFML*>(player));
  Interface.SetPlayer(player);
  plInv = GCS.GetPlayerInventory();
  //world.GetPlayer()->SwitchController();
}

SinglePlayerMode::SinglePlayerMode() : GCS(control, world.GetPlayer()),
Interface(world.GetPlayer(), control) {
  debug.Switch();
  GCS.TransferDistance = 1.0;
  window->setMouseCursorVisible(false);

  world.Camera.BindToObj(dynamic_cast<APhysicObjectSFML*>(world.GetPlayer()));
  world.Interface = &Interface;
  world.GetPlayer()->SwitchController();
  plInv = GCS.GetPlayerInventory();
  plInv->SetPosition(20, Game::Window->getSize().y / 3 - 50);
}

SinglePlayerMode::~SinglePlayerMode() {
}


void SinglePlayerMode::_run() {
  dout << "hello!\n";
  sf::Music wind;
  wind.openFromFile("../resources/sounds/strangeWind.wav");
  wind.play();
  wind.setLoop(true);
  wind.setPosition(0, 0, 0);


  FocusRayCast frc(this);
  GSaveWindow saveW(control, &world);
  GLoadWindow loadW(control, &world, this);




  while(window->isOpen()) {
    sf::Event event;
    if(saveW.IsVisible()) {
      world.OneStepPause();
      while(window->pollEvent(event)) {
        if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
          window->close();
          continue;
        } else
          control.HandleEvent(event);
      }
    } else {
      while(window->pollEvent(event)) {
        if(event.type == sf::Event::KeyPressed) {
          if(event.key.code == sf::Keyboard::Escape) {
            window->close();
            break;
          } else
          if(event.key.code == sf::Keyboard::F4) {
            control.Show(&saveW);
            break;
          } else
          if(event.key.code == sf::Keyboard::F5) {
            world.Save("../testSave/quicksave.save");
            world.Interface->AddMessage(L"quicksave", 2);
          } else
          if(event.key.code == sf::Keyboard::F8) {
            control.Show(&loadW);
          } else
          if (event.key.code == sf::Keyboard::Space) {
            frc.ChooseNextObject();
          } else
          if (event.key.code == sf::Keyboard::Q) {
            auto obj = frc.GetFocusedObject();
            if(obj) {
              Interface.AddMessage(obj->GetLabel(), 5.0);
              obj->ExecuteScript(APhysicObjectBase::SN_View, world.GetPlayer());
            }
          } else
          if (event.key.code == sf::Keyboard::E) {
            auto obj = frc.GetFocusedObject();
            if(obj) {
              obj->ExecuteScript(APhysicObjectBase::SN_Pick, world.GetPlayer());

              if(obj->GetTemplateData()->b2type == b2_dynamicBody) {
                obj->ExecuteScript(APhysicObjectBase::SN_Pick, world.GetPlayer());
                if(obj->GetTemplateData()->containerCapacity > 0 && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                  GCS.AddContainer(obj);
                  control.Show(plInv);

                } else if(! dynamic_cast<ACharacterBase*>(obj)) {
                  world.GetPlayer()->AddObjectToBag(obj);
                  plInv->Refresh();
                }
              }
            }
          } else
          if (event.key.code == sf::Keyboard::Pause) {
            assert(0);
          } else
          if (event.key.code == sf::Keyboard::P) {
            debug.Switch();
          } else
          if (event.key.code == sf::Keyboard::I) {
            /*window->setMouseCursorVisible(!world.GetPlayer()->SwitchController());
            guiHandles = !guiHandles;
            plInv->SwitchVisibility();
            if(!guiHandles) {
              GCS.CloseAll();
            }*/
          } else {
            control.HandleEvent(event);
          }
        } else
        if(event.type == sf::Event::Closed) {
            window->close();
            break;
        } else
        if (event.type == sf::Event::Resized) {
          sf::View v = window->getView();
          v.setSize(event.size.width, event.size.height);
          v.zoom(1.0);
          window->setView(v);
          continue;
        } else
        if ((event.type == sf::Event::MouseButtonReleased) && (event.key.code == sf::Mouse::Left)) {
          control.HandleEvent(event);
          try {
            GCS.CheckAndDrop();
          } catch(GContainerSystem::ExLongDistance& e) {
            dout << "container is too far";
            dout << "\n";
          }
        } else {
          control.HandleEvent(event);
        }
      }
    }

    tgui::Callback callback;
    while (control.Gui.pollCallback(callback)) {

    }



    window->clear();
    //try {

    /*} catch (std::logic_error& e) {
      std::cout << "EXCEPTION: " << e.what() << std::endl;
    }*/


    frc.Step();
    Interface.SetFocusedObject(frc.GetFocusedObject());

    world.LogicStep();
    //world.Camera.Step();
    world.GraphicStep();


    control.Gui.draw();

    window->display();
  }
  //world.Save("../testSave/test.save");
  //world.Save("../testSave/");
}


float32 SinglePlayerMode::FocusRayCast::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float32 fraction) {
  foundObjects.insert(static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData()));
  return fraction;
}

void SinglePlayerMode::FocusRayCast::Step() {
  foundObjects.clear();
  ACharacterBase* character = mode->world.GetPlayer();
  AVector2 near(0.0, 0.1);
  near.Rotate(character->GetAngle());
  AVector2 far(0.0, 0.5);
  far.Rotate(character->GetAngle());
  far = far + character->GetPosition();
  near = near + character->GetPosition();
  mode->world.RayCast(this, near, far);
  if(foundObjects.empty()) {
    focusedObject = 0;
    return;
  }
  auto it = foundObjects.find(focusedObject);
  if(it == foundObjects.end()) {
    focusedObject = *foundObjects.begin();
  }
  dynamic_cast<APhysicObjectSFML*>(focusedObject)->SetFlickering(true);
}

void SinglePlayerMode::FocusRayCast::ChooseNextObject() {
  if(foundObjects.size() < 2) return;
  auto it = foundObjects.find(focusedObject);
  if(++it == foundObjects.end()) {
    --(--it);
  }
  focusedObject = *it;
}

APhysicObjectBase *SinglePlayerMode::FocusRayCast::GetFocusedObject() {
  return focusedObject;
}
