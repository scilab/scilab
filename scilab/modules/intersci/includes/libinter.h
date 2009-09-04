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
#include "dynlib_intersci.h"
#include "machine.h" /* C2F */

#include "scisparse.h" /* SciSparse struct */

INTERSCI_IMPEXP int C2F(out)(char *str,long int str_len);
INTERSCI_IMPEXP void C2F(cdouble)(int *n,double *ip[],double *op);
INTERSCI_IMPEXP void C2F(cfloat)(int *n,float *ip[],double *op);
INTERSCI_IMPEXP void C2F(cint)(int *n,int *ip[],double *op);
INTERSCI_IMPEXP void C2F(ccharf) ( int *n, char **ip, int *op);
INTERSCI_IMPEXP void C2F(cchar) ( int *n, char **ip, int *op);
INTERSCI_IMPEXP void C2F(cbool)(int *n,int *ip[],int *op);
INTERSCI_IMPEXP void C2F(cdoublef) ( int *n, double *ip[], double *op);
INTERSCI_IMPEXP void C2F(cintf) ( int *n, int *ip[], double *op);
INTERSCI_IMPEXP void C2F(cfloatf) ( int *n, float *ip[], double *op);
INTERSCI_IMPEXP void C2F(cboolf) ( int *n, int *ip[], int *op);
INTERSCI_IMPEXP void Cout (char *str);
INTERSCI_IMPEXP void C2F(dbl2cdbl)(int *n,double *ip[], double *op);
INTERSCI_IMPEXP void C2F(int2cint)( int *n, int *ip[], int *op);
INTERSCI_IMPEXP void C2F(cstringf) (char ***ip, int *sciptr, int *m, int *n, int *max, int *ierr);
INTERSCI_IMPEXP int C2F(stringc)(int *sciptr,char ***cptr,int *ierr);
INTERSCI_IMPEXP int C2F(csparsef)(SciSparse **x,int *mnel,int *icol,double *R,double *I);
INTERSCI_IMPEXP SciSparse *NewSparse(int *,int *,int *,int *);
INTERSCI_IMPEXP void FreeSparse(SciSparse *x);
INTERSCI_IMPEXP void FreeRhsSVar(char **ptrStr);

#endif /* !__LIBINTER_H__ */
