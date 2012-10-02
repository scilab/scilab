/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - INRIA - Serge STEER
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "api_scilab.h"
#ifndef __SCI_DATA_UTILITIES__
#define __SCI_DATA_UTILITIES__
int getArrayOfDouble(void* pvApiCtx, int *piAddr, int *ndims, int **dims, double **Ar, double **Ai);

SciErr allocArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar);

SciErr allocComplexArrayOfDouble(void* _pvCtx, int _iVar, int ndims, int *dims, double **Ar, double **Ai);

SciErr getScalarIntArg(void* _pvCtx, int _iVar, char *fname, int *value);

SciErr getVectorIntArg(void* _pvCtx, int _iVar, char *fname, int *pndims, int **pDim);

BOOL isHyperMatrixMlist(void* _pvCtx, int *piAddressVar);
#endif /* __SCI_DATA_UTILITIES__ */
/*--------------------------------------------------------------------------*/
