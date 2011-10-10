/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#ifndef __PROMPTECHO_H__
#define __PROMPTECHO_H__

#include "machine.h"
#include "dynlib_console.h"

CONSOLE_IMPEXP int C2F(promptecho)(int *lunit, char *string, int *strl,int string_len);

#endif /* __PROMPTECHO_H__ */
/*--------------------------------------------------------------------------*/ 
