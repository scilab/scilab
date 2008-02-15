
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "displ.h"
#include "sciprint.h"

void displ(double *a, int ma,int na, int *b,int mb, int nb, char *c,int  mc,int  nc)
{
  sciprint("First parameter (matrix) is %i x %i:\n",ma,na);
  sciprint("its (1,1) entry is %e (double).\n", a[0]);
  sciprint("Second parameter (matrix) is %i x %i:\n",mb,nb);
  sciprint("its (1,1) entry is %i (int).\n", b[0]);
  sciprint("Third parameter (string) is %i character long: it is the string \"%s\"\n",mc*nc, c);
}


