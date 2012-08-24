/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <math.h>
#include <string.h>
#include "stack-c.h"
#include "isanan.h"
#include "core_math.h"
#include "gw_arnoldi.h"
#include "localization.h"
#include "Scierror.h"
#include "api_scilab.h"
#include "stdio.h"
#include "stdlib.h"
#include "sciprint.h"
#include "doublecomplex.h"
#include "MALLOC.h"
#include "eigs.h"

int sci_eigs(char *fname, unsigned long fname_len)
{
    int *piAddressVarOne	= NULL;
    int iRowsOne			= 0;
    int iColsOne			= 0;
    double elemt1			= 0;
    double elemt2			= 0;
    double* Areal			= NULL;
    doublecomplex* Acplx	= NULL;
    int Asym				= 1;
    int Acomplex			= 0;
    int N					= 0;

    int *piAddressVarTwo	= NULL;
    int iTypeVarTwo			= 0;
    int iRowsTwo			= 0;
    int iColsTwo			= 0;
    double* Breal			= NULL;
    doublecomplex* Bcplx	= NULL;
    int Bcomplex			= 0;
    int matB				= 0;

    int *piAddressVarThree	= NULL;
    double dblNEV			= 0;
    int iNEV				= 0;

    int *piAddressVarFour	= NULL;
    int iTypeVarFour		= 0;
    int iRowsFour			= 0;
    int iColsFour			= 0;
    int iLen				= 0;
    char* pstData			= NULL;
    doublecomplex* SIGMA	= NULL;

    int *piAddressVarFive	= NULL;
    double dblMAXITER		= 0;

    int *piAddressVarSix	= NULL;
    double dblTOL			= 0;

    int *piAddressVarSeven	= NULL;
    int TypeVarSeven		= 0;
    int RowsSeven			= 0;
    int ColsSeven			= 0;
    double* dblNCV			= NULL;

    int *piAddressVarEight	= NULL;
    int iTypeVarEight       = 0;
    double dblCHOLB			= 0;
    int iCHOLB              = 0;

    int *piAddressVarNine	= NULL;
    int iTypeVarNine		= 0;
    int iRowsNine			= 0;
    int iColsNine			= 0;
    double* RESID			= NULL;
    doublecomplex* RESIDC	= NULL;

    int *piAddressVarTen	= NULL;
    int iINFO				= 0;

    // Output arguments
    doublecomplex* eigenvalue		= NULL;
    doublecomplex* mat_eigenvalue	= NULL;
    doublecomplex* eigenvector		= NULL;
    int INFO_EUPD					= 0;
    int error						= 0;

    SciErr sciErr;
    int iErr				= 0;
    int i					= 0;
    int j					= 0;

    CheckRhs(1, 10);
    CheckLhs(0, 2);

    /****************************************
    *    	First variable : A    		*
    *****************************************/

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &iRowsOne, &iColsOne);
    //check if A is a square matrix
    if (iRowsOne * iColsOne == 1 || iRowsOne != iColsOne)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A square matrix expected.\n"), "eigs", 1);
        return 0;
    }

    N = iRowsOne;

    //check if A is complex
    if (isVarComplex(pvApiCtx, piAddressVarOne))
    {
        sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarOne, &iRowsOne, &iColsOne, &Acplx);
        Acomplex = 1;
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &iRowsOne, &iColsOne, &Areal);

        for (i = 0; i < iColsOne; i++)
        {
            for (j = 0; j < i; j++)
            {
                elemt1 = Areal[j + i * iColsOne];
                elemt2 = Areal[j * iColsOne + i];
                if (fabs(elemt1 - elemt2) > 0)
                {
                    Asym = 0;
                    break;
                }
            }
            if (Asym == 0)
            {
                break;
            }
        }
    }

    /****************************************
    *    	Second variable : B    		*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iTypeVarTwo);
    if (sciErr.iErr || iTypeVarTwo != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: An empty matrix or full or sparse square matrix expected.\n"), "eigs", 2);
        return 0;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &iRowsTwo, &iColsTwo);
    if (iRowsTwo * iColsTwo == 1 || iRowsTwo != iColsTwo)
    {
        Scierror(999, _("%s: Wrong dimension for input argument #%d: B must have the same size as A.\n"), "eigs", 2);
        return 0;
    }

    matB = iRowsTwo * iColsTwo;
    if (isVarComplex(pvApiCtx, piAddressVarTwo))
    {
        sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarTwo, &iRowsTwo, &iColsTwo, &Bcplx);
        Bcomplex = 1;
    }
    else
    {
        sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &iRowsTwo, &iColsTwo, &Breal);
    }

    if (matB != 0)
    {
        if (Acomplex && !Bcomplex)
        {
            Bcplx = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            memset(Bcplx, 0, N * N * sizeof(doublecomplex));
            Bcomplex = 1;
            for (i = 0 ; i < N * N ;  i++)
            {
                Bcplx[i].r = Breal[i];
            }
        }
        if (!Acomplex && Bcomplex)
        {
            Acplx = (doublecomplex*)malloc(N * N * sizeof(doublecomplex));
            memset(Acplx, 0, N * N * sizeof(doublecomplex));
            Acomplex = 1;
            for (i = 0 ; i < N * N ;  i++)
            {
                Acplx[i].r = Areal[i];
            }
        }
    }


    /****************************************
    *    			NEV   					*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
        free(Acplx);
        return 0;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarThree, &dblNEV);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 3);
        return 0;
    }

    if (dblNEV != floor(dblNEV) || (dblNEV <= 0))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: k must be a positive integer.\n"), "eigs", 3);
        return 0;
    }

    if (!finite(dblNEV))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: k must be in the range 1 to N.\n"), "eigs", 3);
        return 0;
    }


    iNEV = (int)dblNEV;

    /****************************************
    *    		SIGMA AND WHICH    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarFour, &iTypeVarFour);
    if (sciErr.iErr || (iTypeVarFour != sci_matrix && iTypeVarFour != sci_strings))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 4);
        return 0;
    }

    if (iTypeVarFour == sci_strings)
    {
        int iErr = getAllocatedSingleString(pvApiCtx, piAddressVarFour, &pstData);
        if (iErr)
        {
            return 0;
        }

        if (strcmp(pstData, "LM") != 0 && strcmp(pstData, "SM") != 0  && strcmp(pstData, "LR") != 0 && strcmp(pstData, "SR") != 0 && strcmp(pstData, "LI") != 0
                && strcmp(pstData, "SI") != 0 && strcmp(pstData, "LA") != 0 && strcmp(pstData, "SA") != 0 && strcmp(pstData, "BE") != 0)
        {
            if (!Acomplex && Asym)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of '%s', '%s', '%s', '%s' or '%s'.\n" ),
                    "eigs", 4, "LM", "SM", "LA", "SA", "BE");
                return 0;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of '%s', '%s', '%s', '%s', '%s' or '%s'.\n " ), 
                    "eigs", 4, "LM", "SM", "LR", "SR", "LI", "SI");
                return 0;
            }
        }

        if ((Acomplex || !Asym) && (strcmp(pstData, "LA") == 0 || strcmp(pstData, "SA") == 0 || strcmp(pstData, "BE") == 0))
        {
            Scierror(999, _("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs", 4);
            return 0;
        }

        if (!Acomplex && Asym && (strcmp(pstData, "LR") == 0 || strcmp(pstData, "SR") == 0 || strcmp(pstData, "LI") == 0 || strcmp(pstData, "SI") == 0))
        {
            Scierror(999, _("%s: Invalid sigma value for real symmetric problem.\n"), "eigs", 4);
            return 0;
        }

        SIGMA = (doublecomplex*)malloc(1 * sizeof(doublecomplex));
        SIGMA[0].r = 0;
        SIGMA[0].i = 0;
    }

    if (iTypeVarFour == sci_matrix)
    {
        sciErr = getVarDimension(pvApiCtx, piAddressVarFour, &iRowsFour, &iColsFour);
        if (iRowsFour * iColsFour != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 4);
            return 0;
        }

        SIGMA = (doublecomplex*)malloc(1 * sizeof(doublecomplex));
        sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarFour, &iRowsFour, &iColsFour, &SIGMA);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (C2F(isanan)(&SIGMA[0].r) || C2F(isanan)(&SIGMA[0].i))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: sigma must be a real.\n"), "eigs", 4);
            return 0;
        }

        pstData = "LM";
    }

    /****************************************
    *    			MAXITER    				*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
        free(SIGMA);
        return 0;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarFive, &dblMAXITER);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a scalar.\n"), "eigs", 5, "opts.maxiter");
        return 0;
    }

    if ((dblMAXITER != floor(dblMAXITER)) || (dblMAXITER <= 0))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer positive value.\n"), "eigs", 5, "opts.maxiter");
        return 0;
    }

    /****************************************
    *    				TOL	    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarSix);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
        return 0;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarSix, &dblTOL);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 6, "opts.tol");
        return 0;
    }

    if (C2F(isanan)(&dblTOL))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 6, "opts.tol");
        return 0;
    }

    /****************************************
    *    				NCV	    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddressVarSeven);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarSeven, &TypeVarSeven);
    if (sciErr.iErr || TypeVarSeven != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
        return 0;
    }
    else
    {
        if (isVarComplex(pvApiCtx, piAddressVarSeven))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
            0;
        }
        else
        {
            sciErr = getVarDimension(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven);
            if (RowsSeven * ColsSeven > 1)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
                return 0;
            }

            if (RowsSeven * ColsSeven == 1)
            {
                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven, &dblNCV);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
                    return 0;
                }

                if (dblNCV[0] != floor(dblNCV[0]))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
                    return 0;
                }
            }
        }
    }

    /****************************************
    *    			CHOLB    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddressVarEight);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 8);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarEight, &iTypeVarEight);
    if (sciErr.iErr || iTypeVarEight != sci_matrix && iTypeVarEight != sci_boolean)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
        return 0;
    }

    if(iTypeVarEight == sci_boolean)
    {
        iErr = getScalarBoolean(pvApiCtx, piAddressVarEight, &iCHOLB);
        if (iErr)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
            return 0;
        }

        if(iCHOLB != 1 && iCHOLB != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f","%t");
            return 0;
        }
        dblCHOLB = (double) iCHOLB;
    }
    
    if(iTypeVarEight == sci_matrix)
    {
        iErr = getScalarDouble(pvApiCtx, piAddressVarEight, &dblCHOLB);
        if (iErr)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
            return 0;
        }
        
        if(dblCHOLB != 1 && dblCHOLB != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB","%f","%t");
            return 0;
        }
    }

    /****************************************
    *    			RESID    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddressVarNine);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarNine, &iTypeVarNine);
    if (sciErr.iErr || iTypeVarNine != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"), "eigs", 9);
        return 0;
    }
    else
    {
        sciErr = getVarDimension(pvApiCtx, piAddressVarNine, &iRowsNine, &iColsNine);
        if (iRowsNine*iColsNine == 1 || iRowsNine*iColsNine != N)
        {
            Scierror(999, _("%s: Wrong dimension for input argument #%d: Start vector %s must be N by 1.\n"), "eigs", 9, "opts.resid");
            return 0;
        }
    }

    if (!Acomplex && !Bcomplex)
    {
        if (isVarComplex(pvApiCtx, piAddressVarNine))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Start vector %s must be real for real problems.\n"), "eigs", 9, "opts.resid");
            return 0;
        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarNine, &iRowsNine, &iColsNine, &RESID);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), "eigs", 9);
                return 0;
            }
        }
    }
    else
    {
        sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddressVarNine, &iRowsNine, &iColsNine, &RESIDC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), "eigs", 9);
            return 0;
        }
    }

    /****************************************
    *    			INFO    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddressVarTen);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), "eigs", 9);
        return 0;
    }

    iErr = getScalarInteger32(pvApiCtx, piAddressVarTen, &iINFO);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), "eigs", 1);
        return 0;
    }

    // Initialization output arguments
    eigenvalue = (doublecomplex*) malloc((iNEV + 1) * sizeof(doublecomplex));
    memset(eigenvalue, 0, (iNEV + 1) * sizeof(doublecomplex));

    if (Lhs > 1)
    {
        mat_eigenvalue = (doublecomplex*) malloc((iNEV + 1) * (iNEV + 1) * sizeof(doublecomplex));
        memset(mat_eigenvalue, 0, (iNEV + 1) * (iNEV + 1) * sizeof(doublecomplex));

        eigenvector = (doublecomplex*) malloc(N * (iNEV + 1) * sizeof(doublecomplex));
        memset(eigenvector, 0, N * (iNEV + 1) * sizeof(doublecomplex));
    }

    error = eigs(Areal, Acplx, N, Acomplex, Asym, Breal, Bcplx, Bcomplex, matB, iNEV, SIGMA, pstData, &dblMAXITER,
                 &dblTOL, dblNCV, RESID, RESIDC, &iINFO, &dblCHOLB, INFO_EUPD, eigenvalue, eigenvector);

    switch (error)
    {
        case -1 :
            if (Asym && !Acomplex && !Bcomplex)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: For real symmetric problems, NCV must be k < NCV <= N.\n"), "eigs", 7);
            }
            else
            {
                if (!Asym && !Acomplex && !Bcomplex)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: For real non symmetric problems, NCV must be k + 2 < NCV <= N.\n"), "eigs", 7);
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: For complex problems, NCV must be k + 1 < NCV <= N.\n"), "eigs", 7);
                }
            }
            PutLhsVar();
            return 0;

        case -2 :
            if (Asym && !Acomplex && !Bcomplex)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: For real symmetric problems, k must be an integer in the range 1 to N - 1.\n"), "eigs", 3);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: For real non symmetric or complex problems, k must be an integer in the range 1 to N - 2.\n"), "eigs", 3);
            }
            PutLhsVar();
            return 0;

        case -3 :
            Scierror(999, _("%s: Wrong type for input argument #%d: B must be symmetric or hermitian, definite, semi positive.\n"), "eigs", 2);
            PutLhsVar();
            return 0;

        case -4 :
            if (!Acomplex && !Bcomplex)
            {
                if (Asym)
                {
                    Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "DSAUPD", iINFO);
                }
                else
                {
                    Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "DNAUPD", iINFO);
                }
            }
            else
            {
                Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "ZNAUPD", iINFO);
            }
            PutLhsVar();
            return 0;

        case -5 :
            if (!Acomplex && !Bcomplex)
            {
                if (Asym)
                {
                    Scierror(999, _("%s: Error with %s: unknown mode returned.\n"), "eigs", "DSAUPD");
                }
                else
                {
                    Scierror(999, _("%s: Error with %s: unknown mode returned.\n"), "eigs", "DNAUPD");
                }
            }
            else
            {
                Scierror(999, _("%s: Error with %s: unknown mode returned.\n"), "eigs", "ZNAUPD");
            }
            PutLhsVar();
            return 0;

        case -6 :
            if (!Acomplex && !Bcomplex)
            {
                if (Asym)
                {
                    Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "DSEUPD", INFO_EUPD);
                }
                else
                {
                    Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "DNEUPD", INFO_EUPD);
                }
            }
            else
            {
                Scierror(999, _("%s: Error with %s: info = %d \n"), "eigs", "ZNEUPD", INFO_EUPD);
            }
            PutLhsVar();
            free(mat_eigenvalue);
            return 0;
    }

    if (Lhs <= 1)
    {
        sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs + 1, iNEV, 1, eigenvalue);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
    }
    else
    {
        // create a matrix which contains the eigenvalues
        for (i = 0; i < iNEV ; i++)
        {
            mat_eigenvalue[i * iNEV + i].r = eigenvalue[i].r;
            mat_eigenvalue[i * iNEV + i].i = eigenvalue[i].i;
        }

        sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs + 1, iNEV, iNEV, mat_eigenvalue);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        sciErr = createComplexZMatrixOfDouble(pvApiCtx, Rhs + 2, N, iNEV, eigenvector);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        LhsVar(2) = Rhs + 2;
    }

    if (iTypeVarFour == sci_strings)
    {
        freeAllocatedSingleString(pstData);
    }

    free(SIGMA);

    free(eigenvalue);

    if (matB != 0)
    {
        if (Acomplex && !Bcomplex)
        {
            free(Bcplx);
        }
        if (!Acomplex && Bcomplex)
        {
            free(Acplx);
        }
    }

    if (Lhs > 1)
    {
        free(mat_eigenvalue);
        free(eigenvector);
    }
    PutLhsVar();
    return 0;
}

