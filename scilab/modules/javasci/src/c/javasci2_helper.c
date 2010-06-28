/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "api_scilab.h"

double * getDouble(char* variableName, int *nbRow, int *nbCol) {
    SciErr sciErr;
    double * matrixOfDouble = NULL;
    sciErr = readNamedMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol , NULL);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    /* Alloc the memory */
    matrixOfDouble=(double*)malloc(((*nbRow)*(*nbCol))*sizeof(double));

    /* Load the matrix */
    sciErr = readNamedMatrixOfDouble(pvApiCtx, variableName, nbRow, nbCol, matrixOfDouble);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
    }

    return matrixOfDouble;

}


int putDouble(char* variableName, double *variable, int *nbRow, int *nbCol) {
    SciErr sciErr;
    sciErr = createNamedMatrixOfDouble(pvApiCtx,variableName,nbRow,nbCol, variable);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }
    return 0;


}
