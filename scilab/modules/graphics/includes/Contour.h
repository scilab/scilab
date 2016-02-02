/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
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

/*------------------------------------------------------------------------*/
/* file: Contour.h                                                        */
/* desc : Functions to compute a level curves                             */
/*------------------------------------------------------------------------*/

#ifndef _CONTOUR_H_
#define _CONTOUR_H_

#include "dynlib_graphics.h"

/**
 * Draw level curves for a function f(x,y) which values
 * at points x(i),y(j) are given by z(i,j)
 * - z is a (n1,n2) matrix
 * - x is a (1,n1) matrix
 * - y is a (1,n2) matrix
 * - x,y,z are stored as one dimensional array in C
 * - if *flagnz =0
 * -   then  nz is an int pointer to the number of level curves.
 *     else  zz is an array which gives th requested level values.
 *            (and nz is the size of thos array)
 * Computed from min and max of z
 * Example Contour(1:5,1:10,rand(5,10),5);
 */
GRAPHICS_IMPEXP int C2F(contourif)(double *x, double *y, double *z, int *n1, int *n2, int *flagnz, int *nz, double *zz, int *style);

/**
 * used to bring back data to Scilab Stack
 */
GRAPHICS_IMPEXP int C2F(getconts)(double **x, double **y, int *mm, int *n);

#endif /* _CONTOUR_H_ */
