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
int gw_signal(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_ffir)(char *fname,unsigned long fname_len);
int C2F(sci_fft)(char *fname,unsigned long fname_len);
int C2F(sci_corr)(char *fname,unsigned long fname_len);
int C2F(sci_fiir)(char *fname,unsigned long fname_len);
int C2F(sci_rpem)(char *fname,unsigned long fname_len);
int C2F(sci_amell)(char *fname,unsigned long fname_len);
int C2F(sci_delip)(char *fname,unsigned long fname_len);
int C2F(sci_remez)(char *fname,unsigned long fname_len);
int C2F(sci_syredi)(char *fname,unsigned long fname_len);
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

