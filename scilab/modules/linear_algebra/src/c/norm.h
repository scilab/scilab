/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
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

#include <string.h>         // strcmp
#include "core_math.h"      // Abs, pow, Min and Max
#include "machine.h"        // C2F
#include "localization.h"   // Translations
#include "doublecomplex.h"  // Type doublecomplex
#include "dynlib_linear_algebra.h"

LINEAR_ALGEBRA_IMPEXP double normString (double *A, int iRows, int iCols, char *flag);

LINEAR_ALGEBRA_IMPEXP double normStringC (doublecomplex *A, int iRows, int iCols, char *flag);

LINEAR_ALGEBRA_IMPEXP double normP (double *A, int iRows, int iCols, double p);

LINEAR_ALGEBRA_IMPEXP double normPC (doublecomplex *A, int iRows, int iCols, double p);

#ifdef _MSC_VER
LINEAR_ALGEBRA_IMPEXP int la_isinf(double dbl);
#else
#define la_isinf isinf
#endif

