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

#ifndef ATRANSLATIONREADER_H
#define ATRANSLATIONREADER_H
#include <string>
#include <fstream>
#include <wchar.h>
#include <map>
typedef std::map<std::wstring, std::wstring> _dict;

class ATranslationReader {
  std::map<std::string, _dict> mLoadedFiles;
  std::map<std::string, _dict>::iterator mCur;

public:
  const std::string Path;
  bool AddErrorString = true;
  ATranslationReader(const std::string& path);
  void SetFile(const std::string& fileName);
  std::wstring GetTranslation(const std::wstring& mark);
  std::wstring GetTranslation(const std::string& mark);
};

#endif // ATRANSLATIONREADER_H
