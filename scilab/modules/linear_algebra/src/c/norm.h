/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string.h>         // strcmp
#include "core_math.h"      // Abs, pow, Min and Max
#include "machine.h"        // C2F
#include "localization.h"  // Translations
#include "doublecomplex.h" // Type doublecomplex

double normString (double *A, int iRows, int iCols, char *flag);

double normStringC (doublecomplex *A, int iRows, int iCols, char *flag);

double normP (double *A, int iRows, int iCols, double p);

double normPC (doublecomplex *A, int iRows, int iCols, double p);

#ifdef _MSC_VER
int isinf(double dbl);
#endif

