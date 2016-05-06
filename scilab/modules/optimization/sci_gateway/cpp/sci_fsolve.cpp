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

#include "optimization_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "list.hxx"
#include "optimizationfunctions.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "scioptimfunctions.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_fsolve(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    OptimizationFunctions* opFunctionsManager = NULL;

    types::Double* pDblX    = NULL;
    types::Double* pDblV    = NULL;
    types::Double* pDblTol  = NULL;

    int iSizeX      = 0;
    int iWorkSize   = 0;
    int iInfo       = 0;

    double* pdblWork = NULL;
    double* pdblJac  = NULL;

    double dTol = 1.0e-10;
    bool bJac   = false;

    if (in.size() < 2 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "fsolve", 2, 4);
        return types::Function::Error;
    }

    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "fsolve", 1, 3);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get X
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "fsolve", 1);
        return types::Function::Error;
    }

    pDblX = in[0]->clone()->getAs<types::Double>();

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "fsolve", 1);
        return types::Function::Error;
    }

    iSizeX = pDblX->getSize();

    // get function
    opFunctionsManager = new OptimizationFunctions(L"fsolve");
    Optimization::addOptimizationFunctions(opFunctionsManager);
    opFunctionsManager->setXRows(pDblX->getRows());
    opFunctionsManager->setXCols(pDblX->getCols());

    if (in[1]->isCallable())
    {
        types::Callable* pCall = in[1]->getAs<types::Callable>();
        opFunctionsManager->setFsolveFctFunction(pCall);
    }
    else if (in[1]->isString())
    {
        types::String* pStr = in[1]->getAs<types::String>();
        char* pst = wide_string_to_UTF8(pStr->get(0));
        bool bOK = opFunctionsManager->setFsolveFctFunction(pStr);

        if (bOK == false)
        {
            Scierror(50, _("%s: Subroutine not found: %s\n"), "fsolve", pst);
            FREE(pst);
            return types::Function::Error;
        }

        FREE(pst);
    }
    else if (in[1]->isList())
    {
        types::List* pList = in[1]->getAs<types::List>();
        if (pList->getSize() == 0)
        {
            Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "fsolve", 2, "(string empty)");
            return types::Function::Error;
        }

        if (pList->get(0)->isString())
        {
            types::String* pStr = pList->get(0)->getAs<types::String>();
            char* pst = wide_string_to_UTF8(pStr->get(0));
            bool bOK = opFunctionsManager->setFsolveFctFunction(pStr);

            if (bOK == false)
            {
                Scierror(50, _("%s: Subroutine not found: %s\n"), "fsolve", pst);
                FREE(pst);
                return types::Function::Error;
            }

            FREE(pst);
        }
        else if (pList->get(0)->isCallable())
        {
            types::Callable* pCall = pList->get(0)->getAs<types::Callable>();
            opFunctionsManager->setFsolveFctFunction(pCall);
            for (int iter = 1; iter < pList->getSize(); iter++)
            {
                opFunctionsManager->setFsolveFctArgs(pList->get(iter)->getAs<types::InternalType>());
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "fsolve", 2);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "fsolve", 2);
        return types::Function::Error;
    }

    if (in.size() >= 3)
    {
        if (in[2]->isCallable())
        {
            types::Callable* pCall = in[2]->getAs<types::Callable>();
            opFunctionsManager->setFsolveJacFunction(pCall);

            bJac = true;
        }
        else if (in[2]->isString())
        {
            types::String* pStr = in[2]->getAs<types::String>();
            char* pst = wide_string_to_UTF8(pStr->get(0));
            bool bOK = opFunctionsManager->setFsolveJacFunction(pStr);

            if (bOK == false)
            {
                Scierror(50, _("%s: Subroutine not found: %s\n"), "fsolve", pst);
                FREE(pst);
                return types::Function::Error;
            }

            bJac = true;
            FREE(pst);
        }
        else if (in[2]->isList())
        {
            types::List* pList = in[2]->getAs<types::List>();
            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "fsolve", 3, "(string empty)");
                return types::Function::Error;
            }

            if (pList->get(0)->isString())
            {
                types::String* pStr = pList->get(0)->getAs<types::String>();
                char* pst = wide_string_to_UTF8(pStr->get(0));
                bool bOK = opFunctionsManager->setFsolveJacFunction(pStr);

                if (bOK == false)
                {
                    Scierror(50, _("%s: Subroutine not found: %s\n"), "fsolve", pst);
                    FREE(pst);
                    return types::Function::Error;
                }

                bJac = true;
                FREE(pst);
            }
            else if (pList->get(0)->isCallable())
            {
                types::Callable* pCall = pList->get(0)->getAs<types::Callable>();
                opFunctionsManager->setFsolveJacFunction(pCall);
                for (int iter = 1; iter < pList->getSize(); iter++)
                {
                    opFunctionsManager->setFsolveJacArgs(pList->get(iter)->getAs<types::InternalType>());
                }

                bJac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "fsolve", 3);
                return types::Function::Error;
            }
        }
    }

    if (in.size() == 4)
    {
        if (in[3]->isDouble())
        {
            pDblTol = in[3]->getAs<types::Double>();
            if (pDblTol->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "fsolve", 4);
                return types::Function::Error;
            }

            dTol = pDblTol->get(0);
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "fsolve", 4);
            return types::Function::Error;
        }
    }

    /*** perform operations ***/
    // alloc working table
    if (bJac)
    {
        iWorkSize = (iSizeX * (iSizeX + 13)) / 2;
    }
    else
    {
        iWorkSize = (iSizeX * (3 * iSizeX + 13)) / 2;
    }

    pdblWork = new double[iWorkSize];

    // alloc output data
    pDblV = new types::Double(pDblX->getDims(), pDblX->getDimsArray());

    char const * pstrFunc = "fct";
    try
    {
        if (bJac)
        {
            pstrFunc = "jac";
            pdblJac = new double[iSizeX * iSizeX];
            C2F(hybrj1)(jac, &iSizeX, pDblX->get(), pDblV->get(), pdblJac, &iSizeX, &dTol, &iInfo, pdblWork, &iWorkSize);
        }
        else
        {
            C2F(hybrd1)(fct, &iSizeX, pDblX->get(), pDblV->get(), &dTol, &iInfo, pdblWork, &iWorkSize);
        }
    }
    catch (const ast::InternalError &e)
    {
        char* pstrMsg = wide_string_to_UTF8(e.GetErrorMessage().c_str());
        sciprint(_("%s: exception caught in '%s' subroutine.\n"), "fsolve", pstrFunc);
        Scierror(999, pstrMsg);
        FREE(pstrMsg);
        delete[] pdblWork;
        delete pDblX;
        if (pdblJac)
        {
            delete[] pdblJac;
        }

        return types::Function::Error;
    }

    delete[] pdblWork;
    if (pdblJac)
    {
        delete[] pdblJac;
    }

    /*** return output arguments ***/
    out.push_back(pDblX);

    if (_iRetCount >= 2)
    {
        out.push_back(pDblV);
    }
    else
    {
        delete pDblV;
    }

    // info = 0   improper input parameters.
    // info = 1   algorithm estimates that the relative error
    // between x and the solution is at most tol.
    // info = 2   number of calls to fcn with iflag = 1 has
    // reached 100*(n+1).
    // info = 3   tol is too small. no further improvement in
    // the approximate solution x is possible.
    // info = 4   iteration is not making good progress.
    if (_iRetCount == 3)
    {
        out.push_back(new types::Double((double)iInfo));
    }

    return types::Function::OK;
}

