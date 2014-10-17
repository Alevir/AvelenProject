#include "ANetMessage.h"
#include <iostream>


APObjectCreation::APObjectCreation(std::string iTemplateName, double iX, double iY, double iAngle, double iZ, unsigned int iId) {
  size_t end = iTemplateName.size() < 64 ? iTemplateName.size() : 63;
  for(int i = 0; i < end; i++) {
    TemplateName[i] = iTemplateName[i];
  }
  X = iX;
  Z = iZ;
  Y = iY;
  Angle = iAngle;
  id = iId;
}

size_t ANetMessage::MessagesSizes[] = {
  sizeof(ATransformMessage),
  sizeof(ATextMessage),
  sizeof(AHelloMessage),
  sizeof(AStartMessage),
  sizeof(AGoodbyeMessage),
  sizeof(APObjectCreation),
  sizeof(ACharacterCreation),
  sizeof(ACharacterStatus),
  sizeof(AAddEffect),
  sizeof(AControllerMessage)
};


void TranslationHandler(void* message) {
  ATransformMessage* tm = static_cast<ATransformMessage*>(message);

  std::cout << "MT_Transform \tid: " << tm->id << "\tx: " << tm->x
       << "\ty: " << tm->y << "\ta: " << tm->a << std::endl;

}

void TextMessageHandler(void* message) {
  ATextMessage* tm = static_cast<ATextMessage*>(message);
  std::cout << "MT_TextMessage \ttext: " << std::string(tm->text) <<std::endl;
}
/*

void Handler(void* message) {

}

*/

void HelloHandler(void* message) {

}

void GoodbyeHandler(void* message) {

}

void StartHandler(void* message) {

}

void PObjectCreationHandler(void* message) {

}

void CharacterCreationHandler(void* message) {

}

void CharacterStatusHandler(void* message) {

}

void AddEffectHandler(void* message) {

}

void ControllerMessageHandler(void* message) {

}


void (*ANetMessage::handlers[])(void*) = {
  TranslationHandler,
  TextMessageHandler,
  HelloHandler,
  StartHandler,
  GoodbyeHandler,
  PObjectCreationHandler,
  CharacterCreationHandler,
  CharacterStatusHandler,
  AddEffectHandler,
  ControllerMessageHandler,
};


ATextMessage::ATextMessage(const std::string& str) {
  if(str.size() > 255) {
    for(int i = 0; i < 255; i++) {
      text[i] = str[i];
    }
  } else {
    for(int i = 0; i < str.size(); i++) {
      text[i] = str[i];
    }
  }
}


void ANetMessage::Handle(sf::TcpSocket& client) {
  sf::Uint8 data[1000] = { 0 };
  size_t recieved = 0;
  sf::Uint8 headerLen;
  client.receive(&headerLen, 1, recieved);
  if(recieved == 0) return;
  if(headerLen == 0) {
    client.receive(&headerLen, 1, recieved); //get type
    size_t tp = MessagesSizes[size_t(headerLen)];
    client.receive(data, tp, recieved);
    handlers[headerLen](static_cast<void*>(data));
  } else {
    sf::Uint8 header[headerLen * 3];
    client.receive(&header, headerLen*3, recieved);
    size_t hp = 0;
    sf::Uint8 MesType;
    size_t numMes = 0;
    for(int i = 0; i < headerLen; i++) {
      MesType = header[hp];
      hp++;
      numMes = *((sf::Uint16*)(header + hp));
      hp += 2;
      for(int j = 0; j < numMes; j++) {
        size_t tp = MessagesSizes[size_t(MesType)];
        client.receive(data, tp, recieved);
        handlers[MesType](static_cast<void*>(data));
      }
    }
  }
}
