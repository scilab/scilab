/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: Contour.h                                                        */
/* desc : Functions to compute a level curves                             */
/*------------------------------------------------------------------------*/

#ifndef _CONTOUR_H_
#define _CONTOUR_H_

/**
 * Draw level curves for a function f(x,y) which values 
 * at points x(i),y(j) are given by z(i,j)
 * - z is a (n1,n2) matrix 
 * - x is a (1,n1) matrix 
 * - y is a (1,n2) matrix 
 * - x,y,z are stored as one dimensionnal array in C 
 * - if *flagnz =0 
 * -   then  nz is an int pointer to the number of level curves. 
 *     else  zz is an array which gives th requested level values.
 *            (and nz is the size of thos array)
 * Computed from min and max of z
 * Example Contour(1:5,1:10,rand(5,10),5);
 */
int C2F(contourif)(double *x, double *y, double *z, int *n1, int *n2, int *flagnz, int *nz, double *zz, int *style) ;

/**
 * used to bring back data to Scilab Stack
 */
int C2F(getconts)(double **x, double **y, int *mm, int *n) ;

#endif /* _CONTOUR_H_ */
