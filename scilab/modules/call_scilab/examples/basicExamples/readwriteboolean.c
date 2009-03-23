/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 * 
 * This file is released into the public domain
 *
 * This example shows how to read / write a matrix of boolean from Scilab
 * engine
 */
#include <math.h>
#include <stdio.h> 
#include <stdlib.h> /* malloc */
#include <string.h> 
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
	 * Write a line matrix of boolean into Scilab
	 * A=[ T F F T ];
	 */
		{
			int A[]={1,0,0,1};   /* Declare the matrix */
			/* NOTE that it is an array of int and not an array of double */
			int rowA=1, colA=4; /* Size of the matrix */
			char variableName[]="A";

			C2F(cwritebmat)(variableName, &rowA, &colA, A,strlen(variableName)); /* Write it into Scilab's memory */

			printf("Display from Scilab of A:\n");
			SendScilabJob("disp(A);"); /* Display A */
		}

		/* 
		 * Write a matrix into Scilab
		 * B=[F F T F; 
		 *    F F F T ]
		 * Note that it is done column by column
		 */ 
		{
			int B[]={0,0,0,0,1,0,0,1};   /* Declare the matrix */
			int rowB=2, colB=4; /* Size of the matrix */
			char variableNameB[] = "B";
			C2F(cwritebmat)(variableNameB, &rowB, &colB, B, strlen(variableNameB)); /* Write it into Scilab's memory */
			printf("\n");
			printf("Display from Scilab of B:\n");
			SendScilabJob("disp(B);"); /* Display B */
		}
		
	/******************************** READ ****************************/

		/* Load the previously set variable A */
		{
			int rowA_ = 0, colA_ = 0, lp = 0;
			int i = 0;
			int *matrixOfBoolean = NULL; /* Int instead of double */

			char variableToBeRetrieved[] = "A";

			/* First, retrieve the size of the matrix */
			C2F(cmatbptr)(variableToBeRetrieved, &rowA_, &colA_, &lp, strlen(variableToBeRetrieved));

			/* Alloc the memory */
			matrixOfBoolean=(int*)malloc((rowA_*colA_)*sizeof(int));

			/* Load the matrix */
			C2F(creadbmat)(variableToBeRetrieved,&rowA_,&colA_,matrixOfBoolean,strlen(variableToBeRetrieved) );

			printf("\n");
			printf("Display from A (size: %d, %d):\n", rowA_, colA_);
			for(i=0; i < rowA_*colA_; i++)
			{
				fprintf(stdout,"A[%d] = %d\n",i,matrixOfBoolean[i]);
			}

			if (matrixOfBoolean) 
			{
				free(matrixOfBoolean);
				matrixOfBoolean=NULL;
			}
		}

		/* Load the previously set variable B */
		{
			int rowB_ = 0, colB_ = 0, lp_ = 0;
			int i = 0, j = 0;
			int *matrixOfBooleanB = NULL; /* Int instead of double */

			char variableToBeRetrievedB[] = "B";

			/* First, retrieve the size of the matrix */
			C2F(cmatbptr)(variableToBeRetrievedB, &rowB_, &colB_, &lp_, strlen(variableToBeRetrievedB));

			/* Alloc the memory */
			matrixOfBooleanB=(int*)malloc((rowB_*colB_)*sizeof(int));

			/* Load the matrix */
			C2F(creadbmat)(variableToBeRetrievedB,&rowB_,&colB_,matrixOfBooleanB,strlen(variableToBeRetrievedB) );

			printf("\n");
			printf("Display from B raw (size: %d, %d):\n",rowB_, colB_);
			for(i=0; i < rowB_*colB_; i++)
			{
				/* Display the raw matrix */
				fprintf(stdout,"B[%d] = %d\n",i,matrixOfBooleanB[i]);
			}

			printf("\n");
			printf("Display from B formated (size: %d, %d):\n",rowB_, colB_);
			for(j = 0 ; j < rowB_ ; j++)
			{
				for(i = 0 ; i < colB_ ; i++)
				{
					/* Display the formated matrix ... the way the user
					* expect */
					printf("%d ",matrixOfBooleanB[i * rowB_ + j]);
				}
				printf("\n"); /* New row of the matrix */
			}

			if (matrixOfBooleanB) 
			{
				free(matrixOfBooleanB);
				matrixOfBooleanB=NULL;
			}
		}


		if ( TerminateScilab(NULL) == FALSE ) {
			fprintf(stderr,"Error while calling TerminateScilab\n");
			return -2;
		}		
}
