/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Steven G. Johnson <stevenj@alum.mit.edu>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

extern "C" {
#include "gw_special_functions.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "localization.h"
}

#include "faddeeva.h"

#define SCI_FADDEEVA sci_Faddeeva_erf
#define FADDEEVA_FUNC Faddeeva::erf
#include "sci_faddeeva.hxx"
#undef SCI_FADDEEVA
#undef FADDEEVA_FUNC

#define SCI_FADDEEVA sci_Faddeeva_erfc
#define FADDEEVA_FUNC Faddeeva::erfc
#include "sci_faddeeva.hxx"
#undef SCI_FADDEEVA
#undef FADDEEVA_FUNC

#define SCI_FADDEEVA sci_Faddeeva_erfcx
#define FADDEEVA_FUNC Faddeeva::erfcx
#include "sci_faddeeva.hxx"
#undef SCI_FADDEEVA
#undef FADDEEVA_FUNC

#define SCI_FADDEEVA sci_Faddeeva_erfi
#define FADDEEVA_FUNC Faddeeva::erfi
#include "sci_faddeeva.hxx"
#undef SCI_FADDEEVA
#undef FADDEEVA_FUNC

#define SCI_FADDEEVA sci_Faddeeva_dawson
#define FADDEEVA_FUNC Faddeeva::Dawson
#include "sci_faddeeva.hxx"
#undef SCI_FADDEEVA
#undef FADDEEVA_FUNC
