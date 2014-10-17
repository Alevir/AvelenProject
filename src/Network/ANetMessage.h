#ifndef ANETMESSAGE_H
#define ANETMESSAGE_H
#include "SFML/System.hpp"
#include "SFML/Network/TcpSocket.hpp"

enum class AMessageType : sf::Uint8 {
  MT_Transform,
  MT_TextMessage,
  MT_Hello,
  MT_Start,
  MT_Goodbye,
  MT_PObjectCreation,
  MT_CharacterCreation,
  MT_CharacterStatus,
  MT_AddEffect,
  MT_ControllerMessage
};

struct ANetMessage {
  virtual ~ANetMessage() {}
  static size_t MessagesSizes[];
  static void (*handlers[])(void*);
  static void Handle(sf::TcpSocket& client);
};

struct ATransformMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_Transform;
  sf::Uint32 id;
  double x;
  double y;
  double a;
};

struct ATextMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_TextMessage;
  char text[256] = { 0 };
  ATextMessage(const std::string& str);
};

struct AHelloMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_Hello;
};

struct AStartMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_Start;
  unsigned int id;
};

struct AGoodbyeMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_Goodbye;
};

struct APObjectCreation : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_PObjectCreation;
  char TemplateName[64] = {0};
  double X;
  double Y;
  double Angle;
  double Z;
  unsigned int id;
  APObjectCreation(std::string iTemplateName, double iX, double iY, double iAngle, double iZ, unsigned int iId);
};

struct ACharacterCreation : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_PObjectCreation;
  char TemplateName[64] = {0};
  double X;
  double Y;
  double Angle;
  double Z;
  unsigned int id;
  ACharacterCreation(std::string iTemplateName, double iX, double iY, double iAngle, double iZ, unsigned int iId);
};

struct ACharacterStatus : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_CharacterStatus;
};

struct AAddEffect : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_AddEffect;
};

struct AControllerMessage : public ANetMessage {
  static const AMessageType enumType = AMessageType::MT_ControllerMessage;
};


#endif // ANETMESSAGE_H
