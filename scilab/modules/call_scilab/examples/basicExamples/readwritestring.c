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

		SendScilabJob("B=['My' 'Great'; 'and fantastic string',';)'; 'strings' , 'matrix'];"); /* Assign */
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
			char variableToBeRetrievedB[] = "B";
			int nlr;

			int indx = 2, indy = 1;
			char *tmpStr = malloc(sizeof(char)*bsiz);
			nlr = bsiz;

			/* Retrieve the element at pos indx / indy */
			C2F(creadchains)(variableToBeRetrievedB, &indx, &indy, &nlr, tmpStr, (unsigned long)strlen(variableToBeRetrievedB), (unsigned long)strlen(tmpStr));

			printf("\n");
			printf("The retrieved string(%d,%d) from B: %s\n",indx, indy, tmpStr);
		}

		/* Load All elements of B  in a (char**) */
		{
			char variableToBeRetrievedB[] = "B";
			int m = 0, n = 0;
			int i = 0, j = 0;
			int x = 0, y = 0;
			char ** variableBfromScilab = NULL;

			int *lengthOfB = GetLengthStringMatrixByName(variableToBeRetrievedB, &m, &n);

			variableBfromScilab = (char **)malloc(sizeof(char*)* (m*n));
			for (i = 0; i < m * n; i++)
			{
				variableBfromScilab[i] = (char*)malloc(sizeof(char)*(lengthOfB[i]));
			}

			i = 0;
			for (x = 1; x <= m; x++)
			{
				for (y = 1; y <= n; y++)
				{
					int nlr = lengthOfB[i];
					char *tmpStr = NULL;
					tmpStr = variableBfromScilab[i];
					C2F(creadchains)(variableToBeRetrievedB, &x, &y, &nlr, tmpStr, (unsigned long)strlen(variableToBeRetrievedB), (unsigned long)strlen(tmpStr));
					i++;
				}
			}

			printf("\n");
			printf("Display from B formated (size: %d, %d) as in scilab :\n\n", m, n);
			for(j = 0 ; j < m ; j++)
			{
				for(i = 0 ; i < n ; i++)
				{
					/* Display the formated matrix with same scilab indice */
					printf("[%d,%d] = %s\n",j+1,i+1,variableBfromScilab[j* n + i]);
				}
			}
			printf("\n");

			/* free pointer variableBfromScilab */
			for (i = 0; i < m * n; i++)
			{
				free(variableBfromScilab[i]);
			}
			free(variableBfromScilab);
		}
		
	if ( TerminateScilab(NULL) == FALSE ) 
	{
		fprintf(stderr,"Error while calling TerminateScilab\n");
		return -2;
	}		
}
