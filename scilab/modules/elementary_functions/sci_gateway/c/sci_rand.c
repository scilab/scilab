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
/*--------------------------------------------------------------------------*/ 
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"

#define Ran1		C2F(com).ran[0]
#define Ran2		C2F(com).ran[1]

//#define _NEW_TONIO_

double GetNextRandValue();

/*--------------------------------------------------------------------------*/
extern int C2F(intrand) (char *fname,int *id, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_rand) (char *fname,unsigned long fname_len)
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows		= 0;
	int iCols		= 0;
	int iRealData	= 0;
	int iImgData	= 0;
	int iIndex		= 0;
	int iMode		= 0;
	int iRan2Save	= 0;
	int iRan2Change	= 0;
	int iComplex	= 0;
	double dblVal			= 0;
	double *pReturnData		= NULL;
	double *pdblData		= NULL;
	CheckLhs(1,1);

	Rhs = Max(0, Rhs);
	if(Rhs >= 3)
	{
		//trouver un moyen d'appeller %hm_rand :(
		return 0;
	}

	if(Rhs == 0)
	{
		iRows = 1;
		iCols = 1;
		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnData);
		//pReturnData = (double*)malloc(sizeof(double) * iRows * iCols);

		pReturnData[0] = GetNextRandValue();

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,&iRows, &iCols, &pReturnData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnData);
		return 0;
	}

	if(GetType(1) == sci_strings)
	{
		char **szRealData	= 0;
		GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
		if(strcmp(*szRealData, "seed") == 0)
		{
			CheckRhs(1,2);
			if(Rhs == 1)
			{
				iRows = 1;
				iCols = 1;
				iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnData);
				//pReturnData = (double*)malloc(sizeof(double) * iRows * iCols);
				pReturnData[0] = Ran1;
				//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE,&iRows, &iCols, &pReturnData);
				LhsVar(1) = Rhs + 1;
				PutLhsVar();
				//free(pReturnData);
			}
			else
			{
				CheckRhs(2,2);
				if(GetType(2) != sci_matrix)
				{
					Error(53);
					return 0;
				}

				GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
				pdblData = stk(iRealData);
				Ran1 = max(0, (int)pdblData[0]);
				//Attention au repercution de ca
				//siInit = TRUE;
				Top--;
				C2F(objvide)(fname, &Top, fname_len);
			}
		}
		else if(strcmp(*szRealData, "info") == 0)
		{
			//char **sz;
			char *sz;
			int iLen = 20;

			CheckRhs(1,1);
			iRows = 1;
			iCols = 1;

//			sz = (char**)malloc(sizeof(char**)*iRows*iCols);
//			sz[0] = (char*)malloc(sizeof(char*)*20);

			if(Ran2 == 0)
			{
				iLen = strlen("uniform");
				iAllocMatricOfString(Rhs + 1, iRows, iCols, &iLen, &sz);
				memcpy(sz, "uniform", iLen);
			}
			else if(Ran2 == 1)
			{
				iLen = strlen("normal");
				iAllocMatricOfString(Rhs + 1, iRows, iCols, &iLen, &sz);
				memcpy(sz, "normal", iLen);
			}
			else
			{
				C2F(objvide)(fname, &Top, fname_len);
			}

			//iRows = 1;
			//iCols = 1;
			//CreateVarFromPtr(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, sz);
			LhsVar(1) = Rhs + 1;
			PutLhsVar();
			//free(sz[0]);
			//free(sz);
		}
		else
		{
			CheckRhs(1,1);
			Ran2 = 0;
			if(szRealData[0][0] == 'g' || szRealData[0][0] == 'n')
				Ran2 = 1;
			C2F(objvide)(fname, &Top, fname_len);
		}
		return 0;
	}

	if(GetType(Rhs) == sci_strings)
	{
		if(Rhs > 3)
		{
			//trouver un moyen d'appeller %hm_rand
			return 0;
		}
		else
		{
			char **szRealData	= 0;
			GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &iRows, &iCols, &szRealData);
			iRan2Save = Ran2;
			iRan2Change = 1;
			Ran2 = 0;
			if(szRealData[0][0] == 'g' || szRealData[0][0] == 'n')
				Ran2 = 1;
			Top -= 1;
		}
	}

	if(Rhs - iRan2Change >= 3)
	{
		//trouver un moyen d'appeller %hm_rand
		return 0;
	}


	if(Rhs - iRan2Change == 2)
	{//rand(n1, n2)
		GetDimFromVar(Top, Rhs - iRan2Change, &iCols);
		GetDimFromVar(Top - 1, Rhs - iRan2Change - 1, &iRows);
	}
	else
	{//rand(n1)
		int iType = GetType(Top);
		if(iType <= sci_strings)
		{
			GetVarDimension(Top, &iRows, &iCols);
			if((iType <= sci_poly || iType == sci_sparse) && iIsComplex(Top))
				iComplex = 1;
		}
		else
		{
			OverLoad(1);
			return 0;
		}
	}

	if(iRows == 0 || iCols == 0)
	{
		iRows		= 0;
		iCols		= 0;
		pReturnData = (double*)malloc(sizeof(double));

		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData);
		LhsVar(1)	= Rhs + 1;
		PutLhsVar();
		free(pReturnData);
		return 0;
	}

	pReturnData = (double*)malloc(sizeof(double) * iRows * iCols);

	for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
		pReturnData[iIndex] = GetNextRandValue();

	if(iComplex)
	{
		double* pReturnImgData = (double*)malloc(sizeof(double) * iRows * iCols);
		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			pReturnImgData[iIndex] = GetNextRandValue();

		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnData, &pReturnImgData);
		LhsVar(1)	= Rhs + 1;
		PutLhsVar();
		free(pReturnData);
		free(pReturnImgData);
	}
	else
	{
		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnData);
		LhsVar(1)	= Rhs + 1;
		PutLhsVar();
		free(pReturnData);
	}



#else
	C2F(intrand)(fname,id, fname_len);
#endif
	return 0;
}

double GetNextRandValue()
{
	static int siInit		= TRUE;
	static double sdblImg	= 0;
	static double sdblR		= 0;
	double dblReal			= 0;
	double dblVal			= 0;
	double dblTemp			= 2;

	if(Ran2 == 0)
	{
		dblVal = durands(&Ran1);
	}
	else
	{
		if(siInit == TRUE)
		{
			while(dblTemp > 1)
			{
				sdblImg	= 2 * durands(&Ran1) - 1;
				dblReal	= 2 * durands(&Ran1) - 1;
				dblTemp = dblReal * dblReal + sdblImg * sdblImg;
			}
			sdblR	= dsqrts(-2 * dlogs(dblTemp) / dblTemp);
			dblVal	= dblReal * sdblR;
		}
		else
		{
			dblVal	= sdblImg * sdblR;
		}
		siInit = !siInit;
	}
	return dblVal;
}

/*--------------------------------------------------------------------------*/
