/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __PROCESS_DNEUPD_H__
#define __PROCESS_DNEUPD_H__
#include "doublecomplex.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "sciprint.h"

/**
 * @TODO add comment
 *
 * @param DR
 * @param DI
 * @param Z
 * @param N
 * @param nev
 * @param AR
 * @param eigenvalue
 * @param eigenvector
 * @param sigma_imaginary
 * @param RVEC
 */

void process_dneupd_data(double* DR, double* DI, double* Z, int N, int nev, double* AR,
                         doublecomplex* eigenvalue, doublecomplex* eigenvector,
                         int sigma_imaginary);

#endif /* __PROCESS_DNEUPD_H__ */
/*--------------------------------------------------------------------------*/
