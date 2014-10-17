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

#ifndef EDITORMODE_H
#define EDITORMODE_H
#include "Mode.h"
#include "GUI/GEditor.h"
#include "GUI/GContainerSystem.h"

class EditorMode : public WorldMode {
  GInfoManager<APhysicObjectBase, GObjectInfoWindow> GOIM;
  GInfoManager<ACharacterBase, GCharacterInfoWindow> GCIM;




  class InfoQueryCallback : public b2QueryCallback{
    EditorMode* _mode;
  public:
    bool ObjFound = false;
    bool ReportFixture(b2Fixture *fixture) {
      _mode->GOIM.ShowObjectInfo(static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData()));
      ObjFound = true;
      return false;
    }
    InfoQueryCallback(EditorMode* mode) : _mode(mode) { }
  };


  class ContainerInfoQC : public b2QueryCallback {
    GContainerSystem* _GCS;
  public:
    ContainerInfoQC(GContainerSystem* gcs) : _GCS(gcs) { }
    bool ObjFound = false;
    bool ReportFixture(b2Fixture *fixture) {
      _GCS->AddContainer(static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData()));
      return false;
    }
  };

  class ObjectMenuQC : public b2QueryCallback {
    GRightClickObjectMenu* _OM;



  public:
    ObjectMenuQC(GRightClickObjectMenu* OM) : _OM(OM) { }
    bool ReportFixture(b2Fixture *fixture) {
      _OM->SetObject(static_cast<APhysicObjectBase*>(fixture->GetBody()->GetUserData()));
      _OM->Show();
      return false;
    }
  };

  enum mode {
    M_Main = 0,
    M_Creation,
    M_Removal
  };


public:
  void ResetPlayer() {}
  EditorMode();
  void _run();
  ~EditorMode();

};

#endif // EDITORMODE_H
