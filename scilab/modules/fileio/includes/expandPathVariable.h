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

#endif /* __EXPANDPATHVARIABLE_H__ */
/*--------------------------------------------------------------------------*/
