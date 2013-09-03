/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#ifndef __READ2DB_H__
#define __READ2DB_H__

#include "machine.h"
#include "dynlib_elementary_functions.h"
/*--------------------------------------------------------------------------*/
/* cette subroutine traduit un vecteur dx, de taille n, sur un
   vecteur double precision dy.
   dans le cas de deux increments egaux a 1, cette fonction
   emploie des boucles "epanouies".
   dans le cas ou les increments sont negatifs cette
   fonction prend les composantes en ordre inverse.
*/
/*--------------------------------------------------------------------------*/
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(rea2db)(int *n, float *dx, int *incx, double *dy, int *incy);

#endif /* __READ2DB_H__ */
