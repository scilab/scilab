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
	 * Write a single string into Scilab
	 * A="my Message";
	 */
	{
		char *myString = "my Message";   /* Declare the string */
		int rowA = 1, colA = 4; /* Size of the matrix */
		char variableName[] = "A";
		int sizeOfMyString=strlen(myString);

		C2F(cwritechain)(variableName, &sizeOfMyString  , myString, strlen(variableName), sizeOfMyString); /* Write it into Scilab's memory */

		printf("Display from Scilab of A:\n");
		SendScilabJob("disp(A);"); /* Display A */
	}

		/* 
		 * Write a matrix into Scilab
		 * B=['My' 'Great' ; 
		 *    'String' ';)' ]
		 * Note that it is done column by column
		 */ 
		printf("\n");
		/* 
		 * @TODO: fill from a char[][] 
		 */

		SendScilabJob("B=['My' 'Great'; 'and fantastic string',';)'];"); /* Assign */
		printf("\n");

		printf("Display from Scilab of B:\n");
		SendScilabJob("disp(B);"); /* Display B */
		
	/******************************** READ ****************************/

		/* Load the previously set variable A */
		{
			int sizeA = 0;
			char myStringFromScilab[bsiz]; /* Static char */
			int length_myStringFromScilab = bsiz; /* Max size (it is going to be 
												* changed by creadchain */
			char variableToBeRetrieved[]="A";

			/* We are loading a single string from Scilab */
			C2F(creadchain)(variableToBeRetrieved,&length_myStringFromScilab,myStringFromScilab,strlen(variableToBeRetrieved),strlen(myStringFromScilab));

			printf("\n");
			printf("Display of A (size %d): %s\n", length_myStringFromScilab, myStringFromScilab);
		}


		/* Load an element of a the previously set variable B */
		{
			char variableToBeRetrievedB[]="B";
			int nlr;

			/* First, retrieve the size of the matrix
			* disregard */
			//		C2F(cmatsptr) (variableToBeRetrievedB, &rowB_, &colB_, &ix, &j, &lp_, &nlr, strlen(variableToBeRetrievedB));
			/* @TODO: loop on the matrix */

			int indx = 2, indy = 1;
			char *tmpStr = malloc(sizeof(char)*bsiz);
			nlr = bsiz;

			/* Retrieve the element at pos indx / indy */
			C2F(creadchains)(variableToBeRetrievedB, &indx, &indy, &nlr, tmpStr, (unsigned long)strlen(variableToBeRetrievedB), (unsigned long)strlen(tmpStr));

			printf("\n");
			printf("The retrieved string(%d,%d) from B: %s\n",indx, indy, tmpStr);
		}
		
	if ( TerminateScilab(NULL) == FALSE ) 
	{
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}		
}
