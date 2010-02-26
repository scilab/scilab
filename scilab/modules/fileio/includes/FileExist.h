/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __FILEEXIST_H__
#define __FILEEXIST_H__

#include <wchar.h>
#include "dynlib_fileio.h"
#include "BOOL.h" /* BOOL */

/**
* verify if filename is a valid file
* @param filename a string 
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL FileExist(char *filename);

/**
* verify if filename is a valid file
* @param filename a wide string 
* @return TRUE or FALSE
*/
FILEIO_IMPEXP BOOL FileExistW(wchar_t *wcfilename);
/*--------------------------------------------------------------------------*/ 
#endif /* __FILEEXIST_H__ */
