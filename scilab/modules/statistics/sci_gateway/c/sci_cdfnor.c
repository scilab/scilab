/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA -
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "gw_statistics.h"
#include "CdfBase.h"

extern int C2F(cdfnor)(int *,double *,double *,double*,double *,double *, int *,double *);

/**
 * Interface to dcdflib's cdfnor
 * SUBROUTINE CDFNOR ( WHICH, P, Q, X, MEAN, SD, STATUS, BOUND )
 * Cumulative Distribution Function, NORmal distribution
 */
int cdfnorI(char* fname, int* _piKey)
{
  struct cdf_item items[] =
    {{"PQ"  , 3, 2, 2},
     {"X"   , 4, 1, 3},
     {"Mean", 4, 1, 4},
     {"Std" , 4, 1, 0}};
  struct cdf_descriptor cdf = mkcdf(cdfnor, 4, 5, 1, 2, items);
  return cdf_generic(fname, _piKey, &cdf); 
} 
