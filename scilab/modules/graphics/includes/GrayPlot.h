/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*------------------------------------------------------------------------*/
/* file: GrayPlot.h                                                       */
/* desc : Functions to compute and draw a grayplot                        */
/*------------------------------------------------------------------------*/

#ifndef _GRAY_PLOT_H_
#define _GRAY_PLOT_H_

#include "dynlib_graphics.h"
#include "BOOL.h"
#include "machine.h"

/**
 * - z is a (n1,n2) matrix
 * - x is a (1,n1) matrix
 * - y is a (1,n2) matrix
 * - x,y,z are stored as one dimensional array in C
 *
 *  z : is the value of a function on the grid defined by x,y
 *  on each rectangle the average value of z is computed
 *  and [zmin,zmax] is linearly remapped to the [colormin,colormap]
 *  values of colors in the current colormap
 *  the color associated to zmoy is used for filling a specific rectangle
 */
GRAPHICS_IMPEXP int C2F(xgray)(double *x, double *y, double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1) ;


/**
 * Matplot subroutine
 *  z : of size n1*n2
 *  the z value is interpreted as a color number inside the current colormap
 *  z[i,j] is used as the color of a square [i-0.5,i+0.5] [j-0.5,j+0.5]
 */
GRAPHICS_IMPEXP int C2F(xgray1)(double *z, int *n1, int *n2, char *strflag, double *brect, int *aaint, BOOL flagNax, long int l1) ;


/**
 * like xgray1 :
 * but xrect here give the rectangle in which the
 * grayplot is to be drawn using the current scale
 */
GRAPHICS_IMPEXP int C2F(xgray2)(double *z, int *n1, int *n2, double *xrect) ;

#endif /* _GRAY_PLOT_H_ */
