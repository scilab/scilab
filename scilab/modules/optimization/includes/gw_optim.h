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
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_optimization(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_optim) (char *fname,unsigned long fname_len);
int C2F(sci_semidef) (char *fname,unsigned long fname_len);
int C2F(sci_fsolve) (char *fname,unsigned long fname_len);
int C2F(sci_sqrsolve) (char *fname,unsigned long fname_len);
int C2F(sci_qld) (char *fname,unsigned long fname_len);
int C2F(intlsqrsolve) (char *fname,unsigned long fname_len);
int C2F(scioptim) (char *fname,unsigned long fname_len);
int C2F(scisolv)  (char *fname,unsigned long fname_len);
int C2F(scisemidef)(char *fname,unsigned long fname_len);
int C2F(sci_qp_solve)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_OPTIM__ */
/*--------------------------------------------------------------------------*/

