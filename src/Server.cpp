#include "AWorldNS.h"
#include "ALocationNS.h"
#include "iostream"
#include <boost/date_time/microsec_time_clock.hpp>
#include "SFML/Network.hpp"
#include "ANetPackage.h"
#include <vector>

using boost::posix_time::ptime;
using namespace boost::asio;



int main() {
  AWorldNS world;
  ptime _curTimeP;
  ptime _prevTimeP;
  double _timeCountP = 0.0;
  double frameLengthP = SECOND / 30.0;

  sf::TcpListener listener;
  listener.listen(22800);
  AWorldNS world;
  ALocationNS* loc = world.GetCurrentLocation();


  while(true) {
    _prevTimeP = _curTimeP;
    _curTimeP = boost::date_time::microsec_clock<ptime>::local_time();
    double delta = (_curTimeP - _prevTimeP).total_microseconds();
    _timeCountP += delta;
    if(_timeCountP > frameLengthP) {

      while(true) {
        Client c;
        if(listener.accept(c.socket) == sf::Socket::Done) {
          world.clients.push_back(c);
          loc->SendCreationMessages(c.socket);
        } else break;
      }

      world.LogicStep();
      world.GraphicStep();
      _timeCountP -= frameLengthP;
    }
  }
}
