#ifndef ANETPACKAGE_H
#define ANETPACKAGE_H
#include "ANetMessage.h"
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>
#include "SFML/Network/TcpSocket.hpp"
#include "SFML/Network/UdpSocket.hpp"


#define   ADD_MESSAGE_TYPE(TYPE) (pParts.insert(std::pair<const std::type_info*, IPackagePart*> (&typeid(TYPE), new PackagePart<TYPE>())))




class ANetPackage {

public:

    static const size_t MaxPackageSize = 10000;

private:

  struct IPackagePart {
    const size_t mesSize;
    std::vector<void*> messages;
    const AMessageType enumType;
    virtual ~IPackagePart() {}

    IPackagePart(size_t mesSize, AMessageType enumType);

    void AddMessage(void* message) {
      messages.push_back(message);
    }
  };

  template<class mesType> 
  struct PackagePart : public IPackagePart {
    PackagePart();
  };

  std::map<const std::type_info*, IPackagePart*> pParts;
  size_t PackageSize = 0;
  sf::Uint8 data[MaxPackageSize];

public:

  template<class mesType>
  void AddMessage(mesType* message) {
    IPackagePart* part = pParts[&typeid(mesType)];
    part->AddMessage(static_cast<void*>(message));
  }

  ANetPackage();
  void Pack();
  void Clear();
  void Send(sf::TcpSocket &sock);
  ~ANetPackage();
};



template<class mesType>
ANetPackage::PackagePart<mesType>::PackagePart()
    : IPackagePart(sizeof(mesType), mesType::enumType) {
}


class ANetSingleMessage {
public:
  static const size_t MaxPackageSize = 1000;

private:

  sf::Uint8 data[MaxPackageSize];
  size_t size;

public:

  template<class mesType>
  void SetMessage(mesType* message) {
    data[0] = 0;
    data[1] = (sf::Uint8)mesType::enumType;
    sf::Uint8* bytePtr = static_cast<sf::Uint8*>(static_cast<void*>(message));
    int limit = ANetMessage::MessagesSizes[(sf::Uint8)mesType::enumType];
    for(int i = 0; i < limit; i++) {
      data[i+2] = bytePtr[i];
    }
    size = limit + 2;
  }

  void Send(sf::TcpSocket &sock) {
    sock.send(data, size);
  }
};


#endif // ANETPACKAGE_H
