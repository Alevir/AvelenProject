/*
    This file is part of Avelen Project.

    Avelen Project is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Avelen Project is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Avelen Project.  If not, see <http://www.gnu.org/licenses/>.

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    Authors of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#include "Mode.h"
#include <boost/asio.hpp>

MultiplayerMode::MultiplayerMode() {

}

//using boost::asio;
void MultiplayerMode::_run() {
  window->setMouseCursorVisible(false);
  while(window->isOpen()) {
    sf::Event event;
    while(window->pollEvent(event)) {
      if(event.type == sf::Event::Closed) {
          window->close();
          break;
      } else
      if (event.type == sf::Event::Resized) {
        sf::View v = window->getView();
        v.setSize(event.size.width, event.size.height);
        v.zoom(1.0);
        window->setView(v);
        continue;
      } else
      if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
        window->close();
        continue;
      }
    }
    window->clear();
    window->display();
  }
}
