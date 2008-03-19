/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Bruno JOFRET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LU_H__
#define __LU_H__

#define spINSIDE_SPARSE
#include "spConfig.h"
#include "spmatrix.h"
#include "spDefs.h"

#include "cerro.h"
#include "machine.h"
#include "localization.h"

void C2F(lufact1)(double *val, int *lln, int *col, int *n, int *nel,
		  long *fmat, double *eps, double *releps, int *nrank, int *ierr);

void C2F(lusolve1)(long *fmat, double *b, double *x);

void C2F(ludel1)(long *fmat);

void C2F(lusiz1)(long* fmat, int* lsize, int* usize);

void C2F(luget1)(long *fmat, int *indP, double *P,
		 int *indl, double *l, int *indu, double *u,
		 int *indQ, double *Q);
#endif /* !__LU_H__ */
