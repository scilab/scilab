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

IO_IMPEXP types::Library* loadlib(const std::wstring& _wstXML, int* ierr, bool _isFile = true, bool _bAddInContext = true);

#endif /* !__LOADLIB_H__ */