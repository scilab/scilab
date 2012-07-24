/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
*    Graphic library
*    Axis drawing for 2d plots (format selection)
--------------------------------------------------------------------------*/

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "dynlib_graphics.h"
#include "machine.h" /* C2F */
#include "StringMatrix.h"

/**
 * @TODO add comment
 *
 * @param xmi
 * @param xma
 * @param xi
 * @param xa
 * @param np1
 * @param np2
 * @param kminr
 * @param kmaxr
 * @param ar
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, int *np1, int *np2, int *kminr, int *kmaxr, int *ar) ;

/**
 * @TODO add comment
 *
 * @param pobjUID
 * @param c_format
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ComputeC_format( char * pobjUID, char * c_format ) ;

/**
 * @TODO add comment
 *
 * @param pobjUID
 * @param xy_type
 * @param vector
 * @param N
 * @param checkdim
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ComputeXIntervals( char * pobjUID, char xy_type, double ** vector, int * N, int checkdim ) ;

/**
 * @TODO add comment
 *
 * @param pobjUID
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP StringMatrix * computeDefaultTicsLabels( char * pobjUID ) ;

/**
 * same as ChoixFormatE when numbers are given through an
 * array xx[0:nx-1];
 */
GRAPHICS_IMPEXP void ChoixFormatE1(char *fmt, double *xx, int nx) ;

/**
* Create a new string array which is the conversion of the doubles values.
* @param bufferSize size of the buffer used to store the store before the copying
*                   it to the result. It must greater than the length of any of the strings.
*                   and ideally the maximum length.
*/
GRAPHICS_IMPEXP char ** copyFormatedArray( const double values[], int nbStrings, const char format[5], int bufferSize ) ;

/**
 * @TODO add comment
 *
 * @param void
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP char * getFPF(void);

GRAPHICS_IMPEXP int sciGetLogExponent( double minBound, double maxBound, double * expMin, double * expMax );

#endif /* _FORMAT_H_ */
