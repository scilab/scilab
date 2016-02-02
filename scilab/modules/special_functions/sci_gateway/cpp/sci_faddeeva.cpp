/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Steven G. Johnson <stevenj@alum.mit.edu>
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
