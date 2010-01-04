
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
#ifndef __GW_FUNCTIONS_H__
#define __GW_FUNCTIONS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_functions.h"
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
FUNCTIONS_SCILAB_IMPEXP int gw_functions(void);
/*--------------------------------------------------------------------------*/
FUNCTIONS_SCILAB_IMPEXP int C2F(sci_lib)(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int C2F(sci_deff)(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int C2F(sci_getf)(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int C2F(sci_exec)(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int C2F(sci_execstr)(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int sci_librarieslist(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int sci_libraryinfo(char *fname,unsigned long fname_len);
FUNCTIONS_SCILAB_IMPEXP int sci_whereis(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/

