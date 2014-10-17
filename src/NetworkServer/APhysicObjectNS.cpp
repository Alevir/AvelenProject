#include "APhysicObjectNS.h"
#include "AWorldNS.h"


APhysicObjectNS::APhysicObjectNS(const APhysicObjectData& data, double iX, double iY, double iAngle, double iZ, AWorldNS* iWorld)
    : APhysicObjectBase(data, iX, iY, iAngle, iZ, iWorld) {
  nsWorld = iWorld;
}


double MaxObjectHalfSize = 1.0;

void APhysicObjectNS::Extrapolate(double dt) {

}

void APhysicObjectNS::Display(double dt) {
  if(!visible) return;
  //std::cout << "sending object data!\n";

}


APhysicObjectNS::~APhysicObjectNS() {

}

