/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __CALLXPAD_H__
#define __CALLXPAD_H__
/*--------------------------------------------------------------------------*/
#include <wchar.h>
#include "dynlib_xpad.h"
/*--------------------------------------------------------------------------*/
/**
* call Xpad from C (Wide Char as input filesname)
* used by Windows for localized files name
* @param[in] _wcfilenames wide char encoding (filenames to open)
* @param[in] size of _wcfilenames
* @return 0
*/
XPAD_IMPEXP int callXpadW(wchar_t **_wcfilenames, int _nbfiles);
XPAD_IMPEXP int callXpadWWithLineNumber(wchar_t **_wcfilenames, double* pdblLineNumber, int _nbfiles);
/*--------------------------------------------------------------------------*/
#endif /* __CALLXPAD_H__ */
