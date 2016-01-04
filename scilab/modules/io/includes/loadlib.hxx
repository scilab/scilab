/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __LOADLIB_H__
#define __LOADLIB_H__

#include "dynlib_io.h"
#include "library.hxx"

struct MacroInfo
{
    MacroInfo() {} //needed by unordered_map
    MacroInfo(const std::string& _name, const std::string& _file, const std::string& _md5) :
        name(_name),
        file(_file),
        md5(_md5) {}
    std::string name;
    std::string file;
    std::string md5;
};

typedef std::unordered_map<std::string, MacroInfo> MacroInfoList;
IO_IMPEXP types::Library* loadlib(const std::string& _wstXML, int* ierr, bool _isFile = true, bool _bAddInContext = true);
IO_IMPEXP int parseLibFile(const std::string& _wstXML, MacroInfoList& info, std::string& libname);
#endif /* !__LOADLIB_H__ */