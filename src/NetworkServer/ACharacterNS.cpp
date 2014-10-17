#include "ACharacterNS.h"
#include "AWorldNS.h"
#include "AVector2.h"


void ACharacterNS::Display(double dt) {
    if(!visible) return;
  //std::cout << "sending character data!\n";

}


ACharacterNS::ACharacterNS(const APhysicObjectData &data, double iX, double iY, double iAngle, double iZ, AWorldNS *iWorld)
    : APhysicObjectNS(data, iX, iY, iAngle, iZ, iWorld),
    ACharacterBase(data, iX, iY, iAngle, iZ, iWorld),
    APhysicObjectBase(data, iX, iY, iAngle, iZ, iWorld) {
}
