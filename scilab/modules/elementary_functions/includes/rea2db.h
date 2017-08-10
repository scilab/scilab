/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
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
