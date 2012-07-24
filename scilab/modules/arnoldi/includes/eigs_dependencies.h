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
#ifndef __RTIMESRprime_H__
#define __RTIMESRprime_H__
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
 * @param result
 * @param R
 * @param Rprime
 * @param N
 */
void RtimesRprime(double* result, double* R, double* Rprime, int N);

/**
 * @TODO add comment
 *
 * @param result
 * @param R
 * @param A
 * @param Rprime
 * @param N
 */
void invR_times_A_times_invRprime(double* result, double* R, double* A, double* Rprime, int N);

/**
 * @TODO add comment
 *
 * @param result
 * @param U
 * @param L
 * @param E
 * @param N
 */
void invU_times_invL_times_E(double* result, double* U, double* L, double* E, int N);

/**
 * @TODO add comment
 *
 * @param result
 * @param R
 * @param Rprime
 * @param N
 */
void RCtimesRCprime(doublecomplex* result, doublecomplex* R, doublecomplex* Rprime, int N);

/**
 * @TODO add comment
 *
 * @param result
 * @param R
 * @param A
 * @param Rprime
 * @param N
 */
void invRC_times_AC_times_invRCprime(doublecomplex* result, doublecomplex* R, doublecomplex* A, doublecomplex* Rprime, int N);

/**
 * @TODO add comment
 *
 * @param result
 * @param U
 * @param L
 * @param E
 * @param N
 */
void invUC_times_invLC_times_EC(doublecomplex* result, doublecomplex* U, doublecomplex* L, doublecomplex* E, int N);

#endif /* __RTIMESRprime_H__ */
/*--------------------------------------------------------------------------*/

