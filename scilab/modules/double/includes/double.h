/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __DOUBLE_H__
#define __DOUBLE_H__


#include <stdio.h>
#include "basic_functions.h"
#include "stack-c.h"
#include "../../core/src/c/parse.h"

#include "matrix_left_division.h"
#include "matrix_right_division.h"
#include "matrix_multiplication.h"
#include "matrix_transpose.h"

/*Multiplication*/
int matmult();
int matldiv();
int matrdiv();

extern C2F(dlacpy)();
extern C2F(dgetrf)();
extern C2F(dgecon)();
extern C2F(dgetrs)();
extern C2F(dgelsy1)();

extern double C2F(zlange)();
extern C2F(zlacpy)();
extern C2F(zgetrf)();
extern C2F(zgecon)();
extern C2F(zgetrs)();
extern C2F(zgelsy)();
extern C2F(zgelsy1)();

extern int C2F(zscal)();
extern int C2F(dscal)();

#endif //__DOUBLE_H__
