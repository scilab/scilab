/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2009 - Sylvestre LEDRU
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 * This example shows how to read / write a matrix of complex from Scilab
 * engine
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
     * Write a line complex matrix into Scilab
     * A=[ 1+%i 3 3+7*%i 2-2*%i ];
     */
    {
        /* Declare the complex matrix */
        double A[] = {1, 3, 3, 2};
        double A_img[] = {1, 0, 7, -2};

        int rowA = 1, colA = 4; /* Size of the complex matrix
							* (note that colA = sizeof(A)/2
							*/
        char variableName[] = "A";
        SciErr sciErr;

        /* Write it into Scilab's memory */
        sciErr = createNamedComplexMatrixOfDouble(pvApiCtx, variableName, rowA, colA, A, A_img);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }
        printf("Display from Scilab of A:\n");
        SendScilabJob("disp(A);"); /* Display A */
    }

    /*
    * Write a matrix into Scilab
    * B=[1+%i 4-%i 2+1/2*%i 3;
    *    3 9 8+42*%i 2 ]
    * Note that it is done column by column
    */
    {

        double B[] = {1, 3, 4, 9, 2, 8, 3, 2};
        double B_img[] = {1, 0.233, -1, -0.2, 0.5, 42, -23, 123}; /* Declare the matrix */

        int rowB = 2, colB = 4; /* Size of the matrix */
        char variableNameB[] = "B";
        SciErr sciErr;

        /* Write it into Scilab's memory */
        sciErr = createNamedComplexMatrixOfDouble(pvApiCtx, variableNameB, rowB, colB, B, B_img);
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
        int i = 0, j = 0;
        double *matrixOfComplex = NULL;
        double *matrixOfComplex_img = NULL;

        char variableToBeRetrieved[] = "A";
        SciErr sciErr;

        /* First, retrieve the size of the matrix */
        sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }

        /* Alloc the memory */
        matrixOfComplex = (double*)malloc((rowA_ * colA_ * 2) * sizeof(double));
        matrixOfComplex_img = matrixOfComplex + (rowA_ * colA_);

        /* Load the matrix */
        sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableToBeRetrieved, &rowA_, &colA_, matrixOfComplex, matrixOfComplex_img);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        printf("\n");
        printf("Display raw A (size: %d, %d):\n", rowA_, colA_);
        for (i = 0; i < rowA_ * colA_ * 2; i++) /* *2 is because complex part is store
										 * at the end
										 */
        {
            fprintf(stdout, "A[%d] = %5.2f\n", i, matrixOfComplex[i]);
        }

        printf("\n");
        printf("Display formated A (size: %d, %d):\n", rowA_, colA_);
        for (i = 0; i < rowA_ * colA_; i++)
        {
            fprintf(stdout, "%5.2f + %5.2f.i ", matrixOfComplex[i], matrixOfComplex[i + colA_]);
        }
        printf("\n");

        if (matrixOfComplex)
        {
            free(matrixOfComplex);
            matrixOfComplex = NULL;
        }
    }

    /* Load the previously set variable B */
    {
        int rowB_ = 0, colB_ = 0, lp_ = 0;
        int i = 0, j = 0;

        double *matrixOfComplexB = NULL;
        double *matrixOfComplexB_img = NULL;
        char variableToBeRetrievedB[] = "B";
        SciErr sciErr;
        /* First, retrieve the size of the matrix */

        sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, NULL, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        /* Alloc the memory */
        matrixOfComplexB = (double*)malloc((rowB_ * colB_) * sizeof(double));
        matrixOfComplexB_img = (double*)malloc((rowB_ * colB_) * sizeof(double));

        /* Load the matrix */
        sciErr = readNamedComplexMatrixOfDouble(pvApiCtx, variableToBeRetrievedB, &rowB_, &colB_, matrixOfComplexB, matrixOfComplexB_img);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
        }


        printf("\n");
        printf("Display from B raw - real part (size: %d, %d):\n", rowB_, colB_);
        for (i = 0; i < rowB_ * colB_; i++) /* *2 is because complex part is store
										  * at the end
										  */
        {
            /* Display the raw matrix */
            fprintf(stdout, "B[%d] = %5.2f\n", i, matrixOfComplexB[i]);
        }
        printf("Display from B raw - imaginary part (size: %d, %d):\n", rowB_, colB_);
        for (i = 0; i < rowB_ * colB_; i++) /* *2 is because complex part is store
										  * at the end
										  */
        {
            /* Display the raw matrix */
            fprintf(stdout, "B[%d] = %5.2f\n", i, matrixOfComplexB_img[i]);
        }

        printf("\n");
        printf("Display from B formated (size: %d, %d):\n", rowB_, colB_);
        for (j = 0 ; j < rowB_ ; j++)
        {
            for (i = 0 ; i < colB_ ; i++)
            {
                /* Display the formated matrix ... the way the user
                 * expect */
                printf("%5.2f + %5.2f.i  ", matrixOfComplexB[i * rowB_ + j], matrixOfComplexB_img[i * rowB_ + j]);
            }
            printf("\n"); /* New row of the matrix */
        }

        if (matrixOfComplexB)
        {
            free(matrixOfComplexB);
            matrixOfComplexB = NULL;
        }
        if (matrixOfComplexB_img)
        {
            free(matrixOfComplexB_img);
            matrixOfComplexB_img = NULL;
        }
    }

    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;
}
