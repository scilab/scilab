/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 * 
 * This file is released into the public domain
 * 
 * This example shows how to read / write a matrix from Scilab engine
 * 
 * This example works from Scilab 5.2
 * but the previous way of doing is also described as comment
 */
#include <math.h>
#include <stdio.h> 
#include <stdlib.h>
#include "api_scilab.h"
#include "stack-c.h" /* Provide functions to access to the memory of Scilab */
#include "CallScilab.h" /* Provide functions to call Scilab engine */

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
	 * Write a line matrix into Scilab
	 * A=[ 1 3 3 2 ];
	 */
	{
		double A[] = {1,3,3,2};   /* Declare the matrix */
		int rowA = 1, colA = 4; /* Size of the matrix */
		char variableName[] = "A";

		/*
		 * Write it into Scilab's memory 
		 */
		createNamedMatrixOfDouble(pvApiCtx,variableName,rowA,colA, A);
		/*
		 * Prior to Scilab 5.2:
		 * C2F(cwritemat)(variableName, &rowA, &colA, A,strlen(variableName));
		 */

		printf("Display from Scilab of A:\n");
		SendScilabJob("disp(A);"); /* Display A */
	}

		/* 
		 * Write a matrix into Scilab
		 * B=[1 4 2 3; 
		 *    3 9 8 2 ]
		 * Note that it is done column by column
		 */ 
	{
		double B[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
		int rowB = 2, colB = 4; /* Size of the matrix */
		char variableNameB[] = "B";

		/*
		 * Write it into Scilab's memory 
		 */
		createNamedMatrixOfDouble(pvApiCtx,variableNameB,rowB,colB, B);
		/*
		 * Prior to Scilab 5.2:
		 * C2F(cwritemat)(variableNameB, &rowB, &colB, B, strlen(variableNameB));
		*/

		printf("\n");
		printf("Display from Scilab of B:\n");
		SendScilabJob("disp(B);"); /* Display B */
	}	

	/******************************** READ ****************************/

		/* Load the previously set variable A */
	{
		int rowA_ = 0,colA_ = 0,lp = 0;
		int i = 0, j = 0;
		double *matrixOfDouble = NULL;

		char variableToBeRetrieved[]="A";

		/* First, retrieve the size of the matrix */
		readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, NULL);
		/* 
		 * Prior to Scilab 5.2:
		 * 	C2F(cmatptr)(variableToBeRetrieved, &rowA_, &colA_, &lp, strlen(variableToBeRetrieved));
		 */


		/* Alloc the memory */
		matrixOfDouble=(double*)malloc((rowA_*colA_)*sizeof(double));

		/* Load the matrix */
		readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, matrixOfDouble);
		/* 
		 * Prior to Scilab 5.2:
		 * C2F(creadmat)(variableToBeRetrieved,&rowA_,&colA_,matrixOfDouble,strlen(variableToBeRetrieved) );
		 */

		printf("\n");
		printf("Display from A (size: %d, %d):\n", rowA_, colA_);
		for(i=0; i < rowA_*colA_; i++)
		{
			fprintf(stdout,"A[%d] = %5.2f\n",i,matrixOfDouble[i]);
		}

		if (matrixOfDouble) 
		{
			free(matrixOfDouble);
			matrixOfDouble=NULL;
		}
	}


		/* Load the previously set variable B */
	{
		int rowB_ = 0, colB_ = 0, lp_ = 0;
		double *matrixOfDoubleB = NULL;
		int i = 0, j = 0;

		char variableToBeRetrievedB[] = "B";

		/* First, retrieve the size of the matrix */
		readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, NULL);
		/* 
		 * Prior to Scilab 5.2:
		 * C2F(cmatptr)(variableToBeRetrievedB, &rowB_, &colB_, &lp_, strlen(variableToBeRetrievedB));
		 */


		/* Alloc the memory */
		matrixOfDoubleB = (double*)malloc((rowB_*colB_)*sizeof(double));

		/* Load the matrix */
		readNamedMatrixOfDouble(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, matrixOfDoubleB);
		/* 
		 * Prior to Scilab 5.2:
		 * C2F(creadmat)(variableToBeRetrievedB,&rowB_,&colB_,matrixOfDoubleB,strlen(variableToBeRetrievedB) );
		 */


		printf("\n");
		printf("Display from B raw (size: %d, %d):\n",rowB_, colB_);
		for(i=0; i < rowB_*colB_; i++)
			{
				/* Display the raw matrix */
			fprintf(stdout,"B[%d] = %5.2f\n",i,matrixOfDoubleB[i]);
		}

		printf("\n");
		printf("Display from B formated (size: %d, %d):\n",rowB_, colB_);
		for(j = 0 ; j < rowB_ ; j++)
			{
				for(i = 0 ; i < colB_ ; i++)
					{
						/* Display the formated matrix ... the way the user
						 * expect */
						printf("%5.2f ",matrixOfDoubleB[i * rowB_ + j]);
					}
				printf("\n"); /* New row of the matrix */
			}

		if (matrixOfDoubleB) 
		{
			free(matrixOfDoubleB);
			matrixOfDoubleB=NULL;
		}
	}


	if ( TerminateScilab(NULL) == FALSE ) {
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}		
}
