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

#include "Common.h"
#include <fstream>
#include <vector>
#include <zlib.h>
#include <sstream>
#include <cstring>
#include <stdexcept>
#include <SFML/System.hpp>
#include <libconfig.h++>
#include <cstdio>

double Abs(double x) {
  return x > 0 ? x : -x;
}



std::string FromCfgFormat(const std::string& str) {
  std::string s;
  for(int i = 0; i < str.size(); ++i) {
    if(str[i] ==  '\1') {
      s.push_back('\n');
    } else
    if(str[i] ==  '\2') {
      s.push_back('"');
    } else {
      s.push_back(str[i]);
    }
  }
  return s;
}


std::string ToCfgFormat(const std::string& str) {
  std::string s;
  for(int i = 0; i < str.size(); ++i) {
    if(str[i] ==  '\n') {
      s.push_back('\1');
    } else
    if(str[i] ==  '"') {
      s.push_back('\2');
    } else {
      s.push_back(str[i]);
    }
  }
  return s;
}



void CopyFile(const std::string& source, const std::string& dest) {
  char buffer[1];
  std::ifstream input;
  std::ofstream output;
  input.open(source);
  output.open(dest, std::ofstream::trunc | std::ofstream::out);

  do {
    input.read(buffer, sizeof(buffer));
    if (input.good())
    output.write(buffer, sizeof(buffer));
  } while (! input.eof());

  input.close();
  output.close();
}


void PackSave(const std::string& mapDir, const std::string& savePath) {
  std::vector<std::string> pts;
  pts.push_back("main.map");
  std::ifstream input;
  char buffer[1];
  using namespace libconfig;
  Config cfg;
  cfg.readFile((mapDir + "main.map").c_str());
  Setting& locs = cfg.lookup("locs");
  for(int i = 0; i < locs.getLength(); ++i) {
    pts.push_back((const char*)(locs[i][2]));
  }
  std::stringstream data;

  for(const std::string& s : pts) {
    data << '\1' << s << '\n';
    input.open(mapDir + s);
    do {
      input.read(buffer, sizeof(buffer));
      if (input.good())
      data.write(buffer, sizeof(buffer));
    } while (!input.eof());
    input.close();
  }

  uLongf srcLen = data.rdbuf()->str().size() * sizeof(char) / sizeof(Bytef);
  char src[srcLen];
  strcpy(src, data.rdbuf()->str().c_str());
  uLongf destLen = srcLen * 1.01 + 12;
  Bytef dest[destLen];

  if(compress(dest, &destLen, (Bytef*)(src), srcLen) != Z_OK) {
    throw std::logic_error("cant compress");
  }

  std::ofstream save;
  save.open(savePath, std::ofstream::trunc | std::ofstream::out);
  /*save.write((unsigned char*)(&srcsize), 4);
  save.write((unsigned char*)(&destsize), 4);*/
  save << srcLen << ' ' << destLen << ' ';
  save.write((char*)(dest), destLen);
  /*
  data.seekg(0, data.beg);

  std::ofstream save;
  save.open(savePath, std::ofstream::trunc | std::ofstream::out);
  do {
    data.read(buffer, sizeof(buffer));
    if (data.good())
    save.write(buffer, sizeof(buffer));
  } while (! data.eof());*/
  save.close();
}


void UnpackSave(const std::string& savePath, const std::string& mapDir) {
  char buffer[1];

  std::stringstream sstr;
  std::ifstream input;
  std::ofstream output;
  input.open(savePath);
  uLongf srcSize;
  uLongf destSize;
  input >> srcSize >> destSize;
  input.get();
  Bytef dest[destSize];
  Bytef src[srcSize];
  input.read((char*)(dest), destSize);
  uncompress(src, &srcSize, dest, destSize);
  input.close();
  //output.open("../workspace/temp", std::ofstream::out | std::ofstream::trunc);
  //output.write((char*)(src), srcSize);
  sstr.write((char*)(src), srcSize);
  //output.close();
  //input.open("../workspace/temp");

  std::string filename;


  do {
    sstr.read(buffer, sizeof(buffer));
  } while (sstr && buffer[0] != '\1');
  sstr >> filename;
  sstr.get();
  output.open(mapDir + filename, std::ofstream::trunc | std::ofstream::out);

  do {
    sstr.read(buffer, sizeof(buffer));
    if (sstr.good()) {
      if(buffer[0] == '\1') {
        sstr >> filename;
        sstr.get();
        output.close();
        output.open(mapDir + filename, std::ofstream::trunc | std::ofstream::out);
        continue;
      }
    }
    if(!sstr) break;
    output.write(buffer, sizeof(buffer));
  } while (sstr);

  output.close();
}


#ifdef __linux__
#include <sys/types.h>
#include <dirent.h>


void ClearDirectory(const char* dirPath) {
  DIR* d = opendir(dirPath);
  if(!d) throw std::logic_error("can't open directory");
  auto* de = readdir(d);
  while(de) {
    remove((std::string(dirPath) + de->d_name).c_str());
    de = readdir(d);
  }
  closedir(d);
}



void GetAllFilenamesInDir(const char* dirPath, std::vector<std::string>& container) {
  DIR* d = opendir(dirPath);
  if(!d) throw std::logic_error("can't open directory");
  auto* de = readdir(d);
  while(de) {
    container.push_back(de->d_name);
    de = readdir(d);
  }
  closedir(d);
}

#endif
