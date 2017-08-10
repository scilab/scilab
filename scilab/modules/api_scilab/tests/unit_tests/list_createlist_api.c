/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO
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

#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "sci_malloc.h"

int list_createlist(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddr             = NULL;
    int* piChild            = NULL;
    double pdblData1[]      = {1, 3, 5, 2, 4, 6};
    double pdblData2[]      = {6, 4, 2, 5, 3, 1};
    char *pstData[]         = {"may", "be", "the", "with", "puffin", "you"};
    short psData[]          = {1, 4, 2, 5, 3, 6};
    double pdblPoly1[]      = {1};
    double pdblPoly2[]      = { -2, -1};
    double pdblPoly3[]      = {1, 2, 3};
    double pdblPoly4[]      = { -4, -3, -2, -1};
    double pdblPoly5[]      = {1, 2, 3, 4, 5};
    double pdblPoly6[]      = { -6, -5, -4, -3, -2, -1};
    double *pdblPoly[]      = {pdblPoly1, pdblPoly3, pdblPoly5, pdblPoly2, pdblPoly4, pdblPoly6};
    int piCoef[]            = {1, 3, 5, 2, 4, 6};
    int piNbItemRow[]       = {1, 2, 1};
    int piColPos[]          = {8, 4, 7, 2};
    double pdblSReal[]      = {1, 2, 3, 4};
    double pdblSImg[]       = {4, 3, 2, 1};
    int piBool[]            = {1, 0, 1, 0, 1, 0, 1, 0, 1};

    sciErr = createList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 8, &piAddr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 1, 3, 2, pdblData1, pdblData2);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfStringInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 2, 2, 3, pstData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfInteger16InList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 3, 2, 3, psData);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfPolyInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 4, "x", 3, 2, piCoef, pdblPoly);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createComplexSparseMatrixInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 5, 3, 10, 4, piNbItemRow, piColPos, pdblSReal, pdblSImg);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfBooleanInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 6, 3, 3, piBool);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }
    sciErr = createBooleanSparseMatrixInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 7, 3, 10, 4, piNbItemRow, piColPos);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    //add list in list
    sciErr = createListInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piAddr, 8, 2, &piChild);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createMatrixOfDoubleInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piChild, 1, 3, 2, pdblData1);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = createSparseMatrixInList(pvApiCtx, nbInputArgument(pvApiCtx) + 1, piChild, 2, 3, 10, 4, piNbItemRow, piColPos, pdblSReal);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
    return 0;
}
