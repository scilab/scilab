
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#ifndef __GW_CACSD3__
#define __GW_CACSD3__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int gw_cacsd3(void);
/*--------------------------------------------------------------------------*/
extern int C2F(sci_arl2) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_residu) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ldiv) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CACSD3__ */
/*--------------------------------------------------------------------------*/

