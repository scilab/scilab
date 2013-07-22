#include <stdio.h>
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
#ifndef SCI
#define SCI "SCI=../../.."
#endif
#define TRUE 1
#define FALSE 0

/*--------------------------------------------------------------------------*/
/* See routines/system/call_scilab.h */
extern int StartScilab(char *SCIpath, char *ScilabStartup, int *Stacksize);
extern int TerminateScilab(char *ScilabQuit);
extern int SendScilabJob(char *job);
/*--------------------------------------------------------------------------*/
double f(double x)
{
    return x * x + 1;
}
/*--------------------------------------------------------------------------*/
static int my_plot()
{
    int m, n, lx, ly, i;
    /* Scilab is called to build the abscissae vector and
       to allocate the ordinates */

    SendScilabJob("x=1:0.1:10;y=x;");
    GetMatrixptr("x", &m, &n, &lx);
    GetMatrixptr("y", &m, &n, &ly);
    /*
     * y(i)=f(x(i))
     */
    for ( i = 0; i < m * n ; i++)
    {
        *stk(ly + i) = f(*stk(lx + i));
    }
    /*
     * plot(x,y);
     */
    SendScilabJob("plot(y,x);");
}
/*--------------------------------------------------------------------------*/
static int my_job()
{
    static char job[256];
    /* storing a matrix inside scilab */
    static double A[] = {1, 2, 3, 4};
    int mA = 2, nA = 2;
    static double b[] = {4, 5};
    int mb = 2, nb = 1;
    int m, n, lp, i;
    WriteMatrix("A", &mA, &nA, A); /* if failed make my_job return O; */
    WriteMatrix("b", &mb, &nb, b);

    if ( SendScilabJob("A,b,x=0\\b;") == 27)
    {
        fprintf(stdout, "division by zero...\n");
    }
    else
    {
        GetMatrixptr("x", &m, &n, &lp);
        for ( i = 0 ; i < m * n ; i++)
        {
            fprintf(stdout, "x[%d] = %5.2f\n", i, *stk(i + lp));
        }
    }

    if ( SendScilabJob("A,b,x=A\\b;") != 0)
    {
        fprintf(stdout, "Error occurred during scilab execution\n");
    }
    else
    {
        GetMatrixptr("x", &m, &n, &lp);
        for ( i = 0 ; i < m * n ; i++)
        {
            fprintf(stdout, "x[%d] = %5.2f\n", i, *stk(i + lp));
        }
    }
}
/*--------------------------------------------------------------------------*/
/* I want to integrate the previous equation */
int my_ode_job()
{
    double x[] = {1, 0, 0} ;
    int mx = 3, nx = 1;
    double time[] = {0.4, 4};
    int mt = 1, nt = 2;
    fprintf(stdout, "linking \n");
    SendScilabJob("ilib_for_link('odeex','my_ode.c',[],'c');");
    fprintf(stdout, "link done  \n");
    SendScilabJob("exec('loader.sce');link('show')");
    WriteMatrix("x", &mx, &nx, x);
    WriteMatrix("time", &mt, &nt, time);
    /* scilab is called to solve the ODE */
    SendScilabJob("y=ode(x,0,time,''mon_ode''),");
}
/*--------------------------------------------------------------------------*/
#if _MSC_VER
int main(void)
#else
int MAIN__(void)
#endif
{
    if ( StartScilab(NULL, NULL, NULL) == FALSE )
    {
        printf("Error : StartScilab \n");
    }
    my_ode_job();
    my_job();
    my_plot() ;
    cc_test();
    if ( TerminateScilab(NULL) == FALSE )
    {
        printf("Error : TerminateScilab \n");
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
