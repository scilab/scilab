/*------------------------------------------------------------------------*/
/* file: Contour.h                                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
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
 * -   then  nz is an integer pointer to the number of level curves. 
 *     else  zz is an array which gives th requested level values.
 *            (and nz is the size of thos array)
 * Computed from min and max of z
 * Example Contour(1:5,1:10,rand(5,10),5);
 */
int C2F(contourif)(double *x, double *y, double *z, integer *n1, integer *n2, integer *flagnz, integer *nz, double *zz, integer *style) ;

/**
 * used to bring back data to Scilab Stack
 */
int C2F(getconts)(double **x, double **y, integer *mm, integer *n) ;

#endif /* _CONTOUR_H_ */
