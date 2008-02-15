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
/*--------------------------------------------------------------------------*/
int C2F(gw_optim)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_optim) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_semidef) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sqrsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_qld) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sciquapro) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_quapro) _PARAMS((char *fname,unsigned long fname_len));
int C2F(intqld)_PARAMS((char *fname,unsigned long fname_len));
int C2F(intlsqrsolve) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scioptim) _PARAMS((char *fname,unsigned long fname_len));
int C2F(scisolv)  _PARAMS((char *fname,unsigned long fname_len));
int C2F(scisemidef)_PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_OPTIM__ */
/*--------------------------------------------------------------------------*/

