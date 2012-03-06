/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_SYMBOLIC__
#define __GW_SYMBOLIC__
/*--------------------------------------------------------------------------*/ 
#include "dynlib_symbolic.h"
/*--------------------------------------------------------------------------*/ 
SYMBOLIC_IMPEXP int gw_symbolic(void);
/*--------------------------------------------------------------------------*/ 
SYMBOLIC_IMPEXP int sci_addf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_subf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_mulf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_ldivf(char *fname,unsigned long fname_len);
SYMBOLIC_IMPEXP int sci_rdivf(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SYMBOLIC__ */
/*--------------------------------------------------------------------------*/

