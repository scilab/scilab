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
#include "optimizationfunctions.hxx"

extern "C"
{
#include "scioptimfunctions.h"
}

// optim
void costf(int *ind, int *n, double *x, double *f, double *g, int *ti, float *tr, double *td)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    if (opFunction == NULL)
    {
        throw ScilabError(_("An error occurred while getting OptimizationFunctions object.\n"));
    }

    opFunction->execCostf(ind, n, x, f, g, ti, tr, td);
}

// fsolve
void fct(int *n, double *x, double *v, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    if (opFunction == NULL)
    {
        throw ScilabError(_("An error occurred while getting OptimizationFunctions object.\n"));
    }

    opFunction->execFsolveFct(n, x, v, iflag);
}

void jac(int *n, double *x, double *v, double *jac, int *ldjac, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    if (opFunction == NULL)
    {
        throw ScilabError(_("An error occurred while getting OptimizationFunctions object.\n"));
    }

    if (*iflag == 1)
    {
        opFunction->execFsolveFct(n, x, v, iflag);
    }
    else
    {
        opFunction->execFsolveJac(n, x, v, jac, ldjac, iflag);
    }
}

// lsqrsolve
void lsqrfct(int *m, int *n, double *x, double *v, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    if (opFunction == NULL)
    {
        throw ScilabError(_("An error occurred while getting OptimizationFunctions object.\n"));
    }

    opFunction->execLsqrsolveFct(m, n, x, v, iflag);
}

void lsqrjac(int *m, int *n, double *x, double *v, double *jac, int *ldjac, int *iflag)
{
    OptimizationFunctions* opFunction = NULL;
    opFunction = Optimization::getOptimizationFunctions();

    if (opFunction == NULL)
    {
        throw ScilabError(_("An error occurred while getting OptimizationFunctions object.\n"));
    }

    if (*iflag == 1)
    {
        opFunction->execLsqrsolveFct(m, n, x, v, iflag);
    }
    else
    {
        opFunction->execLsqrsolveJac(m, n, x, v, jac, ldjac, iflag);
    }
}
