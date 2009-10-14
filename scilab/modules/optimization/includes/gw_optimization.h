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
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_OPTIM__
#define __GW_OPTIM__
/*--------------------------------------------------------------------------*/
#include "dynlib_optimization.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
OPTIMIZATION_IMPEXP int gw_optimization(void);
/*--------------------------------------------------------------------------*/
OPTIMIZATION_IMPEXP int sci_optim(char *fname,unsigned long fname_len);
OPTIMIZATION_IMPEXP int sci_semidef(char *fname,unsigned long fname_len);
OPTIMIZATION_IMPEXP int sci_fsolve(char *fname,unsigned long fname_len);
OPTIMIZATION_IMPEXP int sci_sqrsolve(char *fname,unsigned long fname_len);
OPTIMIZATION_IMPEXP int sci_qld(char *fname,unsigned long fname_len);
OPTIMIZATION_IMPEXP int sci_qp_solve(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_OPTIM__ */
/*--------------------------------------------------------------------------*/

