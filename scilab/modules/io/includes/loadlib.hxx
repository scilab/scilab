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
    MacroInfo(const std::wstring& _name, const std::wstring& _file, const std::wstring& _md5) :
        name(_name),
        file(_file),
        md5(_md5) {}
    std::wstring name;
    std::wstring file;
    std::wstring md5;
};

typedef std::unordered_map<std::wstring, MacroInfo> MacroInfoList;
IO_IMPEXP types::Library* loadlib(const std::wstring& _wstXML, int* ierr, bool _isFile = true, bool _bAddInContext = true);
IO_IMPEXP int parseLibFile(const std::wstring& _wstXML, MacroInfoList& info, std::wstring& libname);
#endif /* !__LOADLIB_H__ */