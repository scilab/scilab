/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
 * 
 * This file is released into the public domain
 *
 * This example shows how to read / write a matrix of string from Scilab engine
 */
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include "api_scilab.h"
#include "stack-c.h" /* Provide functions to access to the memory of Scilab */
#include "call_scilab.h" /* Provide functions to call Scilab engine */

/*------------------------------------------------------------*/
int main(void)
{
	#ifdef _MSC_VER
	if ( StartScilab(NULL,NULL,NULL) == FALSE )
	#else
	if ( StartScilab(getenv("SCI"),NULL,NULL) == FALSE )
	#endif
	{
		fprintf(stderr,"Error while calling StartScilab\n");
		return -1;
	}

 	/******************************** WRITE ****************************/

	/*
	 * Write a single string into Scilab
	 * A="my Message";
	 */
	{
		int row = 2, col = 1; /* Size of the matrix */
		/* Declare the string */
		char **myMatrixOfString = (char**)malloc(sizeof(char*) * row * col);
		myMatrixOfString[0]="my Message";
		myMatrixOfString[1]="on two lines";
		char variableName[] = "A";

		/* Write it into Scilab's memory */
		createNamedMatrixOfString(pvApiCtx, variableName, row, col, myMatrixOfString);
		/*
		 * Prior to Scilab 5.2
		 * C2F(cwritechain)(variableName, &sizeOfMyString  , myString, strlen(variableName), sizeOfMyString); 
		*/

		printf("Display from Scilab of A:\n");
		SendScilabJob("disp(A);"); /* Display A */
	}

	{
		/* 
		 * Write a matrix into Scilab
		 * B=['My' 'Great' ; 
		 *    'String' ';)' ]
		 * Note that it is done column by column
		 */ 
		printf("\n");

		int row = 2, col = 2; /* Size of the matrix */
		/* Declare the string */
		char **myMatrixOfStringB = (char**)malloc(sizeof(char*) * row * col);
		myMatrixOfStringB[0]="My";
		myMatrixOfStringB[1]="String";
		myMatrixOfStringB[2]="Great";
		myMatrixOfStringB[3]=";)";
		char variableNameB[] = "B";
		createNamedMatrixOfString(pvApiCtx, variableNameB, row, col, myMatrixOfStringB);

		printf("\n");
		printf("Display from Scilab of B:\n");
		SendScilabJob("disp(B);"); /* Display B */
	}
	/******************************** READ ****************************/

		/* Load the previously set variable A */
		{

			char variableToBeRetrieved[]="A";
			int iRows       = 0;
			int iCols       = 0;
			int i,j;
			int* piAddr     = NULL;
			int* piLen      = NULL;
			char** pstData  = NULL;

			//fisrt call to retrieve dimensions
			readNamedMatrixOfString(pvApiCtx,variableToBeRetrieved,&iRows, &iCols, NULL, NULL);
			piLen = (int*)malloc(sizeof(int) * iRows * iCols);
			//second call to retrieve length of each string
			readNamedMatrixOfString(pvApiCtx,variableToBeRetrieved, &iRows, &iCols, piLen, NULL);
			pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
			for(i = 0 ; i < iRows * iCols ; i++)
				{
					pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
				}
			//third call to retrieve data
			readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, pstData);

			printf("\n");
			printf("Load and display of A:\n");
			for(j = 0 ; j < iCols ; j++)
			{
				for(i = 0 ; i < iRows ; i++)
				{
					/* Display the formated matrix with same scilab indice */
					printf("[%d,%d] = %s\n",j+1,i+1,pstData[j* iRows + i]);
				}
			}

			printf("\n");
			free(piLen);
			for(i = 0 ; i < iRows * iCols ; i++)
				{
					free(pstData[i]);
				}
			free(pstData);
			
		}


		/* Load an element of a the previously set variable B */
		{

			char variableToBeRetrieved[]="B";
			int iRows       = 0;
			int iCols       = 0;
			int i,j;
			int* piAddr     = NULL;
			int* piLen      = NULL;
			char** pstData  = NULL;

			//fisrt call to retrieve dimensions
			readNamedMatrixOfString(pvApiCtx,variableToBeRetrieved,&iRows, &iCols, NULL, NULL);
			piLen = (int*)malloc(sizeof(int) * iRows * iCols);
			//second call to retrieve length of each string
			readNamedMatrixOfString(pvApiCtx,variableToBeRetrieved, &iRows, &iCols, piLen, NULL);
			pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
			for(i = 0 ; i < iRows * iCols ; i++)
				{
					pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
				}
			//third call to retrieve data
			readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, pstData);

			printf("\n");
			printf("Load and display of B:\n");			
			for(j = 0 ; j < iCols ; j++)
			{
				for(i = 0 ; i < iRows ; i++)
				{
					/* Display the formated matrix with same scilab indice */
					printf("[%d,%d] = %s\n",j+1,i+1,pstData[j* iRows + i]);
				}
			}

			printf("\n");
			free(piLen);
			for(i = 0 ; i < iRows * iCols ; i++)
				{
					free(pstData[i]);
				}
			free(pstData);

		}
		
	if ( TerminateScilab(NULL) == FALSE ) 
	{
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}		
}
