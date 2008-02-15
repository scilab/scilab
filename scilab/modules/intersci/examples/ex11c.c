/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "../../routines/machine.h"

/*******************************************
 * complex matrices
 * are transmited as ar,ai,itra
 *                   real part, imaginary part and a descriptor flag
 *                   the descriptor flag is 1 for a complex matrix 0 if the
 *                   matrix is real valued
 *******************************************/

int F2C(ext11c)(double *ar, double *ai, int *itra,
		double *br, double *bi,int *itrb, int *m, int *n)
{
  int k;
  for (k = 0; k < *n*(*m); ++k)
      {
	if ( *itra == 1)
	  br[k] = ai[k];
	else
	  br[k] = 0.0;
	bi[k] = ar[k];
      }
  *itrb=1;
  return(0);
}

