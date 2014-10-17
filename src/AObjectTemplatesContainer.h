/*
    This file is part of Avelen Project.
    Copyright (c) 2014  Evdokimov Mikhail

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

*/

#ifndef AOBJECTTEMPLATESCONTAINER_H
#define AOBJECTTEMPLATESCONTAINER_H

#include <libconfig.h++>
#include <iostream>
#include <map>
#include <set>
#include <string>

template<class objType>
class AObjectTemplatesContainer {

  std::map<std::string, objType> objectTemplates;
  std::string _name;

public:
  AObjectTemplatesContainer(std::string templateName = "templates") : _name(templateName) {

  }

  void AddContent(const std::string& file) {
    using namespace libconfig;
    Config cfg;
    try {
      cfg.readFile(file.c_str());
    } catch (libconfig::ParseException& ex) {
      std::cout << "file: " << ex.getFile() << "\nline: " << ex.getLine()
        << "\nerror: " << ex.getError() << std::endl;
      throw ex;
    }
    Setting& objList = cfg.lookup(_name.c_str());
    for(int i = 0; i < objList.getLength(); i++) {
      objType newTemplate(objList[i]);
      objectTemplates.insert(std::pair<std::string, objType>((const char*)objList[i]["name"], newTemplate));
    }
  }

  void GetAllTemplateNames(std::set<std::string>& set) {
    for(std::pair<std::string, objType> p : objectTemplates) {
      set.insert(p.first);
    }
  }

  objType& GetTemplate(std::string iTemplateName) {
    return (objectTemplates.find(iTemplateName))->second;
  }

  void Save(const char* filename) {
    using namespace libconfig;
    Config cfg;
    Setting& tlist = cfg.getRoot().add(_name.c_str(), Setting::TypeList);
    for(auto p : objectTemplates) {
      Setting& t = tlist.add(Setting::TypeGroup);
      p.second.ToConfig(t);
    }
    cfg.writeFile(filename);
  }

};

#endif // AOBJECTTEMPLATESCONTAINER_H
