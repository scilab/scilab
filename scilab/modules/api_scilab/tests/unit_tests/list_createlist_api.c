/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "MALLOC.h"

             
int list_createlist(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int *piAddr             = NULL;
	int* piChild            = NULL;
	double pdblData1[]      = {1,3,5,2,4,6};
	double pdblData2[]      = {6,4,2,5,3,1};
	char *pstData[]         = {"may","be","the","with","puffin","you"};
	short psData[]          = {1,4,2,5,3,6};
	double pdblPoly1[]      = {1};
	double pdblPoly2[]      = {-2,-1};
	double pdblPoly3[]      = {1,2,3};
	double pdblPoly4[]      = {-4,-3,-2,-1};
	double pdblPoly5[]      = {1,2,3,4,5};
	double pdblPoly6[]      = {-6,-5,-4,-3,-2,-1};
	double *pdblPoly[]      = {pdblPoly1, pdblPoly3, pdblPoly5, pdblPoly2, pdblPoly4, pdblPoly6};
	int piCoef[]            = {1,3,5,2,4,6};
	int piNbItemRow[]       = {1,2,1};
	int piColPos[]          = {8,4,7,2};
	double pdblSReal[]      = {1,2,3,4};
	double pdblSImg[]       = {4,3,2,1};
	int piBool[]            = {1,0,1,0,1,0,1,0,1};
	double* pdblDataPtr     = NULL;


	sciErr = createList(pvApiCtx, 1, 8, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piAddr, 1, 3, 2, pdblData1, pdblData2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfStringInList(pvApiCtx, Rhs + 1, piAddr, 2, 2, 3, pstData);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfInteger16InList(pvApiCtx, Rhs + 1, piAddr, 3, 2, 3, psData);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfPolyInList(pvApiCtx, Rhs + 1, piAddr, 4, "x", 3, 2, piCoef, pdblPoly);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createComplexSparseMatrixInList(pvApiCtx, Rhs + 1, piAddr, 5, 3, 10, 4, piNbItemRow, piColPos, pdblSReal, pdblSImg);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfBooleanInList(pvApiCtx, Rhs + 1, piAddr, 6, 3, 3, piBool);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createBooleanSparseMatrixInList(pvApiCtx, Rhs + 1, piAddr, 7, 3, 10, 4, piNbItemRow, piColPos);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//add list in list
	sciErr = createListInList(pvApiCtx, Rhs + 1, piAddr, 8, 3, &piChild);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createMatrixOfDoubleInList(pvApiCtx, Rhs + 1, piChild, 1, 3, 2, pdblData1);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	sciErr = createSparseMatrixInList(pvApiCtx, Rhs + 1, piChild, 2, 3, 10, 4, piNbItemRow, piColPos, pdblSReal);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	pdblDataPtr     = (double*)malloc(sizeof(double) * 4);
	pdblDataPtr[0]  = 1;
	pdblDataPtr[1]  = 2;
	pdblDataPtr[2]  = 3;
	pdblDataPtr[3]  = 4;

	sciErr = createPointerInList(pvApiCtx, Rhs + 1, piChild, 3, pdblDataPtr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	LhsVar(1) = 1;
	return 0;
}
 
        
