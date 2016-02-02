/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2001 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
GRAPHICS_IMPEXP int C2F(graduate)(double *xmi, double *xma, double *xi, double *xa, int *np1, int *np2, int *kminr, int *kmaxr, int *ar);

/**
 * @TODO add comment
 *
 * @param pobjUID
 * @param c_format
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP int ComputeC_format(int iObjUID, char * c_format);

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
GRAPHICS_IMPEXP int ComputeXIntervals(int iObj, char xy_type, double ** vector, int * N, int checkdim);

/**
 * @TODO add comment
 *
 * @param pobjUID
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP StringMatrix * computeDefaultTicsLabels(int iObj);

/**
 * same as ChoixFormatE when numbers are given through an
 * array xx[0:nx-1];
 */
GRAPHICS_IMPEXP void ChoixFormatE1(char *fmt, double *xx, int nx);

/**
* Create a new string array which is the conversion of the doubles values.
* @param bufferSize size of the buffer used to store the store before the copying
*                   it to the result. It must greater than the length of any of the strings.
*                   and ideally the maximum length.
*/
GRAPHICS_IMPEXP char ** copyFormatedArray(const double values[], int nbStrings, const char format[5], int bufferSize);

/**
 * @TODO add comment
 *
 * @param void
 * @return <ReturnValue>
 */
GRAPHICS_IMPEXP char * getFPF(void);

GRAPHICS_IMPEXP int sciGetLogExponent(double minBound, double maxBound, double* expMin, double* expMax);

#endif /* _FORMAT_H_ */
