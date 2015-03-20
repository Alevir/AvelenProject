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

    Author of file: Mikhail Evdokimov

    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
#include "ATranslationReader.h"
#include "stdexcept"
#include "ADebugOutput.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <sstream>
#include "Common/Global.h"
using namespace std;



ATranslationReader::ATranslationReader(const string& path) : Path(path){

}

void ATranslationReader::SetFile(const std::string &fileName) {
  if((mCur = mLoadedFiles.find(fileName)) != mLoadedFiles.end()) return;
  std::wifstream inFile;
  inFile.open(Path + fileName);
  mLoadedFiles[fileName];
  mCur = mLoadedFiles.find(fileName);
  std::wstringstream file;
  std::locale loc("");
  file.imbue(loc);
  inFile.imbue(loc);
  wchar_t c;
  while(!inFile.eof()) {
    inFile.get(c);
    if(c == L'#') {
      while(c != '\n' && !inFile.eof()) {
        inFile.get(c);
      }
      if(inFile.eof()) break;
    }
    file.put(c);
  }

  std::wstring mark;
  std::wstring value;


  while(!file.eof()) {
    mark.clear();
    value.clear();
    file.get(c);
    if(file.eof()) break;
    while(c != L'{') {
      mark.push_back(c);
      file.get(c);
      if(file.eof()) {
        for(int i = 0; i < mark.size(); ++i) {
          if(mark[i] != L'\n' && mark[i] != L' ') {
            throw std::logic_error(std::string("translation file ") + fileName + " has a wrong format");
          }
        }
        mark = L"";
        break;
      }

    }
    if(mark == L"") return;
    while(mark.back() == L' ' || mark.back() == L'\n') {
      mark.pop_back();
    }
    while(mark.front() == L' ' || mark.front() == L'\n') {
      mark.erase(0, 1);
    }
    while(c != L'}') {
      file.get(c);
      if(file.eof()) throw std::logic_error(std::string("translation file ") + fileName + " has wrong format");
      if(c != L'}')
        value.push_back(c);

    }

    mCur->second.insert(std::make_pair(mark, value));
  }
  inFile.close();
}

std::wstring ATranslationReader::GetTranslation(const std::wstring &mark) {
  auto it = mCur->second.begin();
  if((it = mCur->second.find(mark)) == mCur->second.end()) {
    if(AddErrorString) {
      return mark + L" (no translation)";
    } else {
      return mark;
    }
  }
  return it->second;
}

std::wstring ATranslationReader::GetTranslation(const std::string &mark) {

  std::wstring str(mark.begin(), mark.end());
  return  GetTranslation(str);
}
