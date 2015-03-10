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

#include "ALocationSFML.h"
#include <fstream>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <libconfig.h++>
#include "AWorldSFML.h"
#include "Global.h"
#include "AGroundCell.h"
#include "ASpriteContainer.h"

using namespace std;
using namespace libconfig;

/*
ALocationSFML::ALocationSFML(std::string locationFilename, AWorldSFML *iWorld)
      : ALocationBase(iWorld) {
  _worldSFML = iWorld;
  libconfig::Config cfg;
  try {
    cfg.readFile(locationFilename.c_str());
  } catch(libconfig::ParseException& e) {
    std::cout << e.getError() << ' ' << e.getLine() << '\n';
    throw e;
  }

  loadLocationFromCfg(cfg);
}
*/

ALocationSFML::ALocationSFML(libconfig::Config& locationData, AWorldSFML *iWorld, const ContextLocArgs& la)
      : ALocationBase(iWorld) {
  _worldSFML = iWorld;
  loadLocationFromCfg(locationData, la);
}


void ALocationSFML::Display() {
  int lx = (ASprite::cdx - msx - ASprite::renderRadius) / cellSize + 1;
  int ly = (ASprite::cdy - msy - ASprite::renderRadius) / cellSize + 1;
  int rx = (ASprite::cdx - msx + ASprite::renderRadius) / cellSize;
  int ry = (ASprite::cdy - msy + ASprite::renderRadius) / cellSize;
  if(lx < 0) lx = 0;
  if(ly < 0) ly = 0;
  if(rx > mXLen) rx = mXLen;
  if(ry > mYLen) ry = mYLen;
  for(int y = ly; y < ry; y++) {
    for(int x = lx; x < rx; x++) {
      ASprite* spr = mGround[x][y]->Sprite;
      spr->setPosition(cellSize * (x + 0.5 + msx) * PIXELS_IN_METER , -(cellSize * (y + 0.5 + msy) * PIXELS_IN_METER) );
      Game::Window->draw(*spr);
    }
  }
}

void ALocationSFML::LoadGroundCells(const std::string& iFileName) {

  Config cfg;
  cfg.readFile(iFileName.c_str());
  Setting& s = cfg.lookup("cells");
  for(int i = 0; i < s.getLength(); i++) {
    Setting& c = s[i];
    AGroundCell* gc = new AGroundCell(c["name"], Game::ObjectSpriteContainer->GetSprite(c["sprite"]), c["damping"]);
    scriptGroundContainer.insert(pair<string, AGroundCell*> ((const char*)c["name"], gc));
    }
}

void ALocationSFML::Finalize() {
  for(auto p : scriptGroundContainer) {
    delete p.second;
  }
}


