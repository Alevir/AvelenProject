#include "ALocationNS.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

ALocationNS::ALocationNS(std::string iLocationFile, AWorldBase* iWorld)
      : ALocationBase(iLocationFile, iWorld) {

  if(!resourcesLoaded) {
    loadGroundCells("../resources/groundCells.xml");
    resourcesLoaded = true;
  }
  loadLocationFromXML(iLocationFile);
}



void ALocationNS::Display() {

}

void ALocationNS::SendCreationMessages(sf::TcpSocket& sock) {
  ANetPackage p;
  for(APhysicObjectBase* obj : localObjects) {
    p.AddMessage(new APObjectCreation(obj->templateName, obj->GetX(), obj->GetY(), obj->GetAngle(), obj->GetZ(), obj->GetID()));
  }
  for(ACharacterBase* ch : localCharacters) {
    p.AddMessage(new ACharacterCreation(obj->templateName, obj->GetX(), obj->GetY(), obj->GetAngle(), obj->GetZ(), obj->GetID()));
  }
  p.Pack();
  p.Send(sock);
}

void ALocationNS::loadGroundCells(const std::string& iLocationFileName) {
  std::ifstream file(iLocationFileName);
  boost::property_tree::ptree root;
  boost::property_tree::read_xml(file, root);
  boost::property_tree::ptree::iterator it = root.begin();
  while(it != root.end()) {
    std::string name = it->second.get<std::string>("<xmlattr>.name");
    double linDamping = it->second.get<double>("<xmlattr>.linearDamping");
    ASprite* spr = 0;
    AGroundCell* gc = new AGroundCell(name, spr, linDamping);
    scriptGroundContainer.insert(std::pair<std::string, AGroundCell*> (name, gc));
    saveGroundMap.insert(std::pair<AGroundCell*, unsigned int>(gc, loadGroundMap.size()));
    loadGroundMap.push_back(gc);
    it++;
  }
  file.close();
}
