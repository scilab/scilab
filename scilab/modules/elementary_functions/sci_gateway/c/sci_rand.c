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
#include "api_scilab.h"
#include "api_oldstack.h"

#define Ran1		siRandSave		//old C2F(com).ran[0]
#define Ran2		siRandType		//old C2F(com).ran[1]

const char g_pstConfigInfo[] = {"info"};
const char g_pstConfigSeed[] = {"seed"};

const char g_pstTypeUniform[] = {"uniform"};
const char g_pstTypeNormal[] = {"normal"};

int setRandType(char _cType);
double getNextRandValue(int _iRandType, int* _piRandSave, int _iForceInit);

/*--------------------------------------------------------------------------*/
int sci_rand(char *fname, int* _piKey)
{
	//save state of rand function
	static int siRandType = 0;
	static int siRandSave = 0;
	static int iForceInit	= 0;
	int iRet							= 0;
	int iRowsOut					= 0;
	int iColsOut					= 0;

	int iRandSave					= -1;
	int* piAddr						= NULL;

	if(Rhs > 3)
	{
		//TODO YaSp : %hm_rand
		return 0;
	}

	if(Rhs == 0)
	{//rand() or rand
		double *pdblReal = NULL;
		iRet = allocMatrixOfDouble(Rhs + 1, 1, 1, &pdblReal, _piKey);
		if(iRet)
		{
			return 1;
		}

		pdblReal[0] = getNextRandValue(siRandType, &siRandSave, 0);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		return 0;
	}

	iRet = getVarAddressFromPosition(1, &piAddr, _piKey);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr))
	{
	case sci_matrix : // generation functions
		{
			int *piAddrLast	= NULL;

			iRet = getVarAddressFromPosition(Rhs, &piAddrLast, _piKey);
			if(iRet)
			{
				return 1;
			}

			if(getVarType(piAddrLast) == sci_strings)
			{
				int iRowsLast					= 0;
				int iColsLast					= 0;
				int iLenLast					= 0;
				char* pstDataLast[1];

				iRet = getVarDimension(piAddrLast, &iRowsLast, &iColsLast);
				if(iRet || iRowsLast != 1 || iColsLast != 1)
				{
					return 1;
				}

				iRet = getMatrixOfString(piAddrLast, &iRowsLast, &iColsLast, &iLenLast, NULL);
				if(iRet)
				{
					return 1;
				}

				pstDataLast[0] = malloc(sizeof(char) * (iLenLast + 1));//+1 for null termination
				iRet = getMatrixOfString(piAddrLast, &iRowsLast, &iColsLast, &iLenLast, (char**)pstDataLast);
				if(iRet)
				{
					return 1;
				}
				//memorize that the law has been changed
				iRandSave = siRandType;
				//change the law temporarily
				siRandType = setRandType(pstDataLast[0][0]);
			}

			if(Rhs == 1 || (Rhs == 2 && iRandSave != -1)) //rand(A) or rand(A, "law")
			{
				if(isVarMatrixType(piAddr) == 0)
				{
					OverLoad(1);
					return 0;
				}

				iRet = getVarDimension(piAddr, &iRowsOut, &iColsOut);
				if(iRet)
				{
					return 1;
				}
			}
			else //rand(m,n) or rand(m,n,"law")
			{
				int* piAddr2	= NULL;
				iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
				if(iRet)
				{
					return 1;
				}

				iRet = getDimFromVar(piAddr, &iRowsOut);
				if(iRet)
				{
					return 1;
				}

				iRet = getDimFromVar(piAddr2, &iColsOut);
				if(iRet)
				{
					return 1;
				}
			}

			if(iRowsOut == 0 || iColsOut == 0)
			{
				double* pdblReal = NULL;
				iRet = allocMatrixOfDouble(Rhs + 1, 0, 0, &pdblReal, _piKey);
				if(iRet)
				{
					return 1;
				}
			}
			else //generate
			{
				int i;
				double* pdblReal	= NULL;
				double* pdblImg		= NULL;

				if(Rhs == 1 && isVarComplex(piAddr))
				{
					iRet = allocComplexMatrixOfDouble(Rhs + 1, iRowsOut, iColsOut, &pdblReal, &pdblImg, _piKey);
				}
				else
				{
					iRet = allocMatrixOfDouble(Rhs + 1, iRowsOut, iColsOut, &pdblReal, _piKey);
				}

				if(iRet)
				{
					return 1;
				}

				for(i = 0 ; i < iRowsOut * iColsOut ; i++)
				{
					pdblReal[i] = getNextRandValue(siRandType, &siRandSave, iForceInit);
					iForceInit = 0;
				}

				if(pdblImg)
				{
					for(i = 0 ; i < iRowsOut * iColsOut ; i++)
					{
						pdblImg[i] = getNextRandValue(siRandType, &siRandSave, 0);
					}
				}
				if(iRandSave != -1)
				{//restore old law
					siRandType = iRandSave;
				}
			}
		}
		break;
	case sci_strings : //seed, info : setup or info functions
		{
			int iRows						= 0;
			int iCols						= 0;
			int iLen						= 0;
			char* pstData[1];

			iRet = getVarDimension(piAddr, &iRows, &iCols);
			if(iRet || iRows != 1 || iCols != 1)
			{
				return 1;
			}

			iRet = getMatrixOfString(piAddr, &iRows, &iCols, &iLen, NULL);
			if(iRet)
			{
				return 1;
			}

			pstData[0] = malloc(sizeof(char) * (iLen + 1));//+1 for null termination
			iRet = getMatrixOfString(piAddr, &iRows, &iCols, &iLen, (char**)pstData);
			if(iRet)
			{
				return 1;
			}

			if(strcmp(pstData[0], g_pstConfigSeed) == 0) //seed
			{
				if(Rhs == 1) //get
				{
					iRet = createMatrixOfDoubleFromInteger(Rhs + 1, 1, 1, &siRandSave, _piKey);
					if(iRet)
					{
						return 1;
					}
				}
				else if(Rhs == 2)//set
				{
					int iRows2				= 0;
					int iCols2				= 0;
					int* piAddr2			= NULL;
					double* pdblReal2	= NULL;

					iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
					if(iRet)
					{
						return 1;
					}
					
					iRet = getVarDimension(piAddr2, &iRows2, &iCols2);
					if(iRet || iRows2 != 1 || iCols2 != 1 || isVarComplex(piAddr2))
					{
						return 1;
					}

					iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2);
					if(iRet)
					{
						return 1;
					}
					
					siRandSave = (int)Max(pdblReal2[0],0);
					iForceInit = 1;
				}
				else
				{
					return 1;
				}
			}
			else if(strcmp(pstData[0], g_pstConfigInfo) == 0) //info
			{
				char* pstData[1];
				if(Rhs > 1)
				{
					//error
					return 1;
				}
				
				if(siRandType == 0)
				{
					pstData[0] = (char*)g_pstTypeUniform;
				}
				else
				{
					pstData[0] = (char*)g_pstTypeNormal;
				}
				
				iRet = createMatrixOfString(Rhs + 1, 1, 1, pstData, _piKey);
				if(iRet)
				{
					return 1;
				}
			}
			else //uniform or normal or 'g' ( 'u', 'n', 'g' )
			{
				siRandType = setRandType(pstData[0][0]);
			}
		}
		break;
	default :
		break;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

double getNextRandValue(int _iRandType, int* _piRandSave, int _iForceInit)
{
	static int siInit				= TRUE;
	static double sdblImg		= 0;
	static double sdblR			= 0;
	double dblReal					= 0;
	double dblVal						= 0;
	double dblTemp					= 2;

	if(_iForceInit)
	{
		siInit = TRUE;
	}

	if(_iRandType == 0)
	{
		dblVal = durands(_piRandSave);
	}
	else
	{
		if(siInit == TRUE)
		{
			while(dblTemp > 1)
			{
				dblReal	= 2 * durands(_piRandSave) - 1;
				sdblImg	= 2 * durands(_piRandSave) - 1;
				dblTemp = dblReal * dblReal + sdblImg * sdblImg;
			}
			sdblR			= dsqrts(-2 * dlogs(dblTemp) / dblTemp);
			dblVal		= dblReal * sdblR;
		}
		else
		{
			dblVal	= sdblImg * sdblR;
		}
		siInit = !siInit;
	}
	return dblVal;
}

int setRandType(char _cType)
{
	switch(_cType)
	{
	case 'g' :
	case 'n' :
		return 1;
		break;
	default :
		return 0;
	}
}
/*--------------------------------------------------------------------------*/
