/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __EXPANDPATHVARIABLE_H__
#define __EXPANDPATHVARIABLE_H__

#include <wchar.h>
#include "dynlib_fileio.h"

/**
* expanded variables are:
* "SCI"
* "SCIHOME"
* "HOME"
* "TMPDIR"
*/

#ifdef __cplusplus
extern "C"
{
#endif
/**
* expand in_name to produce out_name
* @param[in] string where to find a variable and expand
* @return string with expanded variable
*/
FILEIO_IMPEXP char *expandPathVariable(char* str);

/**
* expand in_name to produce out_name (wide string)
* @param[in] wide string where to find a variable and expand
* @return wide string with expanded variable
*/
FILEIO_IMPEXP wchar_t *expandPathVariableW(wchar_t *wcstr);

/**
* set NULL to all scilab "Variable" (ie: SCI, SCIHOME, ....)
*/
FILEIO_IMPEXP void resetVariableValueDefinedInScilab(void);

#ifdef __cplusplus
}
#endif //_cplusplus

#endif /* __EXPANDPATHVARIABLE_H__ */
/*--------------------------------------------------------------------------*/
