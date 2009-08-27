/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_UMFPACK__
#define __GW_UMFPACK__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_umfpack(void);
int sci_umfpack(char* fname, unsigned long l);
int sci_umf_lufact(char* fname, unsigned long l);
int sci_umf_lusolve(char* fname, unsigned long l);
int sci_umf_ludel(char* fname, unsigned long l);
int sci_umf_luinfo(char* fname, unsigned long l);
int sci_umf_luget(char* fname, unsigned long l);
int sci_taucs_chfact(char* fname, unsigned long l);
int sci_taucs_chsolve(char* fname, unsigned long l);
int sci_taucs_chdel(char* fname, unsigned long l);
int sci_taucs_chinfo(char* fname, unsigned long l);
int sci_taucs_chget(char* fname, unsigned long l);
int sci_res_with_prec(char* fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_UMFPACK__ */
/*--------------------------------------------------------------------------*/
