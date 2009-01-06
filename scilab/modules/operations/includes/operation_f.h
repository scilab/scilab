/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "machine.h"

#ifndef __OPERATION_F_H__
#define __OPERATION_F_H__

int C2F(dpmul)(double *_pdblP1,int *_piD1,double *_pdblP2,int *_piD2,double *_pdblP3,int *_piD3);
int C2F(dmpmu)(double *mp1,int *d1,int *nl1,double *mp2,int *d2,int *nl2,double *mp3,int *d3,int *l,int *m,int *n);
#endif /* __OPERATION_F_H__ */