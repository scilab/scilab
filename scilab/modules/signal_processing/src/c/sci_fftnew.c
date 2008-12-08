/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_signal.h"
#include "machine.h"
#include "stack-c.h"
#include "stackinfo.h"
#include "sciprint.h"
#include "localization.h"
#include <stdlib.h>

extern int C2F(fft842)();
extern int C2F(dfft2)();
/*--------------------------------------------------------------------------*/
//extern int C2F(scifft)(char *id,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_fftnew)(char *fname,unsigned long fname_len)
{
/*	int iWay				= -1; //Default is revert mode
	int iErr				= 0;
	int iOne				= 1;
	int iType				= 0;

	int iRows1				= 0;
	int iCols1				= 0;
	int iSize1				= 0;
	int iReal1				= 0;
	int iImg1				= 0;
	int iVect1				= 0;
	int iComplex			= 0;

	int iRows2				= 0;
	int iCols2				= 0;
	int iReal2				= 0;

	int iSizePow2			= 0;
	int iRowsPow2			= 0;
	int iColsPow2			= 0;

	int iIndex1				= 0;

	int iSpaceTotal			= 0;
	int iSpaceUsed			= 0;
	int iSpaceFree			= 0;

	double *pdblReal1		= NULL;
	double *pdblImg1		= NULL;

	double *pReturnReal		= NULL;
	double *pReturnImg		= NULL;

	double *pdblWorkSpace	= NULL;
	int *piWorkSpace		= NULL;

	if(Rhs != 1 && Rhs != 2 && Rhs != 4)
		Error(39);

	iType = getType(1);
	if(iType != sci_matrix && iType != sci_mlist)
	{
		OverLoad(1);
		return 0;
	}

	if(iType == sci_matrix)
	{
		if(iIsComplex(1))
		{
			iComplex = 1;
			GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows1, &iCols1, &iReal1, &iImg1);
			pdblReal1	= stk(iReal1);
			pdblImg1	= stk(iImg1);
			iSize1		= iRows1 * iCols1;
		}
		else
		{
			GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &iReal1);
			pdblReal1	= stk(iReal1);
			iSize1		= iRows1 * iCols1;
			pdblImg1	= (double*)malloc(sizeof(double) * iSize1);
			memset(pdblImg1, 0x00, sizeof(double) * iSize1);
		}
	}
	else
	{//hypermatrix
	}
	if(iRows1 == 1 || iCols1 == 1)
		iVect1 = 1;

	if(Rhs >= 2) // fft(a) is equivalent to fft(a,-1)
	{
		if(GetType(2) != sci_matrix)
		{
			Error(53);
			return 0;
		}
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iReal2);
		iWay	= (int)stk(iReal2)[0];

		if(iWay != 1 && iWay != -1)
		{
			Error(36);
			return 0;
		}
	}

	//Alloc return variable
	iAllocComplexMatrixOfDouble(Rhs + 1, 1, iRows1, iCols1, &pReturnReal, &pReturnImg);

	//copy input data to return variable
	memcpy(pReturnReal, pdblReal1, sizeof(double) * iRows1 * iCols1);

	if(iComplex)
		memcpy(pReturnImg, pdblImg1, sizeof(double) * iRows1 * iCols1);
	else //if input data are real, feel Img with 0
		memset(pReturnImg, 0x00, sizeof(double) * iRows1 * iCols1);

	//Compute free space in stack
	C2F(getstackinfo)(&iSpaceTotal, &iSpaceUsed);
	iSpaceFree = iSpaceTotal - iSpaceUsed;

	if(Rhs < 4)
	{
		//FFT


		if(iVect1)
		{//fft of a vector
			// power of 2 ?
			iSizePow2	= (int)pow(2,(int)((log(iSize1 + 0.5) / log(2))));
			if(iSizePow2 == iSize1 && iSize1 <= pow(2,15))
				C2F(fft842)(&iWay, &iSize1, pReturnReal, pReturnImg, &iErr);
//				fft842(iWay, iSize1, pReturnReal, pReturnImg, iErr);

			else
			{
				pdblWorkSpace	= (double*)malloc(sizeof(double) * iSpaceFree);
				piWorkSpace		= (int*)malloc(sizeof(int) * iSpaceFree);
//				dfft2(pReturnReal, pReturnImg, 1, iSize1, 1, iWay, iErr, piWorkSpace, iSpaceFree);
				C2F(dfft2)(pReturnReal, pReturnImg, &iOne, &iSize1, &iOne, &iWay,
					&iErr, pdblWorkSpace, &iSpaceFree);
				free(pdblWorkSpace);
			}
		}
		else
		{
			// power of 2 ?
			iRowsPow2	= (int)pow(2,(int)((log(iRows1 + 0.5) / log(2))));
			iColsPow2	= (int)pow(2,(int)((log(iCols1 + 0.5) / log(2))));

			//first call ()
			//If m2 is a power of two and less than m<2^15 use fft842 for efficiency
			if(iRowsPow2 == iRows1 && iRows1 < pow(2,15))
				for(iIndex1 = 0 ; iIndex1 < iCols1 ; iIndex1++)
					C2F(fft842)(&iWay, &iRows1, &pReturnReal[iRows1 * iIndex1],
						&pReturnImg[iRows1 * iIndex1], &iErr);
			else
			{
				pdblWorkSpace	= (double*)malloc(sizeof(double) * iSpaceFree);
				C2F(dfft2)(pReturnReal, pReturnImg, &iCols1, &iRows1, &iOne, &iWay,
					&iErr, pdblWorkSpace, &iSpaceFree);
				free(pdblWorkSpace);
			}

			//second call ()
			if(	pow(2,iCols1) <= iSpaceFree &&
				iColsPow2 == iCols1 &&
				iCols1 <= pow(2,15))
			{
				double *pdblWorkSpaceReal	= (double*)malloc(sizeof(double) * iCols1);
				double *pdblWorkSpaceImg	= (double*)malloc(sizeof(double) * iCols1);
				for(iIndex1 = 0 ; iIndex1 < iRows1 ; iIndex1++)
				{
					C2F(dcopy)(&iCols1, &pReturnReal[iIndex1], &iRows1, pdblWorkSpaceReal, &iOne);
					C2F(dcopy)(&iCols1, &pReturnImg[iIndex1], &iRows1, pdblWorkSpaceImg, &iOne);
					C2F(fft842)(&iWay, &iCols1, pdblWorkSpaceReal, pdblWorkSpaceImg, &iErr);
					if(iErr < 0)
						break;
					C2F(dcopy)(&iCols1, pdblWorkSpaceReal, &iOne, &pReturnReal[iIndex1], &iRows1);
					C2F(dcopy)(&iCols1, pdblWorkSpaceImg, &iOne, &pReturnImg[iIndex1], &iRows1);
				}
				free(pdblWorkSpaceReal);
				free(pdblWorkSpaceImg);
			}
			else
			{
				pdblWorkSpace	= (double*)malloc(sizeof(double) * iSpaceFree);
				C2F(dfft2)(pReturnReal, pReturnImg, &iOne, &iCols1, &iRows1, &iWay,
					&iErr, pdblWorkSpace, &iSpaceFree);
				free(pdblWorkSpace);
			}
		}

		if(iErr)
		{
			sciprint(_("fft fails by lack of memory.\n"), fname);
			Error(999);
			return 0;
		}
	}
	else
	{// n dimensional fft
		int iRows3	= 0;
		int iCols3	= 0;
		int iReal3	= 0;

		int iRows4	= 0;
		int iCols4	= 0;
		int iReal4	= 0;

		int iDim	= 0;
		int iInc	= 0;
		int iSeg	= 0;

		//get Dim
		if(GetType(3) != sci_matrix)
		{
			Err = 3;
			Error(53);
			return 0;
		}

		GetRhsVar(3, MATRIX_OF_DOUBLE_DATATYPE, &iRows3, &iCols3, &iReal3);
		iDim	= (int)stk(iReal3)[0];

		//get Dim
		if(GetType(4) != sci_matrix)
		{
			Err = 4;
			Error(53);
			return 0;
		}

		GetRhsVar(4, MATRIX_OF_DOUBLE_DATATYPE, &iRows4, &iCols4, &iReal4);
		iInc	= (int)stk(iReal4)[0];

		if(iInc == 0)
		{
			Err = 4;
			Error(36);
			return 0;
		}

		iSeg = iSize1/iDim/iInc;
		pdblWorkSpace	= (double*)malloc(sizeof(double) * iSpaceFree);
		C2F(dfft2)(pReturnReal, pReturnImg, &iSeg, &iDim, &iInc, &iWay, &iErr,
			pdblWorkSpace, &iSpaceFree);
		free(pdblWorkSpace);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	if(iComplex == 0)
		free(pdblImg1);

*/	return 0;
}
/*--------------------------------------------------------------------------*/
