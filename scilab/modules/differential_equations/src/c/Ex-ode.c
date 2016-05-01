/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
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
#include "Ex-ode.h"
#include "api_scilab.h"
#include "elem_common.h"

//-----------------------------------------------------------------
//     Examples of argument functions for scilab function ode
//     The function provided here must be listed
//     in the file Flist in order to be used with scilab
//     fydot_list : contains the rhs function
//     fjac_list   : contains the jacobian (if you want to give it)
//                   ( example jex -> jacobian for fex)
//     Examples :
//       fex, jex : first example
//       fex2     : uses creadmat (or matz) to get parameters
//       fex3     : uses matptr to get parameters
//       fexab    : a more complex example with matpr
//-----------------------------------------------------------------

extern C2F(dmmul)(double*, int*, double*, int*, double*, int*, int*, int*, int*);

void fex(int* neq, double* t, double* y, double* ydot)
{
    //        input variables neq, t, y
    //        neq = dimension of state vector y
    //        t = time
    //        y = state variable
    //        output variable = ydot
    //
    //        argument routine must
    //        load ydot(1) with d/dt ( y(1)(t) )
    //          ydot(2) with d/dt ( y(2)(t) )
    //          ...
    //        i.e. ydot vector of derivative of state y
    //
    //        Example:
    //        1/ call this fex routine:
    //        ode([1;0;0],0,[0.4,4],'fex')
    //
    //        2/ May use dynamic link:
    //        link('fex.o','fex')
    //        ode([1;0;0],0,[0.4,4],'fex')
    //
    //        3/ passing a parameter to fex routine:
    //        link('fex.o','fex')
    //        y=ode([1;0;0],0,[0.4,4],list('fex',param))
    //        param can be retrieved in fex by:
    //        param(1)=y(neq+1) , param(2)=y(neq+2) etc
    //        with this syntax y is a nc+nd+np vector
    //        where np=dimension of scilab variable param

    ydot[0] = -0.0400 * y[0] + 1e4 * y[1] * y[2];
    ydot[2] = 3e7 * y[1] * y[1];
    ydot[1] = -ydot[0] - ydot[2];
}

void jex(int* neq, double* t, double* y, double* ml, double* mu, double* pd, int* nrpd)
{
    //        fex example continued : we provide here
    //        the jacobian function
    //        jacobian routine jex
    //        scilab ode
    //        ode([1;0;0],0,[0.4,4],'fex','jex')
    pd[0] = -0.040;
    pd[3] = 1e4 * y[2];
    pd[6] = 1e4 * y[1];
    pd[1] = 0.040;
    pd[7] = -pd[6];
    pd[5] = 6e7 * y[1];
    pd[4] = -pd[3] - pd[5];
}


void fex2(int* neq, double* t, double* y, double* ydot)
{
    //     exemple with a call to creadmat routine
    //     param must be defined as a scilab variable

    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    double* pdblData = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "param", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "param", &iRows, &iCols, pdblData);

    ydot[0] = pdblData[0] * y[0] + pdblData[1] * y[1] * y[2];
    ydot[2] = pdblData[2] * y[1] * y[1];
    ydot[1] = -ydot[0] - ydot[2];

    free(pdblData);
}

void fex3(int* neq, double* t, double* y, double* ydot)
{
    //     same example with call to matptr
    //     param must be defined as a scilab variable

    // pvApiCtx not used when we get a named variable
    void* pvApiCtx = NULL;
    int iRows = 0;
    int iCols = 0;
    double* pdblData = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "param", &iRows, &iCols, pdblData);
    pdblData = (double*)malloc(iRows * iCols * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "param", &iRows, &iCols, pdblData);

    if (iRows == -1)
    {
        return;
    }

    //     param entries are in stk(lp),stk(lp+1),stk(lp+2)
    //     m,n = dimensions of param = 3,1 (or 1,3 if row v.)
    //     (note that vector param not used in this example)
    ydot[0] = pdblData[0] * y[0] + pdblData[1] * y[1] * y[2];
    ydot[2] = pdblData[2] * y[1] * y[1];
    ydot[1] = -ydot[0] - ydot[2];

    free(pdblData);
}

void fexab(int* neq, double* t, double* x, double* xdot)
{
    //    Another example
    //     xdot=A*xc+B*u
    //     A and B real scilab matrices
    //     u=u(t,x)
    //
    //     [ User may extract this fexab.f routine
    //       and -->link('fexab.o','fexab') ]
    //
    //     -->A=..., B=...
    //     -->ode(x0,t0,t,'fexab')
    //
    //     or customize this one and re-make scilab
    //
    //     input variables neq, t, x
    //     neq = dimension of state vector x
    //     t = time
    //     x = state variable
    //     output variable = xdot
    //
    //     routine must
    //     load xdot(1) with d/dt ( x(1)(t) )
    //          xdot(2) with d/dt ( x(2)(t) )
    //          ...
    //     i.e. xdot vector of derivative of state x

    void* pvApiCtx = NULL;
    char cN = 'n';
    int iOne = 1;
    int iRows = 0;
    int iColsA = 0;
    int iColsB = 0;
    double pdblOne = 1.0;
    double pdblU = 0;
    double* pdblDataA = NULL;
    double* pdblDataB = NULL;

    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iColsA, pdblDataA);
    pdblDataA = (double*)malloc(iRows * iColsA * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "A", &iRows, &iColsA, pdblDataA);
    C2F(dmmul)(pdblDataA, &iRows, x, &iRows, xdot, &iRows, &iRows, &iRows, &iOne);

    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iColsB, pdblDataB);
    pdblDataB = (double*)malloc(iRows * iColsB * sizeof(double));
    readNamedMatrixOfDouble(pvApiCtx, "B", &iRows, &iColsB, pdblDataB);

    pdblU = sin(3 * t[0]);
    C2F(dgemm)(&cN, &cN, &iRows, &iOne, &iColsB, &pdblOne, pdblDataB, &iRows, &pdblU, &iOne, &pdblOne, xdot, &iRows);

    free(pdblDataA);
    free(pdblDataB);
}
