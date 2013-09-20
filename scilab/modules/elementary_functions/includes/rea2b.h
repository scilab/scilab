/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
