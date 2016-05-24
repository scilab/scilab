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

types::Function::ReturnValue sci_lsqrsolve(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    OptimizationFunctions* opFunctionsManager = NULL;

    types::Double* pDblX    = NULL;
    types::Double* pDblM    = NULL;
    types::Double* pDblV    = NULL;

    double* pdblDiag = NULL;

    int iSizeX      = 0;
    int iWorkSize   = 0;
    int iInfo       = 0;
    int iM          = 0;
    int iPos        = 0;
    int iNprint     = 0;
    int iMode       = 1;
    int iNfev       = 0;
    int iNjev       = 0;

    bool bJac       = false;

    // stop
    double dFtol    = 1.0e-8;
    double dXtol    = 1.0e-8;
    double dGtol    = 1.0e-5;
    int iMaxfev     = 1000;
    double dEpsfcn  = 0;
    double dFactor  = 100;

    if (in.size() < 3 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lsqrsolve", 3, 6);
        return types::Function::Error;
    }

    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "lsqrsolve", 1, 3);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get X
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "lsqrsolve", 1);
        return types::Function::Error;
    }

    pDblX = in[0]->clone()->getAs<types::Double>();

    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "lsqrsolve", 1);
        return types::Function::Error;
    }

    iSizeX = pDblX->getSize();

    // get function
    opFunctionsManager = new OptimizationFunctions(L"lsqrsolve");
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
            Scierror(50, _("%s: Subroutine not found: %s\n"), "lsqrsolve", pst);
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
            Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "lsqrsolve", 2, "(string empty)");
            return types::Function::Error;
        }

        if (pList->get(0)->isString())
        {
            types::String* pStr = pList->get(0)->getAs<types::String>();
            char* pst = wide_string_to_UTF8(pStr->get(0));
            bool bOK = opFunctionsManager->setFsolveFctFunction(pStr);

            if (bOK == false)
            {
                Scierror(50, _("%s: Subroutine not found: %s\n"), "lsqrsolve", pst);
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
            Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "lsqrsolve", 2);
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "lsqrsolve", 2);
        return types::Function::Error;
    }

    // get M
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real Scalar expected.\n"), "lsqrsolve", 3);
        return types::Function::Error;
    }

    pDblM = in[2]->getAs<types::Double>();

    if (pDblM->isComplex() || pDblM->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real Scalar expected.\n"), "lsqrsolve", 3);
        return types::Function::Error;
    }

    iM = (int)pDblM->get(0);

    if (iM < iSizeX)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: At most %d expected.\n"), "lsqrsolve", 3, iM);
        return types::Function::Error;
    }

    iPos = 3;

    // get jacobian
    if (in.size() >= 4)
    {
        if (in[iPos]->isCallable())
        {
            types::Callable* pCall = in[iPos]->getAs<types::Callable>();
            opFunctionsManager->setFsolveJacFunction(pCall);

            bJac = true;
            iPos++;
        }
        else if (in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            char* pst = wide_string_to_UTF8(pStr->get(0));
            bool bOK = opFunctionsManager->setFsolveJacFunction(pStr);

            if (bOK == false)
            {
                Scierror(50, _("%s: Subroutine not found: %s\n"), "lsqrsolve", pst);
                FREE(pst);
                return types::Function::Error;
            }

            bJac = true;
            iPos++;
            FREE(pst);
        }
        else if (in[iPos]->isList())
        {
            types::List* pList = in[iPos]->getAs<types::List>();
            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "lsqrsolve", 4, "(string empty)");
                return types::Function::Error;
            }

            if (pList->get(0)->isString())
            {
                types::String* pStr = pList->get(0)->getAs<types::String>();
                char* pst = wide_string_to_UTF8(pStr->get(0));
                bool bOK = opFunctionsManager->setFsolveJacFunction(pStr);

                if (bOK == false)
                {
                    Scierror(50, _("%s: Subroutine not found: %s\n"), "lsqrsolve", pst);
                    FREE(pst);
                    return types::Function::Error;
                }

                bJac = true;
                iPos++;
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
                iPos++;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "lsqrsolve", 4);
                return types::Function::Error;
            }
        }
    }

    if (bJac == false && in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "lsqrsolve", 3, 5);
        return types::Function::Error;
    }

    // get stop
    if (iPos < in.size())
    {
        if (in[iPos]->isDouble())
        {
            types::Double* pDblStop = in[iPos]->getAs<types::Double>();
            if (pDblStop->getRows() != 1 || pDblStop->getCols() != 6)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A vector of size %d x %d expected.\n"), "lsqrsolve", iPos + 1, 1, 6);
                return types::Function::Error;
            }

            dFtol   = pDblStop->get(0);
            dXtol   = pDblStop->get(1);
            dGtol   = pDblStop->get(2);
            iMaxfev = static_cast<int>(pDblStop->get(3));
            dEpsfcn = pDblStop->get(4);
            dFactor = pDblStop->get(5);

            if (dFtol < 0 || dXtol < 0 || dGtol < 0 || iMaxfev <= 0 || dFactor <= 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), "lsqrsolve", iPos + 1, 1, 6);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A vector expected.\n"), "lsqrsolve", iPos + 1);
            return types::Function::Error;
        }

        iPos++;
    }

    if (iPos < in.size())
    {
        if (in[iPos]->isDouble())
        {
            types::Double* pDblDiag = in[iPos]->getAs<types::Double>();
            if (pDblDiag->getSize() != iSizeX)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A vector of size %d expected.\n"), "lsqrsolve", iPos + 1, iSizeX);
                return types::Function::Error;
            }

            pdblDiag = pDblDiag->get();

            for (int i = 0; i < iSizeX; i++)
            {
                if (pdblDiag[i] <= 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: A strictly positive vector expected.\n"), "lsqrsolve", iPos + 1);
                    return types::Function::Error;
                }
            }

            iMode = 2;
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A vector expected.\n"), "lsqrsolve", iPos + 1);
            return types::Function::Error;
        }
    }

    /*** perform operations ***/
    // alloc working table
    double* pdblJac     = new double[iSizeX * iM];
    int* piPvt          = new int[iSizeX];
    double* pDblQtf     = new double[iSizeX];
    double* pdblWork1   = new double[iSizeX];
    double* pdblWork2   = new double[iSizeX];
    double* pdblWork3   = new double[iSizeX];
    double* pdblWork4   = new double[iM];

    if (pdblDiag == NULL) // iMode != 2
    {
        pdblDiag = new double[iSizeX];
    }

    // alloc output data
    pDblV = new types::Double(iM, 1);

    char const* pstrFunc = "fct";
    try
    {
        if (bJac)
        {
            pstrFunc = "jac";
            C2F(lmder)( lsqrjac, &iM, &iSizeX, pDblX->get(), pDblV->get(), pdblJac, &iM, &dFtol,
                        &dXtol, &dGtol, &iMaxfev, pdblDiag, &iMode, &dFactor, &iNprint, &iInfo,
                        &iNfev, &iNjev, piPvt, pDblQtf, pdblWork1, pdblWork2, pdblWork3, pdblWork4);
        }
        else
        {
            C2F(lmdif)( lsqrfct, &iM, &iSizeX, pDblX->get(), pDblV->get(), &dFtol, &dXtol, &dGtol,
                        &iMaxfev, &dEpsfcn, pdblDiag, &iMode, &dFactor, &iNprint, &iInfo, &iNfev,
                        pdblJac, &iM, piPvt, pDblQtf, pdblWork1, pdblWork2, pdblWork3, pdblWork4);
        }
    }
    catch (const ast::InternalError &e)
    {
        char* pstrMsg = wide_string_to_UTF8(e.GetErrorMessage().c_str());
        sciprint(_("%s: exception caught in '%s' subroutine.\n"), "lsqrsolve", pstrFunc);
        Scierror(999, pstrMsg);
        FREE(pstrMsg);
        delete pDblX;
        delete[] piPvt;
        delete[] pDblQtf;
        delete[] pdblWork1;
        delete[] pdblWork2;
        delete[] pdblWork3;
        delete[] pdblWork4;
        delete[] pdblJac;
        if (iMode != 2)
        {
            delete[] pdblDiag;
        }

        return types::Function::Error;
    }

    delete[] piPvt;
    delete[] pDblQtf;
    delete[] pdblWork1;
    delete[] pdblWork2;
    delete[] pdblWork3;
    delete[] pdblWork4;
    delete[] pdblJac;
    if (iMode != 2)
    {
        delete[] pdblDiag;
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
    if (_iRetCount == 3) // return
    {
        out.push_back(new types::Double((double)iInfo));
    }
    else // printf warning
    {
        switch (iInfo)
        {
            case 5:
                sciprint(_("%s: %s: Number of calls to %s has reached or exceeded %s.\n"), _("Warning"), "lsqrsolve", "fct", "maxfev");
                break;
            case 6:
                sciprint(_("%s: %s: %s is too small. No further reduction in the criterion is possible.\n"), _("Warning"), "lsqrsolve", "ftol");
                break;
            case 7:
                sciprint(_("%s: %s: %s is too small. No further reduction in the criterion is possible.\n"), _("Warning"), "lsqrsolve", "xtol");
                break;
            case 8:
                sciprint(_("%s: %s: %s is too small. %s is orthogonal to the columns of the Jacobian to machine precision.\n"), _("Warning"), "lsqrsolve", "gtol", "fvec");
                break;
        }
    }

    return types::Function::OK;
}

