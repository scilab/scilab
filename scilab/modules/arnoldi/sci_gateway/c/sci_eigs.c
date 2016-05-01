/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - Scilab Enterprises - Adeline CARNIS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include <math.h>
#include <string.h>

#include "doublecomplex.h"
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
#include "sci_malloc.h"
#include "eigs.h"

#define FREE_AB \
    if (isVarComplex(pvApiCtx, piAddressVarOne) && !isVarComplex(pvApiCtx, piAddressVarTwo)) FREE(Bcplx);   \
    if (!isVarComplex(pvApiCtx, piAddressVarOne) && isVarComplex(pvApiCtx, piAddressVarTwo)) FREE(Acplx);

#define FREE_PSTDATA \
    if (iTypeVarFour == sci_strings) freeAllocatedSingleString(pstData);

int sci_eigs(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

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
    char* pstData			= NULL;
    doublecomplex SIGMA;

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
    int RVEC                = 0;
    // Output arguments
    double* eigenvalue      = NULL;
    double* eigenvector     = NULL;
    doublecomplex* eigenvalueC  = NULL;
    doublecomplex* eigenvectorC	= NULL;

    double* mat_eigenvalue	= NULL;
    doublecomplex* mat_eigenvalueC  = NULL;
    int INFO_EUPD					= 0;
    int error						= 0;

    int iErr				= 0;
    int i					= 0;
    int j					= 0;

    CheckInputArgument(pvApiCtx, 1, 10);
    CheckOutputArgument(pvApiCtx, 0, 2);

    /****************************************
    *    	First variable : A    		*
    *****************************************/

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &iRowsOne, &iColsOne);
    //check if A is a square matrix
    if (iRowsOne * iColsOne == 1 || iRowsOne != iColsOne)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A square matrix expected.\n"), "eigs", 1);
        return 1;
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
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iTypeVarTwo);
    if (sciErr.iErr || iTypeVarTwo != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: An empty matrix or full or sparse square matrix expected.\n"), "eigs", 2);
        return 1;
    }

    sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &iRowsTwo, &iColsTwo);
    matB = iRowsTwo * iColsTwo;
    if (matB && (iRowsTwo != iRowsOne || iColsTwo != iColsOne))
    {
        Scierror(999, _("%s: Wrong dimension for input argument #%d: B must have the same size as A.\n"), "eigs", 2);
        return 1;
    }

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
            Bcplx = (doublecomplex*)MALLOC(N * N * sizeof(doublecomplex));
            memset(Bcplx, 0, N * N * sizeof(doublecomplex));
            Bcomplex = 1;
            for (i = 0 ; i < N * N ;  i++)
            {
                Bcplx[i].r = Breal[i];
            }
        }
        if (!Acomplex && Bcomplex)
        {
            Acplx = (doublecomplex*)MALLOC(N * N * sizeof(doublecomplex));
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
        FREE_AB;
        return 1;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarThree, &dblNEV);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 3);
        FREE_AB;
        return 1;
    }

    if (isVarComplex(pvApiCtx, piAddressVarThree))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 3);
        FREE_AB;
        return 1;
    }

    if (dblNEV != floor(dblNEV) || (dblNEV <= 0))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: k must be a positive integer.\n"), "eigs", 3);
        FREE_AB;
        return 1;
    }

    if (!finite(dblNEV))
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: k must be in the range 1 to N.\n"), "eigs", 3);
        FREE_AB;
        return 1;
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
        FREE_AB;
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarFour, &iTypeVarFour);
    if (sciErr.iErr || (iTypeVarFour != sci_matrix && iTypeVarFour != sci_strings))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 4);
        FREE_AB;
        return 1;
    }

    if (iTypeVarFour == sci_strings)
    {
        int iErr = getAllocatedSingleString(pvApiCtx, piAddressVarFour, &pstData);
        if (iErr)
        {
            FREE_AB;
            return 1;
        }

        if (strcmp(pstData, "LM") != 0 && strcmp(pstData, "SM") != 0  && strcmp(pstData, "LR") != 0 && strcmp(pstData, "SR") != 0 && strcmp(pstData, "LI") != 0
                && strcmp(pstData, "SI") != 0 && strcmp(pstData, "LA") != 0 && strcmp(pstData, "SA") != 0 && strcmp(pstData, "BE") != 0)
        {
            if (!Acomplex && Asym)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of '%s', '%s', '%s', '%s' or '%s'.\n" ),
                         "eigs", 4, "LM", "SM", "LA", "SA", "BE");
                freeAllocatedSingleString(pstData);
                return 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Unrecognized sigma value.\n Sigma must be one of '%s', '%s', '%s', '%s', '%s' or '%s'.\n " ),
                         "eigs", 4, "LM", "SM", "LR", "SR", "LI", "SI");
                FREE_AB;
                freeAllocatedSingleString(pstData);
                return 1;
            }
        }

        if ((Acomplex || !Asym) && (strcmp(pstData, "LA") == 0 || strcmp(pstData, "SA") == 0 || strcmp(pstData, "BE") == 0))
        {
            Scierror(999, _("%s: Invalid sigma value for complex or non symmetric problem.\n"), "eigs", 4);
            FREE_AB;
            freeAllocatedSingleString(pstData);
            return 1;
        }

        if (!Acomplex && Asym && (strcmp(pstData, "LR") == 0 || strcmp(pstData, "SR") == 0 || strcmp(pstData, "LI") == 0 || strcmp(pstData, "SI") == 0))
        {
            Scierror(999, _("%s: Invalid sigma value for real symmetric problem.\n"), "eigs", 4);
            freeAllocatedSingleString(pstData);
            return 1;
        }

        SIGMA.r = 0;
        SIGMA.i = 0;
    }

    if (iTypeVarFour == sci_matrix)
    {
        sciErr = getVarDimension(pvApiCtx, piAddressVarFour, &iRowsFour, &iColsFour);
        if (iRowsFour * iColsFour != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "eigs", 4);
            FREE_AB;
            return 1;
        }

        if (getScalarComplexDouble(pvApiCtx, piAddressVarFour, &SIGMA.r, &SIGMA.i))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            FREE_AB;
            return 1;
        }

        if (C2F(isanan)(&SIGMA.r) || C2F(isanan)(&SIGMA.i))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: sigma must be a real.\n"), "eigs", 4);
            FREE_AB;
            return 1;
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
        FREE_AB;
        FREE_PSTDATA;
        return 0;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarFive, &dblMAXITER);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a scalar.\n"), "eigs", 5, "opts.maxiter");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    if ((dblMAXITER != floor(dblMAXITER)) || (dblMAXITER <= 0))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer positive value.\n"), "eigs", 5, "opts.maxiter");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    /****************************************
    *    				TOL	    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddressVarSix);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 6);
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    iErr = getScalarDouble(pvApiCtx, piAddressVarSix, &dblTOL);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 6, "opts.tol");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    if (C2F(isanan)(&dblTOL))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be a real scalar.\n"), "eigs", 6, "opts.tol");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    /****************************************
    *    				NCV	    			*
    *****************************************/
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddressVarSeven);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarSeven, &TypeVarSeven);
    if (sciErr.iErr || TypeVarSeven != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }
    else
    {
        if (isVarComplex(pvApiCtx, piAddressVarSeven))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
        }
        else
        {
            sciErr = getVarDimension(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven);
            if (RowsSeven * ColsSeven > 1)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
                FREE_AB;
                FREE_PSTDATA;
                return 1;
            }

            if (RowsSeven * ColsSeven == 1)
            {
                sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarSeven, &RowsSeven, &ColsSeven, &dblNCV);
                if (sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
                    FREE_AB;
                    FREE_PSTDATA;
                    return 1;
                }

                if (dblNCV[0] != floor(dblNCV[0]))
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar.\n"), "eigs", 7, "opts.ncv");
                    FREE_AB;
                    FREE_PSTDATA;
                    return 1;
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
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarEight, &iTypeVarEight);
    if (sciErr.iErr || (iTypeVarEight != sci_matrix && iTypeVarEight != sci_boolean))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    if (iTypeVarEight == sci_boolean)
    {
        iErr = getScalarBoolean(pvApiCtx, piAddressVarEight, &iCHOLB);
        if (iErr)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
            FREE_AB;
            FREE_PSTDATA;
            return 1;
        }

        if (iCHOLB != 1 && iCHOLB != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB", "%f", "%t");
            FREE_AB;
            FREE_PSTDATA;
            return 1;
        }
        dblCHOLB = (double) iCHOLB;
    }

    if (iTypeVarEight == sci_matrix)
    {
        iErr = getScalarDouble(pvApiCtx, piAddressVarEight, &dblCHOLB);
        if (iErr)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: %s must be an integer scalar or a boolean.\n"), "eigs", 8, "opts.cholB");
            FREE_AB;
            FREE_PSTDATA;
            return 1;
        }

        if (dblCHOLB != 1 && dblCHOLB != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: %s must be %s or %s.\n"), "eigs", 8, "opts.cholB", "%f", "%t");
            FREE_AB;
            FREE_PSTDATA;
            return 1;
        }
    }

    if ( dblCHOLB ) // check that B is upper triangular with non zero element on the diagonal
    {
        if (!Bcomplex)
        {
            for (i = 0; i < N; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    if (i == j && Breal[i + j * N] == 0)
                    {
                        Scierror(999, _("%s: B is not positive definite. Try with sigma='SM' or sigma=scalar.\n"), "eigs");
                        FREE_PSTDATA;
                        return 0;
                    }
                    else
                    {
                        if ( j < i && Breal[i + j * N] != 0 )
                        {
                            Scierror(999, _("%s: If opts.cholB is true, B should be upper triangular.\n"), "eigs");
                            FREE_PSTDATA;
                            return 0;
                        }
                    }
                }
            }
        }
        else
        {
            for (i = 0; i < N; i++)
            {
                for (j = 0; j <= i; j++)
                {
                    if (i == j && Bcplx[i + i * N].r == 0 && Bcplx[i + i * N].i == 0)
                    {
                        Scierror(999, _("%s: B is not positive definite. Try with sigma='SM' or sigma=scalar.\n"), "eigs");
                        FREE_AB;
                        FREE_PSTDATA;
                        return 0;
                    }
                    else
                    {
                        if ( j < i && (Bcplx[i + j * N].r != 0 || Bcplx[i + j * N].i != 0) )
                        {
                            Scierror(999, _("%s: If opts.cholB is true, B should be upper triangular.\n"), "eigs");
                            FREE_AB;
                            FREE_PSTDATA;
                            return 0;
                        }
                    }
                }
            }
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
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarNine, &iTypeVarNine);
    if (sciErr.iErr || iTypeVarNine != sci_matrix)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Wrong type for input argument #%d: A real or complex matrix expected.\n"), "eigs", 9);
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }
    else
    {
        sciErr = getVarDimension(pvApiCtx, piAddressVarNine, &iRowsNine, &iColsNine);
        if (iRowsNine * iColsNine == 1 || iRowsNine * iColsNine != N)
        {
            Scierror(999, _("%s: Wrong dimension for input argument #%d: Start vector %s must be N by 1.\n"), "eigs", 9, "opts.resid");
            FREE_AB;
            FREE_PSTDATA;
            return 1;
        }
    }

    if (!Acomplex && !Bcomplex)
    {
        if (isVarComplex(pvApiCtx, piAddressVarNine))
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Start vector %s must be real for real problems.\n"), "eigs", 9, "opts.resid");
            FREE_PSTDATA;
            return 1;
        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarNine, &iRowsNine, &iColsNine, &RESID);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), "eigs", 9);
                FREE_PSTDATA;
                return 1;
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
            FREE_AB;
            FREE_PSTDATA;
            return 1;
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
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    iErr = getScalarInteger32(pvApiCtx, piAddressVarTen, &iINFO);
    if (iErr)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: An integer expected.\n"), "eigs", 1);
        FREE_AB;
        FREE_PSTDATA;
        return 1;
    }

    // Initialization output arguments
    if (nbOutputArgument(pvApiCtx) > 1)
    {
        RVEC = 1;
    }

    if (Acomplex || Bcomplex || !Asym)
    {
        eigenvalueC = (doublecomplex*)CALLOC((iNEV + 1), sizeof(doublecomplex));
        if (RVEC)
        {
            eigenvectorC = (doublecomplex*)CALLOC(N * (iNEV + 1), sizeof(doublecomplex));
        }
    }
    else
    {
        eigenvalue = (double*)CALLOC(iNEV, sizeof(double));
        /* we should allocate eigenvector only if RVEC is true, but dseupd segfaults
         if Z is not allocated even when RVEC is false, contrary to the docs.*/
        eigenvector = (double*)CALLOC(iNEV * N, sizeof(double));
    }

    error = eigs(Areal, Acplx, N, Acomplex, Asym, Breal, Bcplx, Bcomplex, matB, iNEV, SIGMA, pstData, &dblMAXITER,
                 &dblTOL, dblNCV, RESID, RESIDC, &iINFO, &dblCHOLB, INFO_EUPD, eigenvalue, eigenvector, eigenvalueC, eigenvectorC, RVEC);

    FREE_AB;
    FREE_PSTDATA;

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
            ReturnArguments(pvApiCtx);
            return 1;

        case -2 :
            if (Asym && !Acomplex && !Bcomplex)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: For real symmetric problems, k must be an integer in the range 1 to N - 1.\n"), "eigs", 3);
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: For real non symmetric or complex problems, k must be an integer in the range 1 to N - 2.\n"), "eigs", 3);
            }
            ReturnArguments(pvApiCtx);
            return 1;

        case -3 :
            Scierror(999, _("%s: Error with input argument #%d: B is not positive definite. Try with sigma='SM' or sigma=scalar.\n"), "eigs", 2);
            ReturnArguments(pvApiCtx);
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
            ReturnArguments(pvApiCtx);
            return 1;

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
            ReturnArguments(pvApiCtx);
            return 1;

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
                Scierror(999,  _("%s: Error with %s: info = %d \n"), "eigs", "ZNEUPD", INFO_EUPD);
            }
            ReturnArguments(pvApiCtx);
            FREE(mat_eigenvalue);
            return 1;
    }

    if (nbOutputArgument(pvApiCtx) <= 1)
    {
        if (eigenvalue)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNEV, 1, eigenvalue);
            FREE(eigenvalue);
            FREE(eigenvector);
        }
        else if (eigenvalueC)
        {
            sciErr = createComplexZMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNEV, 1, eigenvalueC);
            FREE(eigenvalueC);
        }

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    }
    else
    {
        // create a matrix which contains the eigenvalues
        if (eigenvalue)
        {
            mat_eigenvalue = (double*)CALLOC(iNEV * iNEV, sizeof(double));
            for (i = 0; i < iNEV; i++)
            {
                mat_eigenvalue[i * iNEV + i] = eigenvalue[i];
            }
            sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNEV, iNEV, mat_eigenvalue);
            FREE(eigenvalue);
            FREE(mat_eigenvalue);
        }
        else if (eigenvalueC)
        {
            mat_eigenvalueC = (doublecomplex*)CALLOC(iNEV * iNEV, sizeof(doublecomplex));
            for (i = 0; i < iNEV; i++)
            {
                mat_eigenvalueC[i * iNEV + i] = eigenvalueC[i];
            }
            sciErr = createComplexZMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iNEV, iNEV, mat_eigenvalueC);
            FREE(eigenvalueC);
            FREE(mat_eigenvalueC);
        }

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        if (eigenvector)
        {
            sciErr = createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, N, iNEV, eigenvector);
            FREE(eigenvector);
        }
        else if (eigenvectorC)
        {
            sciErr = createComplexZMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 2, N, iNEV, eigenvectorC);
            FREE(eigenvectorC);
        }

        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
