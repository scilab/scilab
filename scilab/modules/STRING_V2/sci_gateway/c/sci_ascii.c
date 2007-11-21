/*-----------------------------------------------------------------------*/
/* File: sci_ascii.c                                                     */
/* Copyright INRIA 2007                                                  */
/* @Authors : Cong Wu                                                    */
/* desc : This function convert Scilab string to a vector of ascii code  */
/*        or vector of ascii code to Scilab strings.                     */
/*        If  txt  is a matrix of string,  ascii(txt)  is equivalent to  */  
/*          ascii(strcat(txt))                                           */
/*-----------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
/*-------------------------------------------------------------------------------------*/
static int asciiStrings(char *fname);
static int asciiMatrix(char *fname);
static int asciiOthers(char *fname);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_ascii) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,1);
	CheckLhs(0,1);

	switch ( GetType(1)) 
	{
		case sci_strings : 
			asciiStrings(fname);
		break;

		case sci_matrix : 
			asciiMatrix(fname);
		break;

		default:
			asciiOthers(fname);
		break;
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
static int asciiStrings(char *fname)
{
	char **Input_StringMatrix = NULL;
	int x = 0,y = 0,Row_Num = 0,Col_Num = 0;

	int Length_Output_Matrix = 0;
	double *Output_IntMatrix = NULL;
	int nbOutput_IntMatrix = 0;
	int numRow = 1;

	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_Num,&Col_Num,&Input_StringMatrix);

	Length_Output_Matrix = 0;
	for (x = 0;x < Row_Num*Col_Num;x++) Length_Output_Matrix = Length_Output_Matrix + (int)strlen(Input_StringMatrix[x]);

	if (Length_Output_Matrix !=0) 
	{
		Output_IntMatrix = (double*)MALLOC(sizeof(double)*(Length_Output_Matrix));
	}
	else Output_IntMatrix = (double*)MALLOC(sizeof(double));

	if (Output_IntMatrix == NULL)
	{
		Scierror(999,"%s : Error memory allocation.\n",fname);
		return 0;
	}

	for (x = 0; x < Row_Num*Col_Num; x++) 
	{
		for (y = 0;y < (int)strlen(Input_StringMatrix[x]); y++)
		{
			Output_IntMatrix[nbOutput_IntMatrix]=Input_StringMatrix[x][y];
			nbOutput_IntMatrix++;
		}
	}

	CreateVarFromPtr(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbOutput_IntMatrix,&Output_IntMatrix);

	LhsVar(1) = Rhs + 1 ;
	C2F(putlhsvar)();
	if (Output_IntMatrix) { FREE(Output_IntMatrix); Output_IntMatrix=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int asciiMatrix(char *fname)
{
	char **Output_StringMatrix = NULL;
	int x = 0,Row_Num = 0,Col_Num = 0,Stack_Pos = 0;
	int *Input_IntMatrix = NULL;
	int numRow = 1;
	int numCol = 0;
	int outIndex = 0 ;

	/*When input vector of integer ascii codes  */
	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&Stack_Pos);
	Input_IntMatrix = istk(Stack_Pos);

	Output_StringMatrix = (char**)MALLOC(sizeof(char*));
	if (Output_StringMatrix == NULL)
	{
		Scierror(999,"%s : Error memory allocation.\n",fname);
		return 0;
	}

	if (Row_Num*Col_Num!=0)
	{
		Output_StringMatrix[0] = (char*)MALLOC(sizeof(char*)*(Row_Num*Col_Num));
	}
	else Output_StringMatrix[0] = (char*)MALLOC(sizeof(char*));

	if (Output_StringMatrix[0] == NULL)
	{
		FREE(Output_StringMatrix);
		Output_StringMatrix = NULL;
		Scierror(999,"%s : Error memory allocation.\n",fname);
		return 0;
	}

	for (x = 0; x < Row_Num*Col_Num; x++) Output_StringMatrix[0][x] = (char)Input_IntMatrix[x];
	Output_StringMatrix[0][Row_Num*Col_Num] = 0;

	numRow   = 1 ;
	numCol   = Row_Num*Col_Num ;
	outIndex = 0 ;

	CreateVar(Rhs+1,STRING_DATATYPE,&numRow,&numCol,&outIndex);
	strncpy(cstk(outIndex), &Output_StringMatrix[0][0] , Row_Num*Col_Num ) ;
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	if (Output_StringMatrix[0]) { FREE(Output_StringMatrix[0]); Output_StringMatrix[0]=NULL;}
	if (Output_StringMatrix) {FREE(Output_StringMatrix); Output_StringMatrix=NULL; }
	return 0;
}

/*-----------------------------------------------------------------------------------*/
static int asciiOthers(char *fname)
{
	Scierror(999,"%s : Invalid parameter.\n",fname);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
