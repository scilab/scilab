/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>
#include "gw_signal.h"
#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"
#include "localization.h"
#include "basic_functions.h"


/*--------------------------------------------------------------------------*/
//extern int C2F(sci_levinson)(char *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_levinson)(char *fname,unsigned long fname_len)
{
	int iIndex			= 0;
	int iRows			= 0;
	int iCols			= 0;


	int *pRoot				= NULL;
	int *pParent1			= NULL;
	int *pParent2			= NULL;
	int *pParent21			= NULL;

	double *pRealData111	= NULL;
	double *pRealData112	= NULL;
	double *pRealData1211	= NULL;
	double *pRealData1212	= NULL;
	double *pRealData122	= NULL;
	double *pRealData13		= NULL;
	double *pRealData2		= NULL;

	Rhs		= Max(Rhs, 0);

	if(Rhs != 0)
	{
		int iAddrBase	= iadr(*Lstk(Top - Rhs + 1));
		int *pIn		= istk(iAddrBase);
		int iType		= 0;
		int iRows		= 0;
		int iCols		= 0;
		int iComplex	= 0;
		int iSize		= 0;
		int *piOffset	= NULL;
		int *piData		= NULL;

		iType			= *istk( iAddrBase ); //Returns variable type
		iRows			= *istk( iAddrBase + 1 ); //Returns number of lines in the matrix
		iCols			= *istk( iAddrBase + 2 ); //Returns number of columns in the matrix
		iComplex		= *istk( iAddrBase + 3 ); //Returns 0
		piOffset		=  istk( iAddrBase + 4 ); // Returns an array of offset, size = Lines * Columns + 1
		piData			=  istk( iAddrBase + 4 + 1 + iRows * iCols ); //Returns a pointer on the first charater of the first string, size = Offset[MAX-1] - 1
		iSize			= iRows * iCols;
	}




	//1
	pRoot		= iAllocList(Rhs + 1, 3);

	//1.1
	pParent1	= iListAllocList(Rhs + 1, pRoot, 1, 2);
		//1.1.1
		iListAllocMatrixOfDouble(Rhs + 1, pParent1, 1, 2, 2, &pRealData111);
		//1.1.2
		iListAllocMatrixOfDouble(Rhs + 1, pParent1, 2, 2, 2, &pRealData112);

	//1.2
	pParent2	= iListAllocList(Rhs + 1, pRoot, 2, 2);
		//1.2.1
		pParent21	= iListAllocList(Rhs + 1, pParent2, 1, 2);
			//1.2.1.1
			iListAllocMatrixOfDouble(Rhs + 1, pParent21, 1, 2, 2, &pRealData1211);
			//1.2.1.2
			iListAllocMatrixOfDouble(Rhs + 1, pParent21, 2, 2, 2, &pRealData1212);
		//1.2.2
		iListAllocMatrixOfDouble(Rhs + 1, pParent2, 2, 2, 2, &pRealData122);
	
	//1.3
	iListAllocMatrixOfDouble(Rhs + 1, pRoot, 3, 2, 2, &pRealData13);

	{
		int **piVar;
		int piPow[2];

		piPow[0] = 1;
		piPow[1] = 3;

		piVar		= (int**)malloc(sizeof(int));
		piVar[0]	= (int*)malloc(4 * sizeof(int));
		piVar[0][0]	= 0x1C;
		piVar[0][1]	= 0x28;
		piVar[0][2]	= 0x28;
		piVar[0][3]	= 0x28;

		iAllocMatrixOfPoly(Rhs + 2, piVar, 1,2, piPow, &pRealData2);

	}

	for(iIndex = 0 ; iIndex < 4 ; iIndex++)
	{
		pRealData111[iIndex]	= iIndex + 1;
		pRealData112[iIndex]	= 4 + iIndex + 1;
		pRealData1211[iIndex]	= 8 + iIndex + 1;
		pRealData1212[iIndex]	= 12 + iIndex + 1;
		pRealData122[iIndex]	= 16 + iIndex + 1;
		pRealData13[iIndex]		= 20 + iIndex + 1;
		pRealData2[iIndex]		= 24 + iIndex + 1;
	}

	LhsVar(1) = Rhs + 1;
	LhsVar(2) = Rhs + 2;
	PutLhsVar();
	
	return 0;
}
/*--------------------------------------------------------------------------*/
