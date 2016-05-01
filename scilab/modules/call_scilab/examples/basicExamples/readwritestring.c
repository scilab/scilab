/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre Ledru
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 * This example shows how to read / write a matrix of string from Scilab engine
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "api_scilab.h"
#include "call_scilab.h" /* Provide functions to call Scilab engine */

/*------------------------------------------------------------*/
int main(void)
{
    void* pvApiCtx = NULL;
#ifdef _MSC_VER
    if ( StartScilab(NULL, NULL, 0) == FALSE )
#else
    if ( StartScilab(getenv("SCI"), NULL, 0) == FALSE )
#endif
    {
        fprintf(stderr, "Error while calling StartScilab\n");
        return -1;
    }

    /******************************** WRITE ****************************/

    /*
    * Write a single string into Scilab
    * A="my Message";
    */
    {
        SciErr sciErr;

        int row = 2, col = 1; /* Size of the matrix */
        /* Declare the string */
        char **myMatrixOfString = (char**)malloc(sizeof(char*) * row * col);
        char variableName[] = "A";
        myMatrixOfString[0] = "my Message";
        myMatrixOfString[1] = "on two lines";

        /* Write it into Scilab's memory */
        sciErr = createNamedMatrixOfString(pvApiCtx, variableName, row, col, myMatrixOfString);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

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
        int row = 2, col = 2; /* Size of the matrix */
        /* Declare the string */
        char **myMatrixOfStringB = (char**)malloc(sizeof(char*) * row * col);
        char variableNameB[] = "B";
        SciErr sciErr;

        printf("\n");

        myMatrixOfStringB[0] = "My";
        myMatrixOfStringB[1] = "String";
        myMatrixOfStringB[2] = "Great";
        myMatrixOfStringB[3] = ";)";

        sciErr = createNamedMatrixOfString(pvApiCtx, variableNameB, row, col, myMatrixOfStringB);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        printf("\n");
        printf("Display from Scilab of B:\n");
        SendScilabJob("disp(B);"); /* Display B */
    }
    /******************************** READ ****************************/

    /* Load the previously set variable A */
    {

        char variableToBeRetrieved[] = "A";
        int iRows       = 0;
        int iCols       = 0;
        int i, j;
        int* piAddr     = NULL;
        int* piLen      = NULL;
        char** pstData  = NULL;
        SciErr sciErr;

        //first call to retrieve dimensions
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        //second call to retrieve length of each string
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }
        //third call to retrieve data
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, pstData);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        printf("\n");
        printf("Load and display of A:\n");
        for (j = 0 ; j < iCols ; j++)
        {
            for (i = 0 ; i < iRows ; i++)
            {
                /* Display the formated matrix with same scilab indice */
                printf("[%d,%d] = %s\n", j + 1, i + 1, pstData[j * iRows + i]);
            }
        }

        printf("\n");
        free(piLen);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);

    }


    /* Load an element of the previously set variable B */
    {

        char variableToBeRetrieved[] = "B";
        int iRows       = 0;
        int iCols       = 0;
        int i, j;
        int* piAddr     = NULL;
        int* piLen      = NULL;
        char** pstData  = NULL;
        SciErr sciErr;

        //first call to retrieve dimensions
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        //second call to retrieve length of each string
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }
        //third call to retrieve data
        sciErr = readNamedMatrixOfString(pvApiCtx, variableToBeRetrieved, &iRows, &iCols, piLen, pstData);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        printf("\n");
        printf("Load and display of B:\n");
        for (j = 0 ; j < iCols ; j++)
        {
            for (i = 0 ; i < iRows ; i++)
            {
                /* Display the formated matrix with same scilab indice */
                printf("[%d,%d] = %s\n", j + 1, i + 1, pstData[j * iRows + i]);
            }
        }

        printf("\n");
        free(piLen);
        for (i = 0 ; i < iRows * iCols ; i++)
        {
            free(pstData[i]);
        }
        free(pstData);

    }

    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;
}
