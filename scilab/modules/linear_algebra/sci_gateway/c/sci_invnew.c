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
/*--------------------------------------------------------------------------*/ 

#include <stdio.h>
#include <stdlib.h>
#include "stack-c.h"
#include "../../core/src/c/parse.h"


int iInverseMatrix(double *_pdblIn, int _iLeadDim, double *_pdblOut);
int iInverseComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iLeadDim, double *_pdblRealOut, double *_pdblImgOut);
/*--------------------------------------------------------------------------*/
extern double	C2F(dlamch)();

//Real fortran function
extern int		C2F(dgecon)();
extern int		C2F(dgetrf)();
extern int		C2F(dgetri)();
extern double	C2F(dlange)();
extern int		C2F(dcopy)();

//Complex fortran function
extern int		C2F(zgecon)();
extern int		C2F(zgetrf)();
extern int		C2F(zgetri)();
extern double	C2F(zlange)();


//extern int C2F(intdgetri)(char *fname, unsigned long fname_len);
//extern int C2F(intzgetri)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(invnew)(char *fname,unsigned long fname_len)
{
	int iRows		= 0;
	int iCols		= 0;
	int iReal		= 0;
	int iImg		= 0;
	int iComplex	= 0;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	double *pReturnReal	= NULL;
	double *pReturnImg	= NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix) 
	{
		OverLoad(1);
		return 0;
	}
	

	//check dimension of input data
	GetVarDimension(1, &iRows, &iCols);

	if(iCols == 0) //matrix []
	{
		CheckVarUsed(1);
		LhsVar(1) = 1;
		return 0;
	}

	if(iRows != iCols)
	{
		Err = 1;
		Error(20);
		return 0;
	}


	//retrieve data
	iComplex = iIsComplex(1);
	if(iComplex)
	{
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iReal, &iImg);
		pdblReal		= stk(iReal);
		pdblImg			= stk(iImg);
	}
	else
	{
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iReal);
		pdblReal		= stk(iReal);
	}


	//Alloc output data in scilab stack
	if(iComplex)
	{
		iAllocComplexMatrixOfDouble(Rhs + 1, iComplex, iRows, iCols, &pReturnReal, &pReturnImg);
		iInverseComplexMatrix(pdblReal, pdblImg, iRows, pReturnReal, pReturnImg);
	}
	else
	{
		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnReal);
		iInverseMatrix(pdblReal, iRows, pReturnReal);
	}

	//set return variable
	LhsVar(1) = Rhs + 1;

	return 0;
}

int iInverseMatrix(double *_pdblIn, int _iLeadDim, double *_pdblOut)
{
	int iOne				= 1;
	char cNorm				= 0;
	int iInfo				= 0;
	int iWork				= -1;
	int iSize				= _iLeadDim * _iLeadDim;
	double dblAnorm			= 0;
	double dblRcond			= 0;

	double *pdblWorkSpace	= NULL;
	int *piPiv				= NULL;
	int *piWork				= NULL;
	double pdblWS[1];


	//standard Workspace for DGECON function

	piPiv			= (int*)malloc(sizeof(int) * _iLeadDim);
	piWork			= (int*)malloc(sizeof(int) * _iLeadDim);

	//Call dgetri with LWORD = -1 to compute the best size of the workspace array WORK
	C2F(dgetri)(&_iLeadDim, _pdblOut, &_iLeadDim, piPiv, pdblWS, &iWork, &iInfo);
	iWork = (int)pdblWS[0];

	pdblWorkSpace	= (double*)malloc(sizeof(double) * iWork); 

	C2F(dcopy)(&iSize, _pdblIn, &iOne, _pdblOut, &iOne);

	cNorm			= '1';
	dblAnorm		= C2F(dlange)(&cNorm, &_iLeadDim, &_iLeadDim, _pdblOut, &_iLeadDim, pdblWorkSpace);

	C2F(dgetrf)(&_iLeadDim, &_iLeadDim, _pdblOut, &_iLeadDim, piPiv, &iInfo);

	if(iInfo == 0)
	{
		dblRcond	= 0;
		cNorm = '1';
		C2F(dgecon)(&cNorm, &_iLeadDim, _pdblOut, &_iLeadDim, &dblAnorm, 
			&dblRcond, pdblWorkSpace, piWork, &iInfo);
		if(dblRcond <= sqrt(F2C(dlamch)("eps",1L)))
		{
			sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
			Msgs(5,0);
		}
		C2F(dgetri)(&_iLeadDim, _pdblOut, &_iLeadDim, piPiv, pdblWorkSpace, &iWork, &iInfo);
	}

	free(piPiv);
	free(piWork);
	free(pdblWorkSpace);
	if(iInfo > 0)
		Error(19);

	return 0;
}


/*
doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize);
void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg);
void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex);
*/
int iInverseComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iLeadDim, double *_pdblRealOut, double *_pdblImgOut)
{
	int iOne				= 1;
	char cNorm				= 0;
	int iInfo				= 0;
	int iWork				= -1;
	int iSize				= _iLeadDim * _iLeadDim;
	double dblAnorm			= 0;
	double dblRcond			= 0;

	doublecomplex *poWS		= NULL;
	doublecomplex *poOut	= NULL;
	int *piPiv				= NULL;
	double *pdblWork		= NULL;
	doublecomplex pdblWS[1];


	poOut			= oGetDoubleComplexFromPointer(_pdblRealIn, _pdblImgIn, iSize);
	//standard Workspace for DGECON function

	piPiv			= (int*)malloc(sizeof(int) * _iLeadDim);
	pdblWork		= (double*)malloc(sizeof(double) * _iLeadDim * 2);

	//Call dgetri with LWORD = -1 to compute the best size of the workspace array WORK
	C2F(zgetri)(&_iLeadDim, poOut, &_iLeadDim, piPiv, pdblWS, &iWork, &iInfo);
	iWork = (int)pdblWS[0].r;

	poWS	= (doublecomplex*)malloc(sizeof(doublecomplex) * iWork); 

	cNorm			= '1';
	dblAnorm		= C2F(zlange)(&cNorm, &_iLeadDim, &_iLeadDim, poOut, &_iLeadDim, poWS);

	C2F(zgetrf)(&_iLeadDim, &_iLeadDim, poOut, &_iLeadDim, piPiv, &iInfo);

	if(iInfo == 0)
	{
		dblRcond	= 0;
		cNorm = '1';
		C2F(zgecon)(&cNorm, &_iLeadDim, poOut, &_iLeadDim, &dblAnorm, 
			&dblRcond, poWS, pdblWork, &iInfo);
		if(dblRcond <= sqrt(F2C(dlamch)("eps",1L)))
		{
			sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
			Msgs(5,0);
		}
		C2F(zgetri)(&_iLeadDim, poOut, &_iLeadDim, piPiv, poWS, &iWork, &iInfo);
		vGetPointerFromDoubleComplex(poOut, iSize, _pdblRealOut, _pdblImgOut);
	}

	//free memory
	free(piPiv);
	free(pdblWork);
	free(poWS);
	vFreeDoubleComplexFromPointer(poOut);
	if(iInfo > 0)
		Error(19);

	return 0;
	
}

/*--------------------------------------------------------------------------*/
