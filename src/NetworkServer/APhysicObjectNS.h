#ifndef APHYSICOBJECTSFML_H
#define APHYSICOBJECTSFML_H
#include "APhysicObjectBase.h"
#include "const.h"

class AWorldNS;

class APhysicObjectNS
    : virtual public APhysicObjectBase {
protected:
  AWorldNS* nsWorld;
  const int x = 0;
  const int y = 1;
  const int a = 2;

public:
  APhysicObjectNS(const APhysicObjectData& data, double iX, double iY, double iAngle, double iZ, AWorldNS *iWorld);
  virtual void Display(double dt);
  void Extrapolate(double dt);
  virtual ~APhysicObjectNS();
};

#endif // APHYSICOBJECTSFML_H
