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

#ifndef COMMON_H
#define COMMON_H
typedef unsigned char byte;
#include <iostream>
#include <math.h>
#include <vector>

double Abs(double x);

template<class type>
void BinWrite(char* buffer, size_t& written, type* obj) {
  byte* bytePtr = static_cast<byte>(static_cast<void*>(obj));
  for(int i = 0; i < sizeof(type); i++, written++) {
    buffer[i] = bytePtr[i];
  }
}

void CopyFile(const std::string& source, const std::string& dest);

std::string FromCfgFormat(const std::string& str);
std::string ToCfgFormat(const std::string& str);
void PackSave(const std::string& mapDir, const std::string& savePath);
void UnpackSave(const std::string& savePath, const std::string& mapDir);

std::string toStr(double d, int p = 2);

void ClearDirectory(const char* dirPath);
void GetAllFilenamesInDir(const char* dirPath, std::vector<std::string>& container);

#endif // COMMON_H
