
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GW_SPECIAL_FUNCTIONS_H__
#define __GW_SPECIAL_FUNCTIONS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_special_functions.h"
/*--------------------------------------------------------------------------*/
SPECIAL_FUNCTIONS_IMPEXP int gw_special_functions(void);
/*--------------------------------------------------------------------------*/
int sci_besselh(char *fname, unsigned long fname_len);
int sci_besseli(char *fname, unsigned long fname_len);
int sci_besselj(char *fname, unsigned long fname_len);
int sci_besselk(char *fname, unsigned long fname_len);
int sci_bessely(char *fname, unsigned long fname_len);
int sci_beta(char *fname, unsigned long fname_len);
int sci_legendre(char *fname, unsigned long fname_len);
int sci_gamma(char *fname, unsigned long fname_len);
int sci_lgamma(char *fname, unsigned long fname_len);
int sci_dlgamma(char *fname, unsigned long fname_len);
int sci_Faddeeva_erf(char *fname, unsigned long fname_len);
int sci_Faddeeva_erfc(char *fname, unsigned long fname_len);
int sci_Faddeeva_erfcx(char *fname, unsigned long fname_len);
int sci_Faddeeva_erfi(char *fname, unsigned long fname_len);
int sci_Faddeeva_dawson(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_SPECIAL_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/
