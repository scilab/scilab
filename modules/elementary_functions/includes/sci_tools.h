/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCI_TOOLS
#define __SCI_TOOLS
#include "machine.h"
#define DATA_TYPE_SCALAR     1

#define TYPE_DOUBLE  0
#define TYPE_COMPLEX 1

/* Valide for Real or Complex matrix */
#define TYPE(ptr)   (((int*) (ptr))[-1])
#define NB_COL(ptr) (((int*) (ptr))[-2])
#define NB_ROW(ptr) (((int*) (ptr))[-3])
#define DATA_TYPE(ptr) (((int*) (ptr))[-4])

#define SET_DATA_TYPE_SCALAR(ptr)  (((int*) (ptr))[-4] = DATA_TYPE_SCALAR)
#define SET_TYPE_DOUBLE(ptr)  (((int*) (ptr))[-1] = TYPE_DOUBLE)
#define SET_TYPE_COMPLEX(ptr) (((int*) (ptr))[-1] = TYPE_COMPLEX)
#define SET_NB_COL(ptr,m) (((int*) (ptr))[-2] = (m))
#define SET_NB_ROW(ptr,n) (((int*) (ptr))[-3] = (n))

typedef struct {double re, im;} complex16;

extern int  F2C(mycmatptr)(char *name,int *m, int *n, int *type, int *lp);
extern void C2F(ccomplexf)(int *n, double **ip, double *op);
extern void (SciToF77)(double *ptr, int size, int lda);
extern void (F77ToSci)(double *ptr, int size, int lda);

void double2z(double *ptr, double *ptr77z, int size, int lda);
void z2double(double *ptrz, double *ptrsci, int size, int lda);

#endif /* __SCI_TOOLS */
