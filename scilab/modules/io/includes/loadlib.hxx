/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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