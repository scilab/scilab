/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include <cmath>

#include "differential_equations_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "callable.hxx"
#include "differentialequationfunctions.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "matrix_division.h"
#include "vfinite.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_intg(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    double pdA    = 0;
    double pdB    = 0;
    double pdEpsR = 1.0e-8;
    double pdEpsA = 1.0e-14;

    double result = 0;
    double abserr = 0;

    int iOne = 1;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 3 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "intg", 3);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "intg", 3);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // A
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "intg", 1);
        return types::Function::Error;
    }

    types::Double* pDblA = in[0]->getAs<types::Double>();

    if (pDblA->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "intg", 1);
        return types::Function::Error;
    }

    pdA = pDblA->get(0);

    if (ISNAN(pdA) || C2F(vfinite)(&iOne , &pdA) == false)
    {
        Scierror(264, _("%s: Wrong type for input argument #%d: Must not contain NaN or Inf.\n"), "intg", 1);
        return types::Function::Error;
    }

    // B
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "intg", 2);
        return types::Function::Error;
    }

    types::Double* pDblB = in[1]->getAs<types::Double>();

    if (pDblB->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "intg", 2);
        return types::Function::Error;
    }

    pdB = pDblB->get(0);

    if (ISNAN(pdB) || C2F(vfinite)(&iOne , &pdB) == false)
    {
        Scierror(264, _("%s: Wrong type for input argument #%d: Must not contain NaN or Inf.\n"), "intg", 1);
        return types::Function::Error;
    }

    // function
    DifferentialEquationFunctions deFunctionsManager(L"intg");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

    if (in[2]->isCallable())
    {
        types::Callable* pCall = in[2]->getAs<types::Callable>();
        deFunctionsManager.setFFunction(pCall);

        // check function
        double t = 1;
        double ret = intg_f(&t);
        /* if (ret == 0)
        {
            Scierror(50, _("%s: Argument #%d: Variable returned by scilab argument function is incorrect.\n"), "intg", 3);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }*/
    }
    else if (in[2]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[2]->getAs<types::String>();
        bOK = deFunctionsManager.setFFunction(pStr);

        if (bOK == false)
        {
            char* pst = wide_string_to_UTF8(pStr->get(0));
            Scierror(50, _("%s: Subroutine not found: %s\n"), "intg", pst);
            FREE(pst);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if (in[2]->isList())
    {
        types::List* pList = in[2]->getAs<types::List>();

        if (pList->getSize() == 0)
        {
            Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "intg", 3, "(string empty)");
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (pList->get(0)->isCallable())
        {
            deFunctionsManager.setFFunction(pList->get(0)->getAs<types::Callable>());
            for (int iter = 1; iter < pList->getSize(); iter++)
            {
                deFunctionsManager.setFArgs(pList->get(iter)->getAs<types::InternalType>());
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a Scilab function.\n"), "intg", 3);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "intg", 3);
        DifferentialEquation::removeDifferentialEquationFunctions();
        return types::Function::Error;
    }

    if (in.size() > 3)
    {
        if (in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "intg", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        types::Double* pDblEpsR = in[3]->getAs<types::Double>();

        if (pDblEpsR->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "intg", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        pdEpsR = pDblEpsR->get(0);
    }

    if (in.size() == 5)
    {
        if (in[4]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "intg", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        types::Double* pDblEpsA = in[4]->getAs<types::Double>();

        if (pDblEpsA->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "intg", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
        pdEpsA = pDblEpsA->get(0);
    }

    // *** Create working table. ***
    int limit   = 750;
    int neval   = 0;
    int last    = 0;
    int lenw    = 4 * limit;

    double* dwork   = (double*)MALLOC(lenw * sizeof(double));
    int* iwork      = (int*)MALLOC(limit * sizeof(int));

    double epsabs   = fabs(pdEpsA);
    double epsrel   = fabs(pdEpsR);

    // *** Perform operation. ***
    int ier = 0;
    try
    {
        C2F(dqags)(intg_f, &pdA, &pdB, &epsabs, &epsrel,
                   &result, &abserr, &neval, &ier,
                   &limit, &lenw, &last, iwork, dwork);
    }
    catch (ast::InternalError &ie)
    {
        os << ie.GetErrorMessage();
        bCatch = true;
    }

    FREE(dwork);
    FREE(iwork);
    DifferentialEquation::removeDifferentialEquationFunctions();

    if (bCatch)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "intg", "dqags");
        os << szError;
        throw ast::InternalError(os.str());
    }

    if (ier)
    {
        char* msg = NULL;
        switch (ier)
        {
            case 1 :
            {
                msg = _("%s: Maximum number of subdivisions achieved. Splitting the interval might help.\n");
                break;
            }
            case 2 :
            {
                msg = _("%s: Round-off error detected, the requested tolerance (or default) cannot be achieved. Try using bigger tolerances.\n");
                break;
            }
            case 3 :
            {
                msg = _("%s: Bad integrand behavior occurs at some points of the integration interval.\n");
                break;
            }
            case 4 :
            {
                msg = _("%s: Convergence problem, round-off error detected. Try using bigger tolerances.\n");
                break;
            }
            case 5 :
            {
                msg = _("%s: The integral is probably divergent, or slowly convergent.\n");
                break;
            }
            case 6 :
            {
                msg = _("%s: Invalid input, absolute tolerance <= 0 and relative tolerance < 2.e-14.\n");
                break;
            }
            default :
                msg = _("%s: Convergence problem...\n");
        }

        if (_iRetCount == 3)
        {
            if (getWarningMode())
            {
                sciprint(msg, "intg: Warning");
            }
        }
        else
        {
            Scierror(999, msg, "intg: Error");
            return types::Function::Error;
        }
    }

    // *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(result);
    out.push_back(pDblOut);

    if (_iRetCount > 1)
    {
        out.push_back(new types::Double(abserr));
    }

    if (_iRetCount == 3)
    {
        out.push_back(new types::Double((double)ier));
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

