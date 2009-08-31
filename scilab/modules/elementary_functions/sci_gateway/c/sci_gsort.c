/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Jean-Baptiste SILVY
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2007 - INRIA - Cong WU
* Copyright (C) 2009 - INRIA - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "gsort.h"
#include "string.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sortTemplate.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "isanan.h"
#include "api_scilab.h"
#include "api_oldstack.h"

const char g_pcstProcessGlobal[] = "g";
const char g_pcstProcessRow[] = "r";
const char g_pcstProcessCol[] = "c";
const char g_pcstProcessListRow[] = "lr";
const char g_pcstProcessListCol[] = "lc";

const char g_pcstWayDec[] = "d";
const char g_pcstWayInc[] = "i";
/* Next define intruction put here instead of stack-c.h waiting for
more genral usage stack-c.h macros CreateVarFromPtr and CreateVar
contains a "return 0" which make impossible to free indices in case
of error
*/
#define CreateVarFromPtrNoCheck(n,ct,mx,nx,lx) C2F(createvarfromptr)((c_local=n,&c_local),ct,mx,nx,(void *)lx,1L)
#define CreateVarNoCheck(n,ct,mx,nx,lx) C2F(createvar)((c_local=n,&c_local),ct,mx,nx,(void *)lx, 1L)

int getVarString(int* _piAddress, char*** _pstData, int* _piRows, int* _piCols);
int checkInputValue(char* _pstInput, const char** _pcstRef, int _iRefCount);
char* getWayType(int* _piAddress);
char* getProcessType(int* _piAddress);
int computeIndiceDim(const char* _pcstProcess, int _iRowsIn, int _iColsIn,  int* _piRowsOut, int* _piColsOut);

int	gsort_int(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey);
int	gsort_string(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey);
int	gsort_double(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey);
/*--------------------------------------------------------------------------*/
static int gsort_complex(char *fname, char *mode, char *order);
/*--------------------------------------------------------------------------*/
int sci_gsort(char *fname, int* _piKey)
{
	int iRet								= 0;

	int iRows1							= 0;
	int iCols1							= 0;

	int* piAddr1						= NULL;
	int* piAddr2						= NULL;
	int* piAddr3						= NULL;

	const char* pcstProcess	= g_pcstProcessGlobal;
	const char* pcstWay			= g_pcstWayDec;

	CheckRhs(1,3);
	CheckLhs(1,2);

	if(Rhs > 1)
	{//get option
		iRet = getVarAddressFromPosition(2, &piAddr2, _piKey);
		if(iRet)
		{
			return 1;
		}

		pcstProcess = getProcessType(piAddr2); 
		if(pcstProcess == NULL)
		{
			return 1;
		}
	}

	if(Rhs == 3)
	{//get way ( inc or dec )
		iRet = getVarAddressFromPosition(3, &piAddr3, _piKey);
		if(iRet)
		{
			return 1;
		}

		pcstWay = getWayType(piAddr3); 
		if(pcstWay == NULL)
		{
			return 1;
		}
	}

	iRet = getVarAddressFromPosition(1, &piAddr1, _piKey);
	if(iRet)
	{
		return 1;
	}

	switch(getVarType(piAddr1))
	{
	case sci_matrix : 
		gsort_double(piAddr1, pcstProcess, pcstWay, _piKey);
		break;
	case sci_strings : 
		gsort_string(piAddr1, pcstProcess, pcstWay, _piKey);
		break;
	case sci_ints : 
		gsort_int(piAddr1, pcstProcess, pcstWay, _piKey);
		break;
	default :
		return 1;
	}

	//create output vars
	return 0;
/*
	char iord[2] ;
	char typex[10];
	SciIntMat Im;
	int Type = 0;
	char **S = NULL;
	int m1 = 0,n1 = 0,l1 = 0 ;
	int m2 = 0,n2 = 0,l2 = 0 ;
	int m3 = 0,n3 = 0,l3 = 0 ;
	int ind_m1 = 0, ind_n1 = 0, ind_l1 =0;
	int *indices = NULL;
	int iflag = 0;
	int i;

	iord[0] = DECREASE_COMMAND; 
	iord[1]='\0';

	typex[0] = GLOBAL_SORT; 
	typex[1] = '\0';

	Rhs = Max(0, Rhs);

	if (Rhs >= 1)
	{
		Type=VarType(1);
		switch (Type)
		{
		case sci_strings :
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&S);
			break;
		case sci_matrix :
			{
#define COMPLEX 1
				int *header = NULL;
				int Cmplx = 0; 
				header = (int *) GetData(1);   
				Cmplx=header[3];

				if (Cmplx == COMPLEX)
				{
					return gsort_complex(fname, typex, iord);
				}
				else
				{
					GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
					if ( (m1 * n1) == 0 ) // [] returns []
					{
						int m = 0, n = 0, l = 0;
						CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
						LhsVar(1) = Rhs+1 ;
						if (Lhs == 2)
						{
							CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
							LhsVar(2) = Rhs+2 ;
						}
						C2F(putlhsvar)();
						return 0;
					}
				}
			}
			break;
		case sci_ints:
			GetRhsVar(1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&m1,&n1,&Im);
			break;
		default :
			Scierror(999,_("%s: Wrong type for input argument #%d: Real, complex, int matrix or matrix of strings expected.\n"),fname,2);
			return 0;
			break;
		}
	}

	if (Rhs == 3)
	{
		GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
		CheckLength(3,m3,1);
		if ( (*cstk(l3) != INCREASE_COMMAND) && (*cstk(l3) != DECREASE_COMMAND) )
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"),fname,3,"i","d");
			return 0;
		}
		iord[0] = *cstk(l3);
	}

	if (Rhs >= 2)
	{
		char c;
		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		if ( m2 == 0 )
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: Non-empty string expected.\n"),fname,2);
			return 0;
		}
		c = *cstk(l2);
		if ( (c != ROW_SORT) && (c != COLUMN_SORT) && (c != GLOBAL_SORT) && (c != LIST_SORT) )
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),fname,2,"r","c","g","lr","lc");
			return 0;
		}
		strcpy(typex,cstk(l2));
	}

	if ( typex[0] == LIST_SORT) 
	{
		if (typex[1] == ROW_SORT) 
		{
			ind_m1 = m1;
			ind_n1 = 1;
			ind_l1 = 0;
			if (ind_m1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_m1));   // Only return in row
		}
		else if (typex[1] == COLUMN_SORT) 
		{
			ind_m1 = 1;
			ind_n1 = n1;
			ind_l1 = 0;
			if (ind_n1 != 0) indices = (int*)MALLOC(sizeof(int)*(ind_n1));  //Only return in col 
		}
		else
		{
			Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname,2);
			return 0;
		}
	}
	else 
	{
		ind_m1 = m1;
		ind_n1 = n1;
		ind_l1 = 0;
		if ( ind_m1*ind_n1 != 0 )indices = (int*)MALLOC(sizeof(int)*(ind_m1*ind_n1));  // return a matrix
	}

	if (Lhs == 2) iflag = 1; 
	else iflag = 0;

	switch (Type)
	{
	case sci_matrix:
		{
			if ( m1*n1 != 0 ) 
			{
				int lr;
				double *matrix = stk(l1);
				double *tmp_matrix = NULL;
				// next CreateVar and corresponding copy not needed if arg1 is not passed by reference 
				if (!CreateVarNoCheck(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&lr)) 
				{
					if (indices) {FREE(indices); indices = NULL;}
					return 0;
				}

				tmp_matrix = stk(lr);
				for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];

				C2F(gsortd)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				LhsVar(1)= Rhs+1 ;

				if (Lhs == 2)
				{
					if (!CreateVarFromPtrNoCheck(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)) 
					{
						if (indices) {FREE(indices); indices = NULL;}
						return 0;
					}
					LhsVar(2)= Rhs+2 ;
				}
				C2F(putlhsvar)();
				if (indices) {FREE(indices); indices = NULL;}
			}
		}
		break;

	case sci_ints:               
		{
			int lr;
			lr=Im.it;
			// next CreateVar and corresponding copy not needed if arg1 is not passed by reference 
			if (!CreateVarNoCheck(Rhs+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&m1,&n1,&lr)) 
			{
				if (indices) {FREE(indices); indices = NULL;}
				return 0;
			}

			switch(Im.it) // Type defined in stack-c.h 
			{
			case I_CHAR :
				{
					char *matrix = Im.D;
					char *tmp_matrix = (char*) istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortchar)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;

			case I_INT32 :
				{
					int *matrix = Im.D;
					int *tmp_matrix = istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortint)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;
			case I_UCHAR :
				{
					unsigned char *matrix = Im.D;
					unsigned char *tmp_matrix = (unsigned char*) istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortuchar)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;
			case I_INT16 :
				{
					short *matrix = Im.D;
					short *tmp_matrix = (short*) istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortshort)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;
			case I_UINT16 :
				{
					unsigned short *matrix = Im.D;
					unsigned short *tmp_matrix = (short*) istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortushort)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;
			case I_UINT32 :
				{
					unsigned int *matrix = Im.D;
					unsigned int *tmp_matrix = (unsigned int *)istk(lr);
					for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
					C2F(gsortuint)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
				}
				break;
			default:
				if (indices) {FREE(indices); indices = NULL;}
				Scierror(999,_("%s: Wrong type for input argument #%d: Unknown type.\n"),fname,1);
				return 0;
			}

			LhsVar(1) = Rhs + 1 ;

			if (Lhs == 2)
			{
				if (!CreateVarFromPtrNoCheck(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)) 
				{
					if (indices) {FREE(indices); indices = NULL;}
					return 0;
				}
				LhsVar(2)= Rhs+2 ;
			}
			if (indices) {FREE(indices); indices = NULL;}
			C2F(putlhsvar)();
		}
		break;

	case sci_strings:
		{
			C2F(gsorts)(S,indices,&iflag,&m1,&n1,typex,iord);
			if (!CreateVarFromPtrNoCheck(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, S)) 
			{
				if (indices) {FREE(indices); indices = NULL;}
				return 0;
			}
			LhsVar(1)=Rhs+1;

			if (Lhs == 2)
			{
				if (!CreateVarFromPtrNoCheck(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)) 
				{
					if (indices) {FREE(indices); indices = NULL;}
					return 0;
				}
				LhsVar(2)= Rhs+2 ;
			}
			C2F(putlhsvar)();
			if (indices) {FREE(indices); indices = NULL;}
			freeArrayOfString(S,m1*n1);
		}
		break;

	default:
		if (indices) {FREE(indices); indices = NULL;}
		Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname,1);
		return 0;
		break;
	}
	return 0;
*/
} 


int	gsort_double(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey)
{
	int iRet							= 0;
	int iRows							= 0;
	int iCols							= 0;
	int iRowsInd					= 0;
	int iColsInd					= 0;
	double* pdblReal			= NULL;
	double* pdblRealOut1	= NULL;
	double* pdblRealOut2	= NULL;
	int *piInd						= NULL;
	int iRhs2							= Rhs > 2;

	if(isVarComplex(_piAddress))
	{
		OverLoad(1);
		return 0;
	}
	iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	if(iRows == 0 && iCols == 0)
	{
		iRet = allocMatrixOfDouble(Rhs + 1, 0, 0, &pdblReal, _piKey);
		if(iRet)
		{
			return 1;
		}

		if(Lhs == 2)
		{
			iRet = allocMatrixOfDouble(Rhs + 2, 0, 0, &pdblReal, _piKey);
			if(iRet)
			{
				return 1;
			}
		}
		return 0;
	}

	iRet = allocMatrixOfDouble(Rhs + 1, iRows, iCols, &pdblRealOut1, _piKey);
	if(iRet)
	{
		return 1;
	}

	memcpy(pdblRealOut1, pdblReal, sizeof(double) * iRows * iCols);
	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		return 1;
	}

	piInd = (int*)malloc(sizeof(int) * iRowsInd * iColsInd);
	C2F(gsortd)(pdblRealOut1, piInd , &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(Rhs + 2, iRowsInd, iColsInd, piInd, _piKey);
		if(iRet)
		{
			free(piInd);
			return 1;
		}
	}

	free(piInd);
	return 0;
}

int	gsort_string(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey)
{
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int iRowsInd		= 0;
	int iColsInd		= 0;
	int iRhs2				= Rhs > 2;

	int* piInd			= NULL;
	char** pstData	= NULL;

	iRet = getVarString(_piAddress, &pstData, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		return 1;
	}

	piInd = (int*)malloc(sizeof(int) * iRowsInd * iColsInd);
	C2F(gsorts)(pstData, piInd, &iRhs2, &iRows,&iCols, (char*)_pcstProcess, (char*)_pcstWay);
	
	iRet = createMatrixOfString(Rhs + 1, iRows, iCols, pstData, _piKey);
	if(iRet)
	{
		return 1;
	}

	if(Lhs == 2)
	{
		iRet = createMatrixOfDoubleFromInteger(Rhs + 2, iRowsInd, iColsInd, piInd, _piKey);
		if(iRet)
		{
			free(piInd);
			return 1;
		}
	}
	
	free(piInd);
	return 0;
}

int	gsort_int(int * _piAddress, const char* _pcstProcess, const char* _pcstWay, int* _piKey)
{
	int iRet			= 0;
	int iRows			= 0;
	int iCols			= 0;
	int iRowsInd	= 0;
	int iColsInd	= 0;
	int iPrec			= 0;
	int iRhs2			= Rhs > 2;

	int* piInd		= NULL;

	iRet = computeIndiceDim(_pcstProcess, iRows, iCols, &iRowsInd, &iColsInd);
	if(iRet)
	{
		return 1;
	}

	piInd = (int*)malloc(sizeof(int) * iRowsInd * iColsInd);

	iRet = getMatrixOfIntegerPrecision(_piAddress, &iPrec);
	switch(iPrec)
	{
	case SCI_INT8 : 
		{
			char* pcDataIn		= NULL;
			char* pcDataOut		= NULL;
			iRet = getMatrixOfInteger8(_piAddress, &iRows, &iCols, &pcDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfInteger8(Rhs + 1, iRows, iCols, &pcDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(pcDataOut, pcDataIn, sizeof(char) * iRows * iCols);
			C2F(gsortchar)(pcDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT8 : 
		{
			unsigned char* pucDataIn		= NULL;
			unsigned char* pucDataOut		= NULL;
			iRet = getMatrixOfUnsignedInteger8(_piAddress, &iRows, &iCols, &pucDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfUnsignedInteger8(Rhs + 1, iRows, iCols, &pucDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(pucDataOut, pucDataIn, sizeof(unsigned char) * iRows * iCols);
			C2F(gsortuchar)(pucDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_INT16 : 
		{
			short* psDataIn		= NULL;
			short* psDataOut	= NULL;
			iRet = getMatrixOfInteger16(_piAddress, &iRows, &iCols, &psDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfInteger16(Rhs + 1, iRows, iCols, &psDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(psDataOut, psDataIn, sizeof(short) * iRows * iCols);
			C2F(gsortshort)(psDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT16 : 
		{
			unsigned short* pusDataIn		= NULL;
			unsigned short* pusDataOut	= NULL;
			iRet = getMatrixOfUnsignedInteger16(_piAddress, &iRows, &iCols, &pusDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfUnsignedInteger16(Rhs + 1, iRows, iCols, &pusDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(pusDataOut, pusDataIn, sizeof(unsigned short) * iRows * iCols);
			C2F(gsortushort)(pusDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_INT32 : 
		{
			int* piDataIn		= NULL;
			int* piDataOut	= NULL;
			iRet = getMatrixOfInteger32(_piAddress, &iRows, &iCols, &piDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfInteger32(Rhs + 1, iRows, iCols, &piDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(piDataOut, piDataIn, sizeof(int) * iRows * iCols);
			C2F(gsortint)(piDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	case SCI_UINT32 : 
		{
			unsigned int* puiDataIn		= NULL;
			unsigned int* puiDataOut	= NULL;
			iRet = getMatrixOfUnsignedInteger32(_piAddress, &iRows, &iCols, &puiDataIn);
			if(iRet)
			{
				return 1;
			}

			iRet = allocMatrixOfUnsignedInteger32(Rhs + 1, iRows, iCols, &puiDataOut, _piKey);
			if(iRet)
			{
				return 1;
			}

			memcpy(puiDataOut, puiDataIn, sizeof(unsigned int) * iRows * iCols);
			C2F(gsortuint)(puiDataOut, piInd, &iRhs2, &iRows, &iCols, (char*)_pcstProcess, (char*)_pcstWay);
			break;
		}
	default : 
		return 1;
	}
	return 0;
/*
	int lr;
	lr=Im.it;
	// next CreateVar and corresponding copy not needed if arg1 is not passed by reference 
	if (!CreateVarNoCheck(Rhs+1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&m1,&n1,&lr)) 
	{
		if (indices) {FREE(indices); indices = NULL;}
			return 0;
	}

	switch(Im.it) // Type defined in stack-c.h 
	{
		case I_CHAR :
		{
			char *matrix = Im.D;
			char *tmp_matrix = (char*) istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortchar)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;

		case I_INT32 :
		{
			int *matrix = Im.D;
			int *tmp_matrix = istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortint)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;
		case I_UCHAR :
		{
			unsigned char *matrix = Im.D;
			unsigned char *tmp_matrix = (unsigned char*) istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortuchar)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;
		case I_INT16 :
		{
			short *matrix = Im.D;
			short *tmp_matrix = (short*) istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortshort)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;
		case I_UINT16 :
		{
			unsigned short *matrix = Im.D;
			unsigned short *tmp_matrix = (short*) istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortushort)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;
		case I_UINT32 :
		{
			unsigned int *matrix = Im.D;
			unsigned int *tmp_matrix = (unsigned int *)istk(lr);
			for (i = 0;i< m1*n1; i++) tmp_matrix[i] = matrix[i];
			C2F(gsortuint)(tmp_matrix,indices,&iflag,&m1,&n1,typex,iord);
		}
		break;
		default:
			if (indices) {FREE(indices); indices = NULL;}
			Scierror(999,_("%s: Wrong type for input argument #%d: Unknown type.\n"),fname,1);
			return 0;
	}

	LhsVar(1) = Rhs + 1 ;

	if (Lhs == 2)
	{
	if (!CreateVarFromPtrNoCheck(Rhs+2,MATRIX_OF_INTEGER_DATATYPE,&ind_m1,&ind_n1,&indices)) 
	{
	if (indices) {FREE(indices); indices = NULL;}
	return 0;
	}
	LhsVar(2)= Rhs+2 ;
	}
	if (indices) {FREE(indices); indices = NULL;}
	C2F(putlhsvar)();
*/
}

static int gsort_complex(char *fname, char *mode, char *order)
{
	int lw = 0;
	C2F(overload)(&lw, "gsort", (unsigned long)strlen("gsort"));
	return 0;
}
/*-----------------------------------------------------------------------------------*/

char* getWayType(int* _piAddress)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	char** pstWay = NULL;

	const char* pcstRef[2] = {g_pcstWayDec,g_pcstWayInc};
	iRet = getVarString(_piAddress, &pstWay, &iRows, &iCols);
	if(iRet)
	{
		return NULL;
	}

	if(iRows == 1 && iCols == 1)
	{
		iRet = checkInputValue(pstWay[0], pcstRef, 2);
	}
	else
	{
		iRet = 1;
	}

	if(iRet)
	{
		int i;
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pstWay[i]);
		}
		free(pstWay);
		return NULL;
	}
	return pstWay[0];
}

char* getProcessType(int* _piAddress)
{
	int iRet					= 0;
	int iRows					= 0;
	int iCols					= 0;
	char** pstProcess = NULL;

	const char* pcstRef[5] = {g_pcstProcessGlobal,g_pcstProcessRow,g_pcstProcessCol,g_pcstProcessListRow,g_pcstProcessListCol};
	iRet = getVarString(_piAddress, &pstProcess, &iRows, &iCols);
	if(iRet)
	{
		return NULL;
	}

	if(iRows == 1 && iCols == 1)
	{
		iRet = checkInputValue(pstProcess[0], pcstRef, 5);
	}
	else
	{
		iRet = 1;
	}

	if(iRet)
	{
		int i;
		for(i = 0 ; i < iRows * iCols ; i++)
		{
			free(pstProcess[i]);
		}
		free(pstProcess);
		return NULL;
	}
	return pstProcess[0];
}

int getVarString(int* _piAddress, char*** _pstData, int* _piRows, int* _piCols)
{
	int i;
	int iRet				= 0;
	int iRows				= 0;
	int iCols				= 0;
	int *piLen			= NULL;

	if(getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}

	iRet = getVarDimension(_piAddress, &iRows, &iCols);
	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, NULL, NULL);
	if(iRet)
	{
		return 1;
	}

	piLen = (int*)malloc(sizeof(int) * iRows * iCols);
	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, NULL);
	if(iRet)
	{
		return 1;
	}

	*_pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		(*_pstData)[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1)); //+1 for null termination
	}

	iRet = getMatrixOfString(_piAddress, &iRows, &iCols, piLen, *_pstData);
	if(iRet)
	{
		return 1;
	}

	*_piRows = iRows;
	*_piCols = iCols;
	return 0;
}

int checkInputValue(char* _pstInput, const char** _pcstRef, int _iRefCount)
{
	int i;
	for(i = 0 ; i < _iRefCount ; i++)
	{
		if(strcmp(_pstInput, _pcstRef[i]) == 0)
		{//find
			return 0;
		}
	}
	//not find
	return 1;
}

int computeIndiceDim(const char* _pcstProcess, int _iRowsIn, int _iColsIn,  int* _piRowsOut, int* _piColsOut)
{
	if(strcmp(_pcstProcess, g_pcstProcessListRow) == 0)
	{
		*_piRowsOut = _iRowsIn;
		*_piColsOut = 1;
	}
	else if(strcmp(_pcstProcess, g_pcstProcessListCol) == 0)
	{
		*_piRowsOut = 1;
		*_piColsOut = _iColsIn;
	}
	else
	{
		*_piRowsOut = _iRowsIn;
		*_piColsOut = _iColsIn;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
