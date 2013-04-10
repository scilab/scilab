/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "differentialequationfunctions.hxx"

extern "C"
{
#include "scifunctions.h"
}

// ode / odedc
int ode_f(int* n, double* t, double* y, double* ydot)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execOdeF(n, t, y, ydot);
}

int ode_jac(int *n, double *t, double *y, int *ml, int *mu, double *J, int *nrpd)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execFunctionJac(n, t, y, ml, mu, J, nrpd);
}

int ode_g(int* n, double* t, double* y, int* ng, double* gout)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execFunctionG(n, t, y, ng, gout);
}

// intg
double intg_f(double* x)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execIntgF(x);
}

// int2d
double int2d_f(double* x, double* y)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execInt2dF(x, y);
}

// int3d
double int3d_f(double* x, int* numfun, double* funvls)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }

    return deFunction->execInt3dF(x, numfun, funvls);
}

// bvode
int bvode_gsub(int* i, double* z, double* g)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execBvodeGsub(i, z, g);
}
int bvode_dgsub(int* i, double* z, double* g)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execBvodeDgsub(i, z, g);
}
int bvode_fsub(double* x, double* z, double* d)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execBvodeFsub(x, z, d);
}
int bvode_dfsub(double* x, double* z, double* d)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execBvodeDfsub(x, z, d);
}
int bvode_guess(double* x, double* z, double* d)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execBvodeGuess(x, z, d);
}

// impl
int impl_f(int* neq, double* t, double* y, double* s, double* r, int* ires)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execImplF(neq, t, y, s, r, ires);
}
int impl_g(int* neq, double* t, double* y, double* ml, double* mu, double* p, int* nrowp)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execImplG(neq, t, y, ml, mu, p, nrowp);
}
int impl_jac(int* neq, double* t, double* y, double* s, double* ml, double* mu, double* p, int* nrowp)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execImplJac(neq, t, y, s, ml, mu, p, nrowp);
}

// dassl, dasrt, daskr
int dassl_f(double* t, double* y, double* ydot, double* delta, int* ires, double* rpar, int* ipar)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execDasslF(t, y, ydot, delta, ires, rpar, ipar);
}
int dassl_jac(double* t, double* y, double* ydot, double* pd, double* cj, double* rpar, int* ipar)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execDasslJac(t, y, ydot, pd, cj, rpar, ipar);
}

// dasrt
int dasrt_g(int* ny, double* t, double* y, int* ng, double* gout, double* rpar, int* ipar)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execDasrtG(ny, t, y, ng, gout, rpar, ipar);
}

//daskr
int daskr_psol(int* neq, double* t, double* y, double* ydot, double* savr, double* wk,
               double* cj, double* wght, double* wp, int* iwp, double* b, double* eplin,
               int* ier, double* rpar, int* ipar)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execDaskrPsol(neq, t, y, ydot, savr, wk, cj, wght, wp, iwp,
                                     b, eplin, ier, rpar, ipar);
}
int daskr_pjac(double* res, int* ires, int* neq, double* t, double* y, double* ydot,
               double* rewt, double* savr, double* wk, double* h, double* cj,
               double* wp, int* iwp, int* ier, double* rpar, int* ipar)
{
    DifferentialEquationFunctions* deFunction = NULL;
    deFunction = DifferentialEquation::getDifferentialEquationFunctions();

    if (deFunction == NULL)
    {
        return 0;
    }
    return deFunction->execDaskrPjac(res, ires, neq, t, y, ydot, rewt, savr, wk, h, cj,
                                     wp, iwp, ier, rpar, ipar);
}
