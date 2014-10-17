#include "AWorldNS.h"
#include "ALocationNS.h"
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

AWorldNS::AWorldNS() {
  ParseObjectsFile("../resources/objects.xml");
  currentLoc = new ALocationNS("../resources/locations/testLocation.xml", this);
  currentLoc->Save("testLocSave");
}

void AWorldNS::_graphicStep(double dt) {
  //currentLoc->Display();

  for(APhysicObjectBase* p : drawContainer) {
    p->Display(dt);
  }
}




APhysicObjectNS* AWorldNS::CreateObject(std::string& iTemplateName, double iX, double iY, double iAngle, double iZ) {
  APhysicObjectData data = objectTemplates.find(iTemplateName)->second;
  APhysicObjectNS* obj = new APhysicObjectNS(data, iX, iY, iAngle, iZ, this);
  for(Client& c : clients) {
    ANetSingleMessage m;
    m.SetMessage(APObjectCreation(iTemplateName, iX, iY, iAngle, obj->GetID()));
    m.Send(c.socket);
  }
  drawContainer.insert(obj);
  return obj;
}


ACharacterNS* AWorldNS::CreateCharacter(std::string &iTemplateName, double iX, double iY, double iAngle, double iZ) {
  APhysicObjectData data = objectTemplates.find(iTemplateName)->second;
  ACharacterNS* ch = new ACharacterNS(data, iX, iY, iAngle, iZ, this);
  for(Client& c : clients) {
    ANetSingleMessage m;
    m.SetMessage(ACharacterCreation(iTemplateName, iX, iY, iAngle, obj->GetID()));
    m.Send(c.socket);
  }
  drawContainer.insert(ch);
  return ch;
}


void AWorldNS::ParseObjectsFile(std::string iFilepath) {
  std::ifstream objectFile(iFilepath);
  boost::property_tree::ptree objects;
  boost::property_tree::read_xml(objectFile, objects);
  objects = objects.get_child("objects");
  boost::property_tree::ptree::iterator it = objects.begin();
  while(it != objects.end()) {
    if(it->first == "object") {
      APhysicObjectData newTemplate(it->second);
      objectTemplates.insert(std::pair<std::string, APhysicObjectData>(newTemplate.templateName, newTemplate));
    }
    it++;
  }
  objectFile.close();
}


void AWorldNS::RemoveObject(APhysicObjectBase* obj) {
  auto it = drawContainer.find(obj);
  if(it != drawContainer.end()) {
    drawContainer.erase(it);
  }
}

