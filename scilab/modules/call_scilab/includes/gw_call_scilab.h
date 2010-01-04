/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_CALL_SCILAB__
#define __GW_CALL_SCILAB__
/*--------------------------------------------------------------------------*/ 
#include "api_scilab.h"
#include "dynlib_call_scilab.h"
/*--------------------------------------------------------------------------*/ 
CALL_SCILAB_IMPEXP int gw_call_scilab(void);
/*--------------------------------------------------------------------------*/ 
CALL_SCILAB_IMPEXP int sci_fromjava(char *fname,unsigned long fname_len);
CALL_SCILAB_IMPEXP int sci_fromc(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_CALL_SCILAB__ */
/*--------------------------------------------------------------------------*/

