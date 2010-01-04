/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __PATHCONVERT_H__
#define __PATHCONVERT_H__

#include <wchar.h>
#include "BOOL.h"

typedef enum { 
WINDOWS_STYLE = 0,
UNIX_STYLE = 1,
AUTO_STYLE = 2
} PathConvertType;

wchar_t *pathconvertW(wchar_t* wcpath, BOOL flagtrail, BOOL flagexpand, PathConvertType PType);

char *pathconvert(char* path, BOOL flagtrail, BOOL flagexpand, PathConvertType PType);

#endif /* PATHCONVERT_H__ */
/*--------------------------------------------------------------------------*/ 
