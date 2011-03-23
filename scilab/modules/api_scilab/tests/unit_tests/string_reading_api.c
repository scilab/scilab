/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Scilab Consortium Operational Team
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
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

int read_string(char *fname,unsigned long fname_len)
{
	SciErr sciErr;
	int i,j;
	int iLen		= 0;
	//variable info
	int iRows		= 0;
	int iCols		= 0;
	int* piAddr		= NULL;
	int* piLen		= NULL;
	char** pstData	= NULL;
	//output variable
	int iRowsOut	= 1;
	int iColsOut	= 1;
	char* pstOut	= NULL;
	//check input and output arguments
	CheckRhs(1,1);
	CheckLhs(1,1);
	//get variable address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	//fisrt call to retrieve dimensions
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	//second call to retrieve length of each string
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
	}
	//third call to retrieve data
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	//computer length of all strings
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		iLen += piLen[i];
	}
	//alloc output variable
	pstOut = (char*)malloc(sizeof(char) * (iLen + iRows * iCols));
	//initialize string to 0x00
	memset(pstOut, 0x00, sizeof(char) * (iLen + iRows * iCols));
	//concat input strings in output string
	for(i = 0 ; i < iRows ; i++)
	{
		for(j = 0 ; j < iCols ; j++)
		{
			int iCurLen = strlen(pstOut);
			if(iCurLen)
			{
				strcat(pstOut, " ");
			}
			strcpy(pstOut + strlen(pstOut), pstData[j * iRows + i]);
		}
	}
	//create new variable
	sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, iRowsOut, iColsOut, &pstOut);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}
	//free memory
	free(piLen);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);
	free(pstOut);
	LhsVar(1) = Rhs + 1;
	return 0;
}
