#ifndef ACHARACTERSFML_H
#define ACHARACTERSFML_H
#include "ACharacterBase.h"
#include "APhysicObjectNS.h"

class ACharacterNS
    : public APhysicObjectNS,
      public ACharacterBase {

public:
  ACharacterNS(const APhysicObjectData &data, double iX, double iY, double iAngle, double iZ, AWorldNS *iWorld);
  virtual void Display(double dt);
  virtual ~ACharacterNS() {}


};

#endif // ACHARACTERSFML_H
