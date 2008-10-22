/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __FEVAL_H__
#define __FEVAL_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
/*--------------------------------------------------------------------------*/
int C2F(parab)(int *nn, double *x1, double *x2,double *xres, int *itype, char *name, long int name_len);
int C2F(parabc)(int *nn, double *x1, double *x2,double *xres, int *itype, char *name, long int name_len);
/*--------------------------------------------------------------------------*/
#endif /* __FEVAL_H__ */
/*--------------------------------------------------------------------------*/
