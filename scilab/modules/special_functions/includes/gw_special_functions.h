
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

#ifndef __GW_SPECIAL_FUNCTIONS_H__
#define __GW_SPECIAL_FUNCTIONS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_special_functions.h"
/*--------------------------------------------------------------------------*/
SPECIAL_FUNCTIONS_IMPEXP int gw_special_functions(void);
/*--------------------------------------------------------------------------*/
SPECIAL_FUNCTIONS_IMPEXP int sci_besselh(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_besseli(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_besselj(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_besselk(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_bessely(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_beta(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_legendre(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_gamma(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_lgamma(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_dlgamma(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_Faddeeva_erf(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_Faddeeva_erfc(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_Faddeeva_erfcx(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_Faddeeva_erfi(char *fname, void* pvApiCtx);
SPECIAL_FUNCTIONS_IMPEXP int sci_Faddeeva_dawson(char *fname, void* pvApiCtx);
/*--------------------------------------------------------------------------*/
#endif /* __GW_SPECIAL_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/
