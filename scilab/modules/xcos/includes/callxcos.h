/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#ifndef __CALLXCOS_H__
#define __CALLXCOS_H__
/*--------------------------------------------------------------------------*/
#include <wchar.h>
#include "dynlib_xcos.h"
/*--------------------------------------------------------------------------*/
/**
* call Xcos from C
* @param[in] _filenames to open
* @param[in] size of _wcfilenames
* @return 0
*/
XCOS_IMPEXP int callXcos(char **_filenames, int _nbfiles);
/*--------------------------------------------------------------------------*/
/**
* call Xcos from C (Wide Char as input filesname)
* used by Windows for localized files name
* @param[in] _wcfilenames wide char encoding (filenames to open)
* @param[in] size of _wcfilenames
* @return 0
*/
XCOS_IMPEXP int callXcosW(wchar_t **_wcfilenames, int _nbfiles);
/*--------------------------------------------------------------------------*/
#endif /* __CALLXCOS_H__ */
