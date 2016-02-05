/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) INRIA - 2005 - Allan Cornet
 *
 * This file is released under the 3-clause BSD license. See COPYING-BSD.
 *
 */
#include <math.h>
#include <stdio.h>
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <unistd.h>
#include <string.h>

#include "api_scilab.h"
#include "call_scilab.h" /* Provide functions to call Scilab engine */

/**
 *
 * Initialisation of Scilab
 */
/*#ifndef SCI
#define SCI "../.."
#endif */
/*------------------------------------------------------------*/
#define TRUE 1
#define FALSE 0
/*------------------------------------------------------------*/
/* See SCI/modules/core/includes/call_scilab.h */
/*------------------------------------------------------------*/
static int first_example(void)
{
    void* pvApiCtx = NULL;
    static double A[] = {1, 2, 3, 4};
    int mA = 2, nA = 2;
    static double b[] = {4, 5};
    int mb = 2, nb = 1;
    printf("\nExample 1:\n");
    printf("Some simple computations\n");

    /* Create Scilab matrices A and b */
    createNamedMatrixOfDouble(pvApiCtx, "A", mA, mA, A);
    createNamedMatrixOfDouble(pvApiCtx, "b", mb, nb, b);

    SendScilabJob("disp('A=');");
    SendScilabJob("disp(A);");
    SendScilabJob("disp('b=');");
    SendScilabJob("disp(b);");
    SendScilabJob("disp('x=A\\b');");

    if ( SendScilabJob("A,b,x=A\\b;") != 0)
    {
        fprintf(stdout, "Error occurred during scilab execution (SendScilabJob)\n");
    }
    else
    {
        double *cxtmp = NULL;
        int m, n, lp, i;

        /* Get m and n */
        getNamedVarDimension(pvApiCtx, "x", &m, &n);

        cxtmp = (double*)malloc((m * n) * sizeof(double));

        readNamedMatrixOfDouble(pvApiCtx, "x", &m, &n, cxtmp);

        for (i = 0; i < m * n; i++)
        {
            fprintf(stdout, "x[%d] = %5.2f\n", i, cxtmp[i]);
        }

        if (cxtmp)
        {
            free(cxtmp);
            cxtmp = NULL;
        }
    }
    return 0;
}
/*------------------------------------------------------------*/
static int second_example(void)
{
    printf("\nExample 2:\n");
    printf("Call graphics\n");

    SendScilabJob("plot3d();");

    printf("\nClose Graphical Windows to close this example.\n");
    while ( ScilabHaveAGraph() )
    {
        ScilabDoOneEvent();
        sleep(1);
    }
    return 1;
}
/*------------------------------------------------------------*/
static int third_example(void)
{
    int code = 0;
    char **JOBS = NULL;
    const int SizeJOBS = 6;
    int i = 0;
    printf("\nExample 3:\n");
    printf("Send many jobs.\n");
    JOBS = (char**)malloc(sizeof(char**) * SizeJOBS);

    for (i = 0; i < SizeJOBS; i++)
    {
        JOBS[i] = (char*)malloc(sizeof(char*) * 1024);
    }

    strcpy(JOBS[0], "A=1 ..");
    strcpy(JOBS[1], "+3;");
    strcpy(JOBS[2], "B = 8;");
    /* strcpy(JOBS[2],"b = V_NOT_EXIST;"); */
    strcpy(JOBS[3], "+3;");
    strcpy(JOBS[4], "disp('C=');");
    strcpy(JOBS[5], "C=A+B;disp(C);"); /* C = 12 */

    code = SendScilabJobs(JOBS, SizeJOBS);

    if (code)
    {
        char lastjob[4096]; // bsiz in scilab 4096 max
        if (GetLastJob(lastjob, 4096))
        {
            printf("Error %s\n", lastjob);
        }
    }

    for (i = 0; i < SizeJOBS; i++)
    {
        if (JOBS[i])
        {
            free(JOBS[i]);
            JOBS[i] = NULL;
        }
    }
    return 0;
}

/*------------------------------------------------------------*/
int main(void)
{
#ifdef _MSC_VER
    if ( StartScilab(NULL, NULL, NULL) == FALSE )
#else
    if ( StartScilab(getenv("SCI"), NULL, NULL) == FALSE )
#endif
    {
        fprintf(stderr, "Error while calling StartScilab\n");
        return -1;
    }

    first_example();

    second_example() ;

    third_example() ;

    if ( TerminateScilab(NULL) == FALSE )
    {
        fprintf(stderr, "Error while calling TerminateScilab\n");
        return -2;
    }
    return 0;
}
/*------------------------------------------------------------*/
