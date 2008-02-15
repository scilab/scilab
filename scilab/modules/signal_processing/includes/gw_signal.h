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
 
#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(gw_signal)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_ffir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fft) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_corr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fiir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_rpem) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_amell) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_delip) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_remez) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_syredi) _PARAMS((char *fname,unsigned long fname_len));
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

