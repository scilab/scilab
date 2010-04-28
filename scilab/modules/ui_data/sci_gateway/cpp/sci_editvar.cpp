/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan SIMON
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <iostream>
#include "EditVar.hxx"

extern "C"
{
#include "gw_ui_data.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "sciprint.h"
#include "getScilabJavaVM.h"
}

using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
int sci_editvar(char *fname,unsigned long fname_len)
{
    CheckRhs(1,1); /* TODO change this in the future */
    CheckLhs(0,1);    
    SciErr sciErr; 

    int *piAddr = NULL;
    int iComplex = 0;
    int iType = 0;
    int iCols = 0;
    int iRows = 0;
    double *pdblReal = NULL;
    double *pdblImg = NULL;
    double **ppdblRealMatrix = NULL;
    /* get address */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);

    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    /* get type */
    sciErr = getVarType(pvApiCtx, piAddr, &iType);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    if ( iType == sci_matrix )
    {
        /* get complexity */
        iComplex    = isVarComplex(pvApiCtx, piAddr);

        /* check complexity */
        if(iComplex)
        {
            /* get size and data from Scilab memory */
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
        }
        else
        {
            /* get size and data from Scilab memory */
            sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
        }

        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

    } else {
        Scierror(42,"Type not handle by editvar yet");
        return 0;
    }
    
    /* 
     * we need this to make the links between the API (which return a double*)
     * and the JNI which needs a double**
     */
    ppdblRealMatrix = new double*[iRows];
    for (int i = 0; i < iRows; ++i)
    {
        ppdblRealMatrix[i] = &pdblReal[i*iCols];

    }

    /* Launch Java Variable Editor through JNI */
    EditVar::openVariableEditor(getScilabJavaVM(), ppdblRealMatrix, iRows, iCols);
    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
