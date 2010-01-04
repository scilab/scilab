/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GETLONGPATHNAME_H__
#define __GETLONGPATHNAME_H__

#include <wchar.h>
#include "BOOL.h" /* BOOL */

/**
* Retrieves the long path form of the specified path
* @param[in] short path name
* @param[out] TRUE if conversion is ok
* @return long path name
* on Linux returns same path name
* used for Windows
*/
char *getlongpathname(char *shortpathname,BOOL *convertok);

/**
* Retrieves the long path form of the specified path (wide string)
* @param[in] short path name
* @param[out] TRUE if conversion is ok
* @return long path name
* on Linux returns same path name
* used for Windows
*/
wchar_t *getlongpathnameW(wchar_t *wcshortpathname,BOOL *convertok);


#endif /* __GETLONGPATHNAME_H__ */
/*--------------------------------------------------------------------------*/
