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

void RtimesRprime(double*, double *, double*, int);

void invR_times_A_times_invRprime(double *, double*, double*, double*, int);

void invU_times_invL_times_E(double*, double*, double*, double*, int);

void RCtimesRCprime(doublecomplex*, doublecomplex *, doublecomplex*, int);

void invRC_times_AC_times_invRCprime(doublecomplex *, doublecomplex*, doublecomplex*, doublecomplex*, int);

void invUC_times_invLC_times_EC(doublecomplex*, doublecomplex*, doublecomplex*, doublecomplex*, int);

#endif /* __RTIMESRprime_H__ */
/*--------------------------------------------------------------------------*/

