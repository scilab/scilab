
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_SPECIAL_FUNCTIONS2__
#define __GW_SPECIAL_FUNCTIONS2__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
int gw_special_functions2(void);
/*--------------------------------------------------------------------------*/ 
int sci_besselh(char *fname,unsigned long fname_len);
int sci_besseli(char *fname,unsigned long fname_len);
int sci_besselj(char *fname,unsigned long fname_len);
int sci_besselk(char *fname,unsigned long fname_len);
int sci_bessely(char *fname,unsigned long fname_len);
int sci_beta(char *fname,unsigned long fname_len);
int sci_legendre(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_SPECIAL_FUNCTIONS2__ */
/*--------------------------------------------------------------------------*/
