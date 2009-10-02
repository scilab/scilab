
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
#ifndef __GW_CACSD0__
#define __GW_CACSD0__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
int gw_cacsd0(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_gschur)(char *fname,unsigned long fname_len);
int C2F(sci_gspec)(char *fname,unsigned long fname_len);
int C2F(sci_ereduc)(char *fname,unsigned long fname_len);
int C2F(sci_fstair)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CACSD0__ */
/*--------------------------------------------------------------------------*/

