/*--------------------------------------------------------------------------*/
/* Example only for Windows */
/*--------------------------------------------------------------------------*/
#pragma comment(lib, "../../../../../../bin/call_scilab.lib")
#pragma comment(lib, "../../../../../../bin/api_scilab.lib")
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "call_scilab.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
/* See SCI/modules/core/includes/call_scilab.h */
/* See SCI/modules/core/includes/api_scilab.h */
/*--------------------------------------------------------------------------*/
static int example1(void)
{
    SciErr sciErr;

    static double A[] = {1, 2, 3, 4};
    int mA = 2, nA = 2;

    static double b[] = {4, 5};
    int mb = 2, nb = 1;

    /* Create Scilab matrices A and b */
    sciErr = createNamedMatrixOfDouble(NULL, "A", mA, nA, A);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    sciErr = createNamedMatrixOfDouble(NULL, "b", mb, nb, b);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

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
        int m = 0, n = 0;
        int i = 0;

        /* Get m and n dimensions of x */
        sciErr = readNamedMatrixOfDouble(NULL, "x", &m, &n, NULL);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return -1;
        }

        cxtmp = (double*)malloc((m * n) * sizeof(double));
        sciErr = readNamedMatrixOfDouble(NULL, "x", &m, &n, cxtmp);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return -1;
        }

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
/*--------------------------------------------------------------------------*/
static int example2(void)
{
    SendScilabJob("plot3d();");
    printf("\nClose Graphical Windows to close this example.\n");
    while (ScilabHaveAGraph());
    return 1;
}
/*--------------------------------------------------------------------------*/
static int example3(void)
{
    int code = 0;

    char **JOBS = NULL;
    const int SizeJOBS = 6;
    int i = 0;

    JOBS = (char**)malloc(sizeof(char**) * SizeJOBS);

    for (i = 0; i < SizeJOBS; i++)
    {
        JOBS[i] = (char*)malloc(sizeof(char*) * 1024);
    }

    strcpy(JOBS[0], "A=1 ..");
    strcpy(JOBS[1], "+3;");
    strcpy(JOBS[2], "B = 8;");
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
    return 1;
}
/*--------------------------------------------------------------------------*/
int main(void)
/* int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR szCmdLine, int iCmdShow) */
{
    if ( StartScilab(NULL, NULL, 0) == FALSE )
    {
        printf("Error : StartScilab\n");
        return 0;
    }

    printf("\nexample 1\n");
    example1();
    system("pause");
    //printf("\nexample 2\n");
    example2();
    system("pause");
    //printf("\nexample 3\n");
    example3();
    system("pause");

    if ( TerminateScilab(NULL) == FALSE )
    {
        printf("Error : TerminateScilab\n");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
