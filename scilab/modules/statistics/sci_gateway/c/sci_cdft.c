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

extern int C2F(cdft)(int *, double *, double *, double *, double *, int *, double *);

/**
 * Interface to dcdflib's cdft
 * SUBROUTINE CDFT( WHICH, P, Q, T, DF, STATUS, BOUND )
 * Cumulative Distribution Function, T distribution
 */
int cdftI(char* fname, int* _piKey)
{
  struct cdf_item items[] =
    {{"PQ", 2, 2, 2},
     {"T" , 3, 1, 3},
     {"Df", 3, 1, 0}};
  struct cdf_descriptor cdf = mkcdf(cdft, 3, 4, 1, 2, items);
  return cdf_generic(fname, _piKey, &cdf);
} 
