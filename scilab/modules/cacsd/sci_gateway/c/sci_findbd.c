/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
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

//RELEASE 4.0, WGS COPYRIGHT 2000.
//
//scilab call:
//  [(x0)(,B(,D))(,V)(,rcnd)] = findBD(jobx0,comuse(,job),A(,B),C(,D),Y
//                                     (,U,tol,printw,ldwork))
//
//    [x0,B,V,rcnd] = findBD(1,1,1,A,C,Y,U)
//  [x0,B,D,V,rcnd] = findBD(1,1,2,A,C,Y,U)
//       [B,V,rcnd] = findBD(0,1,1,A,C,Y,U)
//     [B,D,V,rcnd] = findBD(0,1,2,A,C,Y,U)
//      [x0,V,rcnd] = findBD(1,2,1,A,B,C,Y,U)
//      [x0,V,rcnd] = findBD(1,2,2,A,B,C,D,Y,U)
//        [x0,rcnd] = findBD(0,2)                  (x0 = 0, rcnd = 1)
//      [x0,V,rcnd] = findBD(1,3,A,C,Y)
//
//Purpose:
//  To estimate the initial state and/or the matrices B and D of a
//  discrete-time system, given the system matrices A, C, and possibly
//  B, D, and the input and output trajectories of the system.
//
//Input parameters:
//  jobx0 - integer option to specify whether or not the initial state
//          should be computed:
//          = 1 : compute the initial state x(0);
//          = 0 : do not compute the initial state (possibly, because
//                x(0) is known to be zero).
//  comuse- integer option to specify whether the system matrices B
//          and D should be computed or used:
//          = 1 : compute the matrices B and D, as specified by job;
//          = 2 : use the matrices B and D, as specified by job;
//          = 3 : do not compute/use the matrices B and D.
//  job   - integer option to determine which of the system matrices
//          B and D should be computed or used:
//          = 1 : compute/use the matrix B only (D is known to be zero);
//          = 2 : compute/use the matrices B and D.
//          job must not be specified if jobx0 = 0 and comuse = 2, or
//          if comuse = 3.
//  A     - the n-by-n system state matrix A.
//  B     - the n-by-m system input matrix B (input when jobx0 = 1 and
//          comuse = 2).
//  C     - the l-by-n system output matrix C.
//  D     - the l-by-m system matrix D (input when jobx0 = 1,
//       comuse = 2
//          and job = 2).
//  Y     - the t-by-l output-data sequence matrix.  Column  j  of  Y
//          contains the  t  values of the j-th output component for
//          consecutive time increments.
//  U     - the t-by-m input-data sequence matrix (input when jobx0 = 1
//          and comuse = 2, or comuse = 1).  Column  j  of  U
//          contains the  t  values of the j-th input component for
//          consecutive time increments.
//  tol   - (optional) tolerance used for estimating the rank of
//          matrices. If  tol > 0,  then the given value of  tol  is
//          used as a lower bound for the reciprocal condition number;
//          an m-by-n matrix whose estimated condition number is less
//          than  1/tol  is considered to be of full rank.
//          Default: m*n*epsilon_machine where epsilon_machine is the
//          relative machine precision.
//  printw- (optional) switch for printing the warning messages.
//          = 1:  print warning messages;
//          = 0:  do not print warning messages.
//          Default:    printw = 0.
//  ldwork- (optional) the workspace size.
//          Default : computed by the formula
//          LDWORK = MAX( minimum workspace size needed, 2*CSIZE/3,
//                       CSIZE - ( m + l )*t - 2*n*( n + m + l ) - l*m )
//          where CSIZE is the cache size in double precision words.
//
//Output parameters:
//  x0    - the n-vector of estimated initial state x(0) (output when
//          jobx0 = 1, and set to 0 when jobx0 = 0 and comuse = 2).
//  B     - the n-by-m system input matrix B (output when comuse = 1).
//  D     - the l-by-m system matrix D (output when comuse = 1 and
//          job = 2).
//  V     - the n-by-n orthogonal matrix which reduces A to a real Schur
//          form (output when jobx0 = 1 or comuse = 1).
//  rcnd  - (optional) vector of length 1 (or 2, if comuse = 1 and
//          job = 2), containing the reciprocal condition numbers of
//          the matrices involved in rank decisions.
//
//Contributor:
//  V. Sima, Research Institute for Informatics, Bucharest, April 2000.
//
//Revisions:
//  V. Sima, July 2000, February 2001.
//
//**********************************************************************
//

#include <string.h>
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
#include "sci_malloc.h"
#include "gw_cacsd.h"
#include "elem_common.h"
/*--------------------------------------------------------------------------*/
extern int C2F(ib01cd)(char*, char*, char*, int*, int*, int*, int*, double*, int*, double*,
                       int*, double*, int*, double*, int*, double*, int*, double*, int*, double*, double*,
                       int*, double*, int*, double*, int*, int*, int*);
/*--------------------------------------------------------------------------*/
int sci_findbd(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iOne = 1;

    int iTASK = 0;
    char cJOBX0 = 'N';
    int iCUSE = 0;
    char cCOMUSE = 'N';
    int iIJOB = 0;
    char cJOB = 'D';

    int iN = 0;
    int iM = 0;
    int iL = 1;
    int iNSMP = 1;
    int iNCOL = 0;
    int iMINSMP = 0;
    int iIQ = 0;

    int iSizeA = 0;
    int iSizeB = 0;
    int iSizeC = 0;
    int iSizeD = 0;
    int iSizeY = 0;
    int iSizeU = 0;

    BOOL bPRINTW = FALSE;
    double dblTOL = 0;

    double* pdblA = NULL;
    double* pdblB = NULL;
    double* pdblC = NULL;
    double* pdblD = NULL;
    double* pdblY = NULL;
    double* pdblU = NULL;

    int iIPS = 0;
    int iIP = 0;
    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);
    int iCurrentLhs = 1;
    int iPos = 0;

    int iLDA = 0;
    int iLDB = 0;
    int iLDC = 0;
    int iLDD = 0;
    int iLDV = 0;
    int iLDY = 0;
    int iLDU = 0;
    int iLIWORK = 0;
    int iLDWORK = 0;
    int iLDWMIN = 0;

    int iWARN = 0;
    int iINFO = 0;

    /*temporary variables*/
    double* pA = NULL;
    double* pB = NULL;
    double* pC = NULL;
    double* pD = NULL;
    double* pDWORK = NULL;
    int* pIWORK = NULL;
    double* pU = NULL;
    double* pV = NULL;
    double* pX0 = NULL;
    double* pY = NULL;

    CheckInputArgumentAtLeast(pvApiCtx, 2);
    CheckOutputArgumentAtLeast(pvApiCtx, 1);


    // jobx0
    CHECK_PARAM(pvApiCtx, 1);
    iTASK = getIntegerValue(pvApiCtx, 1);

    if (iTASK == 1)
    {
        cJOBX0 = 'Q';
    }

    // comuse
    CHECK_PARAM(pvApiCtx, 2);
    iCUSE = getIntegerValue(pvApiCtx, 2);

    if (iCUSE == 1)
    {
        cCOMUSE = 'C';
    }

    if (iCUSE == 2)
    {
        cCOMUSE = 'U';
    }

    if (iTASK == 1 && iCUSE == 2)
    {
        if (iRhs < 5)
        {
            Scierror(999, _("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 5);
            return 0;
        }
    }
    else if (iCUSE == 1)
    {
        if (iRhs < 6)
        {
            Scierror(999, _("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 6);
            return 0;
        }
    }


    // job
    if ((iTASK == 1 && iCUSE == 2) || iCUSE == 1)
    {
        CHECK_PARAM(pvApiCtx, 3);
        iIJOB = getIntegerValue(pvApiCtx, 3);
        iIP = 4;
        if (iIJOB == 1)
        {
            cJOB = 'B';
        }
    }
    else
    {
        cJOB = 'B';
        iIJOB = 1;
        iIP = 3;
    }

    if (iTASK == 1 && iCUSE == 2)
    {
        if (iIJOB == 1 && iRhs < 7)
        {
            Scierror(999, _("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 7);
            return 0;
        }
        else if (iIJOB == 2 && iRhs < 8)
        {
            Scierror(999, _("%s: Wrong number of input argument(s): At least %d expected.\n"), fname, 8);
            return 0;
        }
    }

    // A(n,n)
    iIPS = iIP;

    if (iRhs > iIP)
    {
        int* piAddr = NULL;
        int iCols = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iN, &iCols, &pdblA);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iN != iCols)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, iN, iN);
            return 0;
        }

        iIP++;
    }

    // B(n,m)
    if (iTASK == 1 && iCUSE == 2 && iRhs >= iIP)
    {
        int* piAddr = NULL;
        int iRows = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iM, &pdblB);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iRows != iN)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, iN, iN);
            return 0;
        }

        iIP++;
    }

    // C(1,n)
    if (iRhs >= iIP)
    {
        int* piAddr = NULL;
        int iCols = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iL, &iCols, &pdblC);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iCols != iN)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix with %d columns expected.\n"), fname, iN);
            return 0;
        }

        if (iL <= 0)
        {
            Scierror(999, _("%s: The system has no outputs\n"), fname);
            return 0;
        }

        iIP++;
    }

    // D(1,m)
    if (iTASK == 1 && iCUSE == 2 && iIJOB == 2 && iRhs <= iIP)
    {
        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblD);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iRows != iL || iCols != iM)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, iL, iM);
            return 0;
        }

        iIP++;
    }

    // Y(txp)

    if (iRhs >= iIP)
    {
        int* piAddr = NULL;
        int iCols = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iNSMP, &iCols, &pdblY);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iCols != iL)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix with %d columns expected.\n"), fname, iL);
            return 0;
        }
    }

    // U(txm)
    if ((iTASK == 1 && iCUSE == 2) || (iCUSE == 1 && iRhs > iIP))
    {
        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddr, &iNSMP, &iCols, &pdblU);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        if (iCUSE == 2 && iCols != iM)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A matrix with %d columns expected.\n"), fname, iM);
            return 0;
        }

        iM = iCols;

        if (iM > 0)
        {
            if (iRows != iNSMP)
            {
                Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n"), fname, iIP - 1, iIP);
                return 0;
            }
        }

        iIP++;
    }

    if (iCUSE == 1)
    {
        iNCOL = iN * iM;
        if (iTASK == 1)
        {
            iNCOL += iN;
        }
        iMINSMP = iNCOL;

        if (iIJOB == 2)
        {
            iMINSMP += iM;
            iIQ = iMINSMP;
        }
        else if (iTASK == 0)
        {
            iIQ = iMINSMP;
        }
        else
        {
            iIQ = iMINSMP;
        }
    }
    else
    {
        iNCOL = iN;
        if (iTASK == 1)
        {
            iMINSMP = iN;
        }
        else
        {
            iMINSMP = 0;
        }

        iIQ = iMINSMP;
    }


    if (iNSMP < iMINSMP)
    {
        Scierror(999, _("%s: The number of samples should be at least %d"), fname, iMINSMP);
        return 0;
    }

    // tol

    if (iRhs >= iIP)
    {
        int* piAddr = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, iIP, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        getScalarDouble(pvApiCtx, piAddr, &dblTOL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, iIP);
            return 0;
        }

        iIP++;
    }


    // printw
    if (iRhs >= iIP)
    {
        int iPrintw = 0;
        CHECK_PARAM(pvApiCtx, iIP);
        iPrintw = getIntegerValue(pvApiCtx, iIP);

        switch (iPrintw)
        {
            case 1 :
                bPRINTW = TRUE;
                break;
            case 0 :
                bPRINTW = FALSE;
                break;
            default :
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 10, "0", "1");
                return 0;
        }
    }


    // Determine the lengths of working arrays.
    // The default value for LDWORK is computed using the formula
    //      LDWORK = MAX( minimum value needed, 2*CSIZE/3,
    //     $              CSIZE - ( M + L )*NSMP - 2*N*( N + M + L ) - L*M )
    // where CSIZE is the cache size in double precision words.
    // If LDWORK is specified,
    //        but it is less than the minimum workspace size
    // needed, that minimum value is used instead.

    iLDA = Max(1, iN);
    iLDB = iLDA;
    iLDC = iL;
    iLDD = iLDC;
    iLDV = iLDA;
    iLDY = Max(1, iNSMP);
    iLDU = 1;

    if (iM > 0 && ((iTASK == 1 && iCUSE == 2) || iCUSE == 1))
    {
        iLDU = iLDY;
    }

    iLIWORK = iNCOL;

    if ((iTASK == 0 && iCUSE != 1) || Max(iN, iM) == 0)
    {
        iLDWORK = 2;
    }
    else
    {
        int iISIZE = 0;
        int iNCP1 = 0;
        int iIC = 0;
        int iMINWLS = 0;

        int iLDW1 = 0;
        int iLDW2 = 0;
        int iLDW3 = 0;
        int iINI = 0;

        if (iIJOB == 2)
        {
            iLIWORK = Max(iLIWORK, iM);
        }

        iIQ *= iL;
        iNCP1 = iNCOL + 1;
        iISIZE = iNSMP * iL * iNCP1;
        if (iN > 0 && iTASK == 1)
        {
            if (iCUSE == 1)
            {
                iIC = 2 * iN * iN + iN;
            }
            else
            {
                iIC = 2 * iN * iN;
            }
        }
        else
        {
            iIC = 0;
        }

        iMINSMP = iNCOL * iNCP1;

        if (iCUSE == 1)
        {
            int iIA = 0;

            if (iIJOB == 2)
            {
                iMINWLS +=  iL * iM * iNCP1;
            }

            if (iM > 0 && iIJOB == 2)
            {
                iIA = iM + Max(2 * iNCOL, iM);
            }
            else
            {
                iIA = 2 * iNCOL;
            }

            iLDW1 = iN * iN * iM + Max(iIC, iIA);
            if (iTASK == 1)
            {
                iLDW1 += iL * iN;
            }

            iLDW2 = iISIZE + Max(iN + Max(iIC, iIA), 6 * iNCOL);
            iLDW3 = iMINWLS + Max(iIQ * iNCP1 + iLDW1, 6 * iNCOL);
            if (iM > 0 && iIJOB == 2)
            {
                iLDW2 = Max(iLDW2, iISIZE + 2 * iM * iM + 6 * iM);
                iLDW3 = Max(iLDW3, iMINWLS + 2 * iM * iM + 6 * iM);
                iINI = 3;
            }
            else
            {
                iINI = 2;
            }
        }
        else
        {
            int iITAU = iIC + iL * iN;
            iLDW2 = iISIZE + 2 * iN + Max(iIC, 4 * iN);
            iLDW3 = iMINWLS + 2 * iN + Max(iIQ + iNCP1 + iITAU, 4 * iN);
            iINI = 2;
        }

        iLDWMIN = iINI + iN * (iN + iM + iL) + Max(5 * iN, Max(iINI, Min(iLDW2, iLDW3)));
        iLDWORK = Max(Max(iLDWMIN, 2 * CSIZE / 3), CSIZE - (iM + iL ) * iNSMP - 2 * iN * (iN + iM + iL ) - iL * iM);
    }

    // ldwork
    if (iRhs > iIP)
    {
        CHECK_PARAM(pvApiCtx, iIP);
        iLDWORK = Max(getIntegerValue(pvApiCtx, iIP), iLDWMIN);
    }

    // Allocate variable dimension local arrays.
    iSizeA = iLDA * iN;
    iSizeB = iLDB * iM;
    iSizeC = iLDC * iN;
    iSizeD = iLDD * iM;
    iSizeU = iLDU * iM;
    iSizeY = iLDY * iL;

    pA = (double*)MALLOC(iLDA * iN * sizeof(double));
    pB = (double*)MALLOC(iLDB * iM * sizeof(double));
    pC = (double*)MALLOC(iLDC * iN * sizeof(double));
    pD = (double*)MALLOC(iLDD * iM * sizeof(double));
    pDWORK = (double*)MALLOC(iLDWORK * sizeof(double));
    pIWORK = (int*)MALLOC(iLIWORK * sizeof(int));
    pU = (double*)MALLOC(iLDU * iM * sizeof(double));
    pV = (double*)MALLOC(iLDV * iN * sizeof(double));
    pX0 = (double*)MALLOC(iN * sizeof(double));
    pY = (double*)MALLOC(iLDY * iL * sizeof(double));

    //Copy inputs from scilab workspace to locally allocated arrays.

    iIP = iIPS;
    if (iRhs >= iIP)
    {
        C2F(dcopy)(&iSizeA, pdblA, &iOne, pA, &iOne);
        if (iTASK == 1 && iCUSE == 2)
        {
            if (iN > 0)
            {
                C2F(dcopy)(&iSizeB, pdblB, &iOne, pB, &iOne);
            }
        }

        C2F(dcopy)(&iSizeC, pdblC, &iOne, pC, &iOne);
        if (iTASK == 1 && iCUSE == 2 && iIJOB == 2)
        {
            C2F(dcopy)(&iSizeD, pdblD, &iOne, pD, &iOne);
        }

        C2F(dcopy)(&iSizeY, pdblY, &iOne, pY, &iOne);
        if (iCUSE == 1 || (iTASK == 1 && iCUSE == 2))
        {
            C2F(dcopy)(&iSizeU, pdblU, &iOne, pU, &iOne);
        }
    }

    //Do the actual computations.
    C2F(ib01cd)(&cJOBX0 , &cCOMUSE, &cJOB, &iN, &iM, &iL, &iNSMP, pA, &iLDA, pB, &iLDB,
                pC, &iLDC , pD, &iLDD, pU, &iLDU, pY, &iLDY, pX0, pV, &iLDV, &dblTOL, pIWORK,
                pDWORK, &iLDWORK, &iWARN, &iINFO);

    if (iWARN != 0 && bPRINTW)
    {
        sciprint("IWARN = %d on exit from IB01CD\n", iWARN);
    }

    if (iINFO != 0)
    {
        Scierror(999, _("%s: INFO = %d on exit from IB01CD\n"), fname, iINFO);
        FREE(pA);
        FREE(pB);
        FREE(pC);
        FREE(pD);
        FREE(pDWORK);
        FREE(pIWORK);
        FREE(pU);
        FREE(pV);
        FREE(pX0);
        FREE(pY);
        return 0;
    }

    iCurrentLhs = iRhs + 1;
    iPos = 1;
    //Copy output to scilab workspace.
    if (iTASK == 1 || (iTASK == 0 && iCUSE == 2))
    {
        createMatrixOfDouble(pvApiCtx, iCurrentLhs, iN, 1, pX0);
        AssignOutputVariable(pvApiCtx, iPos) = iCurrentLhs;
        iCurrentLhs++;
        iPos++;
    }

    if (iCUSE == 1 && iLhs >= iPos)
    {
        createMatrixOfDouble(pvApiCtx, iCurrentLhs, iN, iM, pB);
        AssignOutputVariable(pvApiCtx, iPos) = iCurrentLhs;
        iCurrentLhs++;
        iPos++;

        if (iLhs >= iPos)
        {
            createMatrixOfDouble(pvApiCtx, iCurrentLhs, iL, iM, pD);
            AssignOutputVariable(pvApiCtx, iPos) = iCurrentLhs;
            iCurrentLhs++;
            iPos++;
        }
    }

    if ((iTASK == 1 || iCUSE == 1) && iLhs >= iPos)
    {
        createMatrixOfDouble(pvApiCtx, iCurrentLhs, iN, iN, pV);
        AssignOutputVariable(pvApiCtx, iPos) = iCurrentLhs;
        iCurrentLhs++;
        iPos++;
    }

    if (iLhs >= iPos)
    {
        int iNO = 1;
        if (iCUSE == 1 && iM > 0 && iIJOB == 2)
        {
            iNO = 2;
        }

        createMatrixOfDouble(pvApiCtx, iCurrentLhs, iNO, 1, pDWORK + 1);
        AssignOutputVariable(pvApiCtx, iPos) = iCurrentLhs;
    }

    ReturnArguments(pvApiCtx);

    FREE(pA);
    FREE(pB);
    FREE(pC);
    FREE(pD);
    FREE(pDWORK);
    FREE(pIWORK);
    FREE(pU);
    FREE(pV);
    FREE(pX0);
    FREE(pY);
    return 0;
}
