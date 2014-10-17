#ifndef ALOCATIONSFML_H
#define ALOCATIONSFML_H
#include "ALocationBase.h"
#include "ANetPackage.h"

class ALocationNS : public ALocationBase {
  void loadGroundCells(const std::string& iLocationFileName);
public:
  ALocationNS(std::string iLocationFile, AWorldBase* iWorld);
  void SendCreationMessages(sf::TcpSocket& sock);
  void Display();
};

#endif // ALOCATIONSFML_H
