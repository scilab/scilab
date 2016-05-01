/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifndef __REA2B_H__
#define __REA2B_H__

/**
* translate a float vector to double precision vector
* @param n size of dx vector
* @param dx float vector
* @param incx increment order
* @param dy double precision vector
* @param incy increment order
*/
int C2F(rea2db)(int *n, float *dx, int *incx, double *dy, int *incy);

#endif /* __REA2B_H__ */
/*--------------------------------------------------------------------------*/
