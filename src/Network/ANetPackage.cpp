#include "ANetPackage.h"
#include <iostream>

ANetPackage::ANetPackage() {
  ADD_MESSAGE_TYPE(ATransformMessage);
  ADD_MESSAGE_TYPE(ATextMessage);
  ADD_MESSAGE_TYPE(AHelloMessage);
  ADD_MESSAGE_TYPE(AGoodbyeMessage);
}

void ANetPackage::Pack() {

  size_t numParts = 0;
  for(std::pair<const std::type_info*, IPackagePart*> p : pParts) {
    if(p.second->messages.size() > 0) numParts++;
  }
  data[0] = static_cast<sf::Uint8>(numParts);

  size_t headerPoint = 1;
  size_t dataPoint = 1 + numParts * 3;
  for(std::pair<const std::type_info*, IPackagePart*> p : pParts) {
    if(p.second->messages.size() == 0) continue;
    IPackagePart* part = p.second;

    data[headerPoint] = static_cast<sf::Uint8>(part->enumType);
    sf::Uint16 s = part->messages.size();
    sf::Uint8* bytePtr = static_cast<sf::Uint8*>((void*)(&s));
    data[headerPoint+1] = bytePtr[0];
    data[headerPoint+2] = bytePtr[1];
    headerPoint += 3;

    if(dataPoint >= 10000) {
      throw std::logic_error("NetPackage is too large");
    }
    for(int k = 0; k < part->messages.size(); k++) {
      bytePtr = static_cast<sf::Uint8*>(part->messages[k]);
      for(int j = 0; j < part->mesSize; j++) {
        data[dataPoint] = bytePtr[j];
        dataPoint++;
      }
    }
  }
  PackageSize = dataPoint;

}

ANetPackage::IPackagePart::IPackagePart(size_t size, AMessageType enumType)
    : mesSize(size), enumType(enumType) {
}


void ANetPackage::Send(sf::TcpSocket& sock) {
  sock.send(data, PackageSize);
}


void ANetPackage::Clear() {
  for(std::pair<const std::type_info*, IPackagePart*> p : pParts) {
    for(void* m : p.second->messages) {
      delete m;
    }
    p.second->messages.clear();
  }
  PackageSize = 0;
}

ANetPackage::~ANetPackage() {
  Clear();
  for(std::pair<const std::type_info*, IPackagePart*> p : pParts) {
    delete p.second;
  }
}
