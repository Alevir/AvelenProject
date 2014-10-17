#ifndef AWORLDSFML_H
#define AWORLDSFML_H

#include "AWorldBase.h"
#include "ACharacterNS.h"
#include <map>
#include <set>

struct Client {
  sf::TcpSocket socket;
  ANetPackage package;
};

class ALocationNS;


class AWorldNS
    : public AWorldBase {
  friend class ALocationNS;

  virtual void _graphicStep(double dt);
  void ParseObjectsFile(std::string iFilepath);
  APhysicObjectNS* CreateObject(std::string &iTemplateName, double iX, double iY, double iAngle, double iZ);
  ACharacterNS* CreateCharacter(std::string& iTemplateName, double iX, double iY, double iAngle, double iZ);
  virtual void RemoveObject(APhysicObjectBase* obj);
  std::set<APhysicObjectBase*> drawContainer;
public:
  std::vector<Client> clients;
  AWorldNS();

};

#endif // AWORLDSFML_H
