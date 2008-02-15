/*--------------------------------------------------------------------------*/
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
#ifndef __GW_CACSD2__
#define __GW_CACSD2__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_cacsd2)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_ppol) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_tzer) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_freq) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ltitr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rtitr) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/
#endif /*  __GW_GW_CACSD2__ */
/*--------------------------------------------------------------------------*/

