/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 * This example shows how to read / write a matrix of boolean from Scilab
 * engine
 *
 * This example works from Scilab 5.2
 * but the previous way of doing is also described as comment
 */
#include <math.h>
#include <stdio.h>
#include <stdlib.h> /* malloc */
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
     * Write a line matrix of boolean into Scilab
     * A=[ T F F T ];
     */
    {
        int A[] = {1, 0, 0, 1}; /* Declare the matrix */
        /* NOTE that it is an array of int and not an array of double */
        int rowA = 1, colA = 4; /* Size of the matrix */
        char variableName[] = "A";
        SciErr sciErr;

        /*
         Write it into Scilab's memory
         */
        sciErr = createNamedMatrixOfBoolean(pvApiCtx, variableName, rowA, colA, A);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

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
        int B[] = {0, 0, 0, 0, 1, 0, 0, 1}; /* Declare the matrix */
        int rowB = 2, colB = 4; /* Size of the matrix */
        char variableNameB[] = "B";
        SciErr sciErr;

        /* Write it into Scilab's memory */
        sciErr = createNamedMatrixOfBoolean(pvApiCtx, variableNameB, rowB, colB, B);
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
        int rowA_ = 0, colA_ = 0, lp = 0;
        int i = 0;
        int *matrixOfBoolean = NULL; /* Int instead of double */

        char variableToBeRetrieved[] = "A";
        SciErr sciErr;

        /* First, retrieve the size of the matrix */
        sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        /* Alloc the memory */
        matrixOfBoolean = (int*)malloc((rowA_ * colA_) * sizeof(int));

        /* Load the matrix */
        sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, matrixOfBoolean);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        printf("\n");
        printf("Display from A (size: %d, %d):\n", rowA_, colA_);
        for (i = 0; i < rowA_ * colA_; i++)
        {
            fprintf(stdout, "A[%d] = %d\n", i, matrixOfBoolean[i]);
        }

        if (matrixOfBoolean)
        {
            free(matrixOfBoolean);
            matrixOfBoolean = NULL;
        }
    }

    /* Load the previously set variable B */
    {
        int rowB_ = 0, colB_ = 0, lp_ = 0;
        int i = 0, j = 0;
        int *matrixOfBooleanB = NULL; /* Int instead of double */

        char variableToBeRetrievedB[] = "B";
        SciErr sciErr;

        /* First, retrieve the size of the matrix */
        sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, NULL);

        /* Alloc the memory */
        matrixOfBooleanB = (int*)malloc((rowB_ * colB_) * sizeof(int));

        /* Load the matrix */
        sciErr = readNamedMatrixOfBoolean(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, matrixOfBooleanB);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        printf("\n");
        printf("Display from B raw (size: %d, %d):\n", rowB_, colB_);
        for (i = 0; i < rowB_ * colB_; i++)
        {
            /* Display the raw matrix */
            fprintf(stdout, "B[%d] = %d\n", i, matrixOfBooleanB[i]);
        }

        printf("\n");
        printf("Display from B formated (size: %d, %d):\n", rowB_, colB_);
        for (j = 0 ; j < rowB_ ; j++)
        {
            for (i = 0 ; i < colB_ ; i++)
            {
                /* Display the formated matrix ... the way the user
                * expect */
                printf("%d ", matrixOfBooleanB[i * rowB_ + j]);
            }
            printf("\n"); /* New row of the matrix */
        }

        if (matrixOfBooleanB)
        {
            free(matrixOfBooleanB);
            matrixOfBooleanB = NULL;
        }
    }


    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;
}
