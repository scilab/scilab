/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
/*--------------------------------------------------------------------------*/
#include "Ex-odedc.h"
#include "api_scilab.h"
#include "elem_common.h"

static void brdmmul(double* a, int na, double* b, int nb, double* c, int nc, int l, int m, int n)
{
    int i  = 0;
    int j  = 0;
    int ib = 0;
    int ic = 0;
    int iOne = 1;

    for (j = 0; j < n; j++)
    {
        for (i = 0; i < l; i++)
        {
            c[ic + i] = C2F(ddot)(&m, a + i, &na, b + ib, &iOne);
        }

        ic += nc;
        ib += nb;
    }
}

void fexcd(int* jflag, int* nc, int* nd, double* t, double* y, double* ydp)
{
    // input variables jflag, nc, nd, t, y
    // jflag=0 or 1
    // nc = # of continuous states yc
    // nd = # of discrete states yd
    // t = time
    // y = state variable = [yc; yd]
    // output variable = ydp
    //
    // iflag=0  >> external routine must
    // load ydp(1:nc) with ydot=d/dt ( yc(t) )
    // derivative of continuous state
    // iflag=1  >> external routine must
    // load ydp(1:nd) with yplus= yd(t+)
    // update of discrete state
    // here y=[yc;yd] has dimension nc+nd=3+2
    //
    // Example:
    // 1/ call this fexcd:
    // y0c=[1;2;3]; y0d=[1;-1]; nd=2; t0=0; t=1:10;
    // y=odedc([y0c;y0d],nd,[5,0],t0,t,'fexcd')
    //
    // 2/ using dynamic link:
    // link('fexcd.o','fexcd')
    // y0c=[1;2;3]; y0d=[1;-1]; nd=2; t0=0; t=1:10;
    // y=odedc([y0c;y0d],nd,[5,0],t0,t,'fexcd')
    //
    // 3/ passing a parameter to fexcd routine:
    // y=odedc([y0c;y0d],nd,[5,0],t0,t,list('fexcd',param))
    // param can be retrieved in fexcd by:
    // param(1)=y(nc+nd+1) , param(2)=y(nc+nd+2) etc
    // with this syntax y is a nc+nd+np vector
    // where np=dimension of scilab variable param

    if (*jflag == 0)
    {
        ydp[0] = y[3];
        ydp[1] = y[4];
        ydp[2] = 0.;
    }
    else if (*jflag == 1)
    {
        ydp[0] = -y[3];
        ydp[1] = -y[4];
    }

}

// The odedc example in the manual:
void fcd(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp)
{
    // set dimensions of u,v,y
    double u = 0.;
    double v = 0.;
    double y = 0.;

    if (*jflag == 0)
    {
        // fc(t,xc,e(t)-hd(t,xd)
        // u=e(t)-hd(t,xd)
        finput(*t, &v);
        hd(*t, x + (*nc), &u);
        u = v - u;
        // u(2)=v(2)-u(2)  ....
        fc(*t, x, &u, xdp);
    }
    else if (*jflag == 1)
    {
        hc(*t, x, &y);
        fd(x + (*nc), &y, xdp);
    }
}

void fc(double t, double* xc, double* u, double* xdot)
{
    // A=[-10,2,3;4,-10,6;7,8,-10];B=[1;1;1]
    xdot[0] = -10 * xc[0] + 2 * xc[1] + 3 * xc[2] + u[0];
    xdot[1] = 4 * xc[0] - 10 * xc[1] + 6 * xc[2] + u[0];
    xdot[2] = 7 * xc[0] + 8 * xc[1] - 10 * xc[2] + u[0];
}

void hc(double t, double* x, double* y)
{
    y[0] = x[0] + x[1] + x[2];
}

void fd(double* xd, double* y, double* xp)
{
    // Ad=[0.5,1;0,0.05] Bd=[1;1]
    xp[0] = 0.5 * xd[0] + xd[1] + y[0];
    xp[1] = /*0 * xd[0] +*/ 0.05 * xd[1] + y[0];
}

void hd(double t, double* xd, double* u)
{
    u[0] = xd[0] + xd[1];
}

// The odedc example in the manual:
// It is assumed here that scilab variables
// A,B,C  and  Ad,Bd,Cd   exist.
void fcd1(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp)
{
    // iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
    // iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
    // set here dimensions of u,v,y
    double u = 0.;
    double v = 0.;
    double y = 0.;

    if (*jflag == 0)
    {
        // xcd=fc1(t,xc,u)
        // u=e(t)-hd1(t,xd)
        finput(*t, &v);
        hd1(*t, x + (*nc), &u);
        u = v - u;
        // u(2)=v(2)-u(2)  ....
        fc1(*t, x, &u, xdp);
    }
    else if (*jflag == 1)
    {
        // xcd=fd1(xd,y)     xd=[x(nc+1),x(nc+2),...]
        // y=hc1(t,xc)
        hc1(*t, x, &y);
        fd1(x + (*nc), &y, xdp);
    }
}

void fc1(double t, double* xc, double* u, double* xdot)
{
    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    int iOne  = 1;
    double dOne = 1.;
    double* pdblData = NULL;

    // xdot=A*xc+B*u
    // A and B real scilab matrices
    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iCols, pdblData);
    brdmmul(pdblData, iRows, xc, iRows, xdot, iRows, iRows, iRows, 1);
    free(pdblData);
    pdblData = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iCols, pdblData);
    C2F(dgemm)("n", "n", &iRows, &iOne, &iCols, &dOne, pdblData, &iRows, u, &iOne, &dOne, xdot, &iRows);
    free(pdblData);
    pdblData = NULL;
}

void hc1(double t, double* x, double* y)
{
    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    double* pdblData = NULL;

    // y=C*x
    readNamedMatrixOfDouble(pvApiCtx, "C", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "C", &iRows, &iCols, pdblData);

    brdmmul(pdblData, iRows, x, iRows, y, iRows, iRows, iCols, 1);
    free(pdblData);
    pdblData = NULL;
}

void fd1(double* xd, double* y, double* xp)
{
    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    int iOne  = 1;
    double dOne = 1.;
    double* pdblData = NULL;

    // xp=Ad*xd + Bd*y
    readNamedMatrixOfDouble(pvApiCtx, "Ad", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "Ad", &iRows, &iCols, pdblData);
    brdmmul(pdblData, iRows, xd, iRows, xp, iRows, iRows, iRows, 1);
    free(pdblData);
    pdblData = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "Bd", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "Bd", &iRows, &iCols, pdblData);
    C2F(dgemm)("n", "n", &iRows, &iOne, &iCols, &dOne, pdblData, &iRows, y, &iOne, &dOne, xp, &iRows);
    free(pdblData);
    pdblData = NULL;
}

void hd1(double t, double* xd, double* u)
{
    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    double* pdblData = NULL;

    // u=Cd*xd
    // y=C*x
    readNamedMatrixOfDouble(pvApiCtx, "Cd", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "Cd", &iRows, &iCols, pdblData);
    brdmmul(pdblData, iRows, xd, iRows, u, iRows, iRows, iCols, 1);
    free(pdblData);
    pdblData = NULL;
}

void finput(double t, double* v)
{
    v[0] = sin(3 * t);
}

// dot(x)=A x + B u with u= (0,1) step function
void phis(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp)
{
    // iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
    // iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
    if (*jflag == 0)
    {
        // dot(x)=A*x+B*xd
        sbrc(*t, x, xdp);
    }
    else if (*jflag == 1)
    {
        // xd=1-xd
        xdp[0] = 1. - x[*nc];
    }
}

// dot(x)=A x + B u with u= piecewise triangular function
void phit(int* jflag, int* nc, int* nd, double* t, double* x, double* xdp)
{
    // iflag=0 --> returns in xcd(1:nc) dot(xc=x(1:nc))
    // iflag=1 --> returns in xcd(1:nd) update(xd=x(nc+1:nc+nd))
    if (*jflag == 0)
    {
        // dot(x1c)=A*x1c+B*x2c
        // dot(x2c)=xd
        sbrc(*t, x, xdp);
        xdp[(*nc) - 1] = x[*nc];
    }
    else if (*jflag == 1)
    {
        // xd=-xd
        xdp[0] = -x[*nc];
    }
}

void sbrc(double t, double* x, double* xdot)
{
    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    int iOne  = 1;
    double dOne = 1.;
    double* pdblData = NULL;

    // xdot=A*x1+B*x2
    // A and B real scilab matrices
    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iCols, pdblData);
    brdmmul(pdblData, iRows, x, iRows, xdot, iRows, iRows, iRows, 1);
    free(pdblData);
    pdblData = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iCols, pdblData);
    C2F(dgemm)("n", "n", &iRows, &iOne, &iCols, &dOne, pdblData, &iRows, x + iRows, &iOne, &dOne, xdot, &iRows);
    free(pdblData);
    pdblData = NULL;
}





