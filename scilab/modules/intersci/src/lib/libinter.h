/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __LIBINTER_H__
#define __LIBINTER_H__

#include "machine.h"
#include "stack-c.h"

void C2F(out) (char *str,unsigned int);
void C2F(ccharf) ( int *n, char **ip, int *op);
void C2F(cchar) ( int *n, char **ip, int *op);
void C2F(cdoublef) ( int *n, double *ip[], double *op);
void C2F(cintf) ( int *n, int *ip[], double *op);
void C2F(cfloatf) ( int *n, float *ip[], double *op);
void C2F(cboolf) ( int *n, int *ip[], int *op);
void Cout (char *str);
void C2F(dbl2cdbl)(int *n,double *ip[], double *op);
void C2F(int2cint)( int *n, integer *ip[], integer *op);
int C2F(csparsef)(SciSparse **x,int *mnel,int *icol,double *R,double *I);
SciSparse *NewSparse(int *,int *,int *,int *);
void FreeSparse(SciSparse *x);
void FreeRhsSVar(char **ptrStr);

#endif /* !__LIBINTER_H__ */
