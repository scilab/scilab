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

//ORDER.F  - Gateway function for computation of the order of a
//           discrete-time system using SLICOT routine IB01AD.
//
//RELEASE 4.0, WGS COPYRIGHT 2000.
//
//scilab call:
//  [R(,n,sval,rcnd)] = order(meth,alg,jobd,batch,conct,s,Y(,U,tol,
//                            printw,ldwork,R))
//
//Purpose:
//  To preprocess the input-output data for estimating the matrices
//  of a linear time-invariant dynamical system, using Cholesky or
//  QR factorization and subspace identification techniques (MOESP
//  and N4SID), and to find an estimate of the system order. The
//  input-output data can, optionally, be processed sequentially.
//
//Input parameters:
//  meth  - integer option to determine the method to use:
//          = 1 : MOESP method with past inputs and outputs;
//          = 2 : N4SID method.
//  alg   - integer option to determine the algorithm for computing the
//          triangular factor of the concatenated block-Hankel matrices
//          built from the input-output data:
//          = 1 : Cholesky algorithm on the correlation matrix;
//          = 2 : fast QR algorithm;
//          = 3 : standard QR algorithm.
//  jobd  - integer option to specify if the matrices B and D should
//          later be computed using the MOESP approach:
//          = 1 : the matrices B and D should later be computed using
//                the MOESP approach;
//          = 2 : the matrices B and D should not be computed using
//                the MOESP approach.
//          This parameter is not relevant for meth = 2.
//  batch - integer option to specify whether or not sequential data
//          processing is to be used, and, for sequential processing,
//          whether or not the current data block is the first block,
//          an intermediate block, or the last block, as follows:
//          = 1:  the first block in sequential data processing;
//          = 2:  an intermediate block in sequential data processing;
//          = 3:  the last block in sequential data processing;
//          = 4:  one block only (non-sequential data processing).
//  conct - integer option to specify whether or not the successive data
//          blocks in sequential data processing belong to a single
//          experiment, as follows:
//          = 1:  the current data block is a continuation of the
//                previous data block and/or it will be continued by the
//                next data block;
//          = 2:  there is no connection between the current data block
//                and the previous and/or the next ones.
//          This parameter is not used if batch = 4.
//  s     - the number of block rows in the input and output block
//          Hankel matrices to be processed.  s > 0.
//  Y     - the t-by-l output-data sequence matrix.  Column j of  Y
//          contains the  t  values of the j-th output component for
//          consecutive time increments.
//  U     - (optional) the t-by-m input-data sequence matrix.  Column j
//          of  U  contains the  t  values of the j-th input component
//          for consecutive time increments.
//          Default:    U = [].
//  tol   - (optional) vector of length 2 containing tolerances:
//          tol(1) - tolerance used for estimating the rank of matrices.
//          If  tol(1) > 0,  then the given value of  tol(1)  is used
//          as a lower bound for the reciprocal condition number;
//          an m-by-n matrix whose estimated condition number is less
//          than  1/tol(1)  is considered to be of full rank.
//          If  tol(1) <= 0,  then a default value m*n*epsilon_machine
//          is used, where epsilon_machine is the relative machine
//          precision.
//          tol(2) - tolerance used for determining an estimate of the
//          system order. If  tol(2) >= 0,  the estimate is indicated
//          by the index of the last singular value greater than or
//          equal to  tol(2).  (Singular values less than  tol(2) are
//          considered as zero.) When  tol(2) = 0,  an internally
//          computed default value,  tol(2) = s*epsilon_machine*sval(1),
//          is used, where  sval(1)  is the maximal singular value, and
//          epsilon_machine the relative machine precision.
//          When  tol(2) < 0,  the estimate is indicated by the index of
//          the singular value that has the largest logarithmic gap to
//          its successor.
//          Default:    tol(1:2) = [0,-1].
//  printw- (optional) switch for printing the warning messages.
//          = 1:  print warning messages;
//          = 0:  do not print warning messages.
//          Default:    printw = 0.
//  ldwork- (optional) the workspace size.
//          Default : computed by the formulas
//          nr = 2*( m + l )*s
//          LDWORK = ( t - 2*s + 3 + 64 )*nr
//          if ( CSIZE > MAX( nr*nr + t*( m + l ) + 16, 2*nr ) ) then
//             LDWORK = MIN( LDWORK, CSIZE - nr*nr - t*( m + l ) - 16 )
//          else
//             LDWORK = MIN( LDWORK, MAX( 2*nr, CSIZE/2 ) )
//          end if
//          LDWORK = MAX( minimum workspace size needed, LDWORK )
//          where CSIZE is the cache size in double precision words.
//          If LDWORK is specified less than the minimum workspace size
//          needed, that minimum value is used instead.
//  R     - (optional) if batch = 2 or 3, the 2*(m+l)*s-by-2*(m+l)*s
//          (upper triangular, if alg <> 2) part of  R  must contain the
//          (upper triangular) matrix  R  computed at the previous call
//          of this mexfile in sequential data processing. If conct = 1,
//          R  has an additional column, also set at the previous call.
//          If alg = 2,  R  has m+l+1 additional columns, set at the
//          previous call.
//          This parameter is not used for batch = 1 or batch = 4.
//
//Output parameter:
//  R     - if batch = 3 or 4, the 2*(m+l)*s-by-2*(m+l)*s part of  R
//          contains the processed upper triangular factor  R  from the
//          QR factorization of the concatenated block-Hankel matrices,
//          and further details needed for computing system matrices.
//          If batch = 1 or 2, then  R  contains intermediate results
//          needed at the next call of this mexfile. If batch = 1 or 2
//          and conct = 1,  R  has an additional column, also set before
//          return. If batch = 1 or 2 and alg = 2,  R  has m+l+1
//          additional columns, set before return.
//  n     - the order of the system.
//  sval  - (optional) the singular values used for estimating the order
//          of the system.
//  rcnd  - (optional) if meth = 2, vector of length 2 containing the
//          reciprocal condition numbers of the matrices involved in
//          rank decisions or least squares solutions.
//
//Contributor:
//  V. Sima, Research Institute for Informatics, Bucharest, Oct. 1999.
//
//Revisions:
//  V. Sima, May 2000, July 2000.
//
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "BOOL.h"
#include "sci_malloc.h"
#include "gw_cacsd.h"
#include "elem_common.h"
/*--------------------------------------------------------------------------*/
extern int C2F(ib01ad)(char*, char*, char*, char*, char*, char*, int*, int*,
                       int*, int*, double*, int*, double*, int*, int*, double*, int*, double*, double*,
                       double*, int*, double*, int*, int*, int*);
/*--------------------------------------------------------------------------*/
int sci_sorder(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iOne = 1;

    char cMeth = 0;
    int iMeth = 0;
    char cAlg = 0;
    int iAlg = 0;
    char cJobd = 0;
    int iJobd = 0;
    char cBatch = 0;
    int iBatch = 0;
    char cConct = 0;
    int iConct = 0;
    char cCtrl = 0;
    int iNOBR = 0;

    int* piAddrY = NULL;
    int iSizeY = 0;
    int iNSMP = 0;
    int iL = 0;
    double* pdblY = NULL;

    int* piAddrU = NULL;
    int iSizeU = 0;
    int iRowsU = 0;
    int iM = 0;
    int iN = 0; //used but never filled
    double* pdblU = NULL;

    int iSizeR = 0;
    double* pdblR = NULL;

    double dblTol1 = 0;
    double dblTol2 = -1;

    BOOL bPrintw = FALSE;
    int iNCOL = 0;
    int iNRSAVE = 0;

    int iLDY = 0;
    int iLDU = 1;
    int iLDR = 0;
    int iLIWORK = 1;
    int iLDWORK = 0;
    int iLDWMIN = 0;

    int iRhs = nbInputArgument(pvApiCtx);
    int iLhs = nbOutputArgument(pvApiCtx);

    //some ubber explicit variables ...
    int iNR = 0;
    int iNSMPMN = 0;
    int iWARN = 0;
    int iINFO = 0;

    //working array
    double* pR = NULL;
    double* pSV = NULL;
    double* pU = NULL;
    double* pY = NULL;
    int* pIWORK = NULL;
    double* pDWORK = NULL;

    CheckInputArgumentAtLeast(pvApiCtx, 7);
    CheckOutputArgumentAtLeast(pvApiCtx, 1);


    //  METH
    CHECK_PARAM(pvApiCtx, 1);
    iMeth = getIntegerValue(pvApiCtx, 1);

    switch (iMeth)
    {
        case 1 :
            cMeth = 'M';
            break;
        case 2 :
            cMeth = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 1, "1", "2");
            return 0;
    }

    //  ALG
    CHECK_PARAM(pvApiCtx, 2);
    iAlg = getIntegerValue(pvApiCtx, 2);

    switch (iAlg)
    {
        case 1 :
            cAlg  = 'C';
            break;
        case 2 :
            cAlg = 'F';
            break;
        case 3 :
            cAlg = 'Q';
            break;
        default:
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname, 2, "1", "2", "3");
            return 0;
    }

    //  JOBD
    CHECK_PARAM(pvApiCtx, 3);
    iJobd = getIntegerValue(pvApiCtx, 3);

    switch (iJobd)
    {
        case 1 :
            cJobd = 'M';
            break;
        case 2 :
            cJobd = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 3, "1", "2");
            return 0;

    }

    //  BATCH
    CHECK_PARAM(pvApiCtx, 4);
    iBatch = getIntegerValue(pvApiCtx, 4);

    switch (iBatch)
    {
        case 1 :
            cBatch  = 'F';
            break;
        case 2 :
            cBatch = 'I';
            break;
        case 3 :
            cBatch = 'L';
            break;
        case 4 :
            cBatch = 'O';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s', '%s' or '%s' expected.\n"), fname, 4, "1", "2", "3", "4");
            return 0;
    }


    //  CONCT
    CHECK_PARAM(pvApiCtx, 5);
    iConct = getIntegerValue(pvApiCtx, 5);

    switch (iConct)
    {
        case 1 :
            cConct = 'M';
            break;
        case 2 :
            cConct = 'N';
            break;
        default :
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 5, "1", "2");
            return 0;
    }

    //CTRL
    CHECK_PARAM(pvApiCtx, 6);
    iNOBR = getIntegerValue(pvApiCtx, 6);
    if (iNOBR < 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Scalar positive integer expected.\n"), fname, 6);
        return 0;
    }

    cCtrl = 'N';

    //  Y

    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        return 0;
    }

    getMatrixOfDouble(pvApiCtx, piAddrY, &iNSMP, &iL, &pdblY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 7);
        return 0;
    }

    iSizeY = iNSMP * iL;
    if (iL <= 0)
    {
        Scierror(999, _("%s: The system has no outputs\n"), fname);
        return 0;
    }

    //  U
    if (iRhs >= 8)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddrU);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 8);
            return 0;
        }

        getMatrixOfDouble(pvApiCtx, piAddrU, &iRowsU, &iM, &pdblU);
        iSizeU = iRowsU * iM;
    }

    iNR = 2 * (iM + iL) * iNOBR;
    if (iBatch <= 2)
    {
        iNSMPMN = 2 * iNOBR;
    }
    else
    {
        iNSMPMN = iNR + 2 * iNOBR - 1;
    }

    if (iNSMP < iNSMPMN)
    {
        Scierror(999, _("%s: The number of samples should be at least %d\n"), fname, iNSMPMN);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddrY) == 0 || isVarComplex(pvApiCtx, piAddrY) == 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 7);
        return 0;
    }

    if (iM > 0)
    {
        if (isDoubleType(pvApiCtx, piAddrU) == 0 || isVarComplex(pvApiCtx, piAddrU) == 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 8);
            return 0;
        }

        if (iRowsU != iNSMP)
        {
            Scierror(999, _("%s: Incompatible input arguments #%d and #%d: Same row dimensions expected.\n"), fname, 7, 8);
            return 0;
        }
    }


    //  TOL
    if (iRhs >= 9)
    {
        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;
        double* pdblTol = NULL;

        sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddr);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
            return 0;
        }

        sciErr = getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblTol);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
            return 0;
        }

        if (iRows * iCols > 0)
        {
            dblTol1 = pdblTol[0];
        }
        else if (iRows * iCols > 1)
        {
            dblTol2 = pdblTol[1];
        }
    }

    iNCOL = iNR;
    if (iBatch < 4)
    {
        if (iConct == 1)
        {
            iNCOL += 1;
        }

        if (iAlg == 2)
        {
            iNCOL += iM + iL + 1;
        }
    }

    iNRSAVE = (iNCOL - iNR ) * iNR;

    //  PRINTW
    if (iRhs >= 10)
    {
        int iPrintw = 0;
        CHECK_PARAM(pvApiCtx, 10);
        iPrintw = getIntegerValue(pvApiCtx, 10);

        switch (iPrintw)
        {
            case 1 :
                bPrintw = TRUE;
                break;
            case 0 :
                bPrintw = FALSE;
                break;
            default :
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), fname, 10, "0", "1");
                return 0;
        }
    }

    //  Determine the lengths of working arrays.
    //  The quasi-optimal value for LDWORK (assumed block-size 64) is possibly
    //  modified, so that the sum of the lengths of DWORK and other needed
    //  arrays does not exceed the cache size. Specifically, the default value
    //  for LDWORK is computed using the formulas
    //            nr = 2*( m + l )*s
    //            LDWORK = ( t - 2*s + 3 + 64 )*nr
    //            if ( CSIZE > MAX( nr*nr + t*( m + l ) + 16, 2*nr ) ) then
    //               LDWORK = MIN( LDWORK, CSIZE - nr*nr - t*( m + l ) - 16 )
    //            else
    //               LDWORK = MIN( LDWORK, MAX( 2*nr, CSIZE/2 ) )
    //            end if
    //            LDWORK = MAX( minimum workspace size needed, LDWORK )
    //  where CSIZE is the cache size in double precision words.
    //  If LDWORK is specified,
    //         but it is less than the minimum workspace size
    //  needed, that minimum value is used instead.

    iLDY = iNSMP;
    if (iM > 0)
    {
        iLDU = iLDY;
    }

    iLDR = iNR;
    if (iMeth == 1 && iJobd == 1)
    {
        iLDR = Max(iNR, 3 * iM * iNOBR);
    }

    if (iMeth == 2)
    {
        iLIWORK = (iM + iL) * iNOBR;
    }
    else if (iAlg == 2)
    {
        iLIWORK = iM + iL;
    }


    //  The next statement is included in order to allow switching
    //  from Cholesky or fast QR to QR algorithm.

    iLDWORK = (iNSMP - 2 * iNOBR + 3 + 64) * iNR;
    if (Max(iNR * iNR + iNSMP * (iM + iL ) + 16, 2 * iNR) < CSIZE)
    {
        iLDWORK = Min(iLDWORK, CSIZE - iNR * iNR - iNSMP * (iM + iL) - 16);
    }
    else
    {
        iLDWORK = Min(iLDWORK , Max(2 * iNR, CSIZE / 2));
    }

    iLDWMIN = 2 * iNR * (iNOBR + 1);
    if (iAlg == 2)
    {
        iLDWMIN = Max(iLDWMIN, Max(2 * iNR * (iM + iL + 1) + iNR, iNR * (iM + iL + 3)));
    }

    iLDWORK = Max(iLDWORK, iLDWMIN);

    //  LDWORK
    if (iRhs >= 11)
    {
        int iTmp = 0;
        CHECK_PARAM(pvApiCtx, 11);
        getIntegerValue(pvApiCtx, 11);
        if (iTmp < iLDWMIN)
        {
            iLDWORK = iLDWMIN;
        }
        else
        {
            iLDWORK = iTmp;
        }

        //  R(2*(m+p)*s,2*(m+p)*s(+c)), where
        //  c = 1,       if conct = 1 and batch < 4;
        //  c = 0,       if conct = 2 or  batch = 4;
        //  c = c+m+l+1, if  alg  = 2 and batch < 4.

        if (iRhs >= 12 && (iBatch == 2 || iBatch == 3))
        {
            int* piAddr = NULL;
            int iRows = 0;
            int iCols = 0;

            sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddr);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 9);
                return 0;
            }

            getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblR);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 12);
                return 0;
            }

            if (iRows != iNR || iCols != iNCOL)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A matrix of size %dx%d expected.\n"), fname, 12, iNR, iNCOL);
                return 0;
            }

            iSizeR = iRows * iCols;
        }
    }

    //  Allocate variable dimension local arrays.
    pR = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pSV = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pU = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pY = (double*)MALLOC(iLDR * iNCOL * sizeof(double));
    pIWORK = (int*)MALLOC(iLDR * iNCOL * sizeof(int));
    pDWORK = (double*)MALLOC(iLDR * iNCOL * sizeof(double));

    memset(pR, 0x00, iLDR * iNCOL * sizeof(double));

    // Copy inputs from scilab workspace to locally allocated arrays.
    C2F(dcopy)(&iSizeY, pdblY, &iOne, pY, &iOne);

    if (iM > 0)
    {
        C2F(dcopy)(&iSizeU, pdblU, &iOne, pU, &iOne);
    }

    if (iRhs >= 12 && (iBatch == 2 || iBatch == 3))
    {
        C2F(dcopy)(&iSizeR, pdblR, &iOne, pR, &iOne);
    }

    if (iAlg == 2 || iConct == 1)
    {
        if (iLDR == iNR)
        {
            C2F(dcopy)(&iNRSAVE, pR + iNR * iLDR, &iOne, pDWORK, &iOne);
        }
        else
        {
            C2F(dcopy)(&iSizeR, pR + iNR * iNR, &iOne, pDWORK, &iOne);
        }
    }

    // Do the actual computations.

    C2F(ib01ad)(&cMeth, &cAlg , &cJobd, &cBatch, &cConct, &cCtrl, &iNOBR, &iM, &iL,
                &iNSMP, pU, &iLDU, pY, &iLDY, &iN, pR, &iLDR, pSV, &dblTol1,
                &dblTol2, pIWORK, pDWORK, &iLDWORK, &iWARN, &iINFO);

    if (iWARN != 0 && bPrintw)
    {
        sciprint("IWARN = %d on exit from IB01AD\n", iWARN);
    }

    if (iINFO != 0)
    {
        Scierror(999, _("%s: INFO = %d on exit from IB01AD\n"), fname, iINFO);
        FREE(pR);
        FREE(pSV);
        FREE(pU);
        FREE(pY);
        FREE(pIWORK);
        FREE(pDWORK);
        return 0;
    }

    //  Copy output to scilab workspace.
    if (iLDR > iNR)
    {
        C2F(dlacpy)("FULL", &iNR, &iNR, pR, &iLDR, pR, &iNR);
    }

    if ((iAlg == 2 || iConct == 1) && iBatch <= 2)
    {
        if (iLDR == iNR)
        {
            C2F(dcopy)(&iNRSAVE, pDWORK, &iOne, pR + iNR * iLDR, &iOne);
        }
        else
        {
            C2F(dcopy)(&iNRSAVE, pDWORK, &iOne, pR + iNR * iNR, &iOne);
        }
    }

    createMatrixOfDouble(pvApiCtx, iRhs + 1, iNR, iNCOL, pR);

    if (iLhs > 1 && iBatch > 2)
    {
        createScalarDouble(pvApiCtx, iRhs + 2, (double)iN);
    }
    else if (iLhs > 1)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 2);
    }

    if (iLhs > 2 && iBatch > 2)
    {
        createMatrixOfDouble(pvApiCtx, iRhs + 3, iL * iNOBR, 1, pSV);
    }
    else if (iLhs > 2)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 3);
    }

    if (iLhs > 3 && iBatch > 2)
    {
        createMatrixOfDouble(pvApiCtx, iRhs + 4, 2, 1, pDWORK + 1);
    }
    else if (iLhs > 3)
    {
        createEmptyMatrix(pvApiCtx, iRhs + 4);
    }


    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 4) = iRhs + 4;
    }

    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 3) = iRhs + 3;
    }

    if (iRhs > 3)
    {
        AssignOutputVariable(pvApiCtx, 2) = iRhs + 2;
    }

    AssignOutputVariable(pvApiCtx, 1) = iRhs + 1;

    ReturnArguments(pvApiCtx);

    FREE(pR);
    FREE(pSV);
    FREE(pU);
    FREE(pY);
    FREE(pIWORK);
    FREE(pDWORK);
    return 0;
}
