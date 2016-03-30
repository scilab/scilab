/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
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
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_int2d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //input
    types::Double* pDblX        = NULL;
    types::Double* pDblY        = NULL;
    types::Double* pDblParams   = NULL;

    double tol   = 1.0e-10;
    int iclose   = 1;
    int maxtri   = 50;
    int mevals   = 4000;
    int iflag    = 1;

    // output
    double result   = 0;
    double err      = 0;
    int nevals      = 0;
    int nu          = 0;
    int nd          = 0;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 3 || in.size() > 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "int2d", 3, 4);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "int2d", 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // X
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 1);
        return types::Function::Error;
    }
    pDblX = in[0]->getAs<types::Double>();//->clone()->getAs<types::Double>();
    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 1);
        return types::Function::Error;
    }

    if (pDblX->getRows() != 3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 3 by N matrix expected.\n"), "int2d", 1);
        return types::Function::Error;
    }

    // Y
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 2);
        return types::Function::Error;
    }
    pDblY = in[1]->getAs<types::Double>();//->clone()->getAs<types::Double>();
    if (pDblY->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 2);
        return types::Function::Error;
    }

    if (pDblY->getRows() != 3)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 3 by N matrix expected.\n"), "int2d", 2);
        return types::Function::Error;
    }

    if (pDblY->getCols() != pDblX->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Same size of input argument %d expected.\n"), "int2d", 2, 1);
        return types::Function::Error;
    }


    // function
    DifferentialEquationFunctions deFunctionsManager(L"int2d");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

    if (in[2]->isCallable())
    {
        types::Callable* pCall = in[2]->getAs<types::Callable>();
        deFunctionsManager.setFFunction(pCall);

        // check function
        double x = 1;
        double y = 1;
        double ret = int2d_f(&x, &y);
        if (ret == 0)
        {
            Scierror(50, _("%s: Argument #%d: Variable returned by scilab argument function is incorrect.\n"), "int2d", 3);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if (in[2]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[2]->getAs<types::String>();
        bOK = deFunctionsManager.setFFunction(pStr);

        if (bOK == false)
        {
            char* pst = wide_string_to_UTF8(pStr->get(0));
            Scierror(50, _("%s: Subroutine not found: %s\n"), "int2d", pst);
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
            Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "int2d", 3, "(string empty)");
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
            Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a Scilab function.\n"), "int2d", 3);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "int2d", 3);
        DifferentialEquation::removeDifferentialEquationFunctions();
        return types::Function::Error;
    }

    // params (optional)
    if (in.size() == 4)
    {
        if (in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        pDblParams = in[3]->getAs<types::Double>();
        if (pDblParams->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int2d", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (pDblParams->getSize() != 5)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), "int2d", 4, 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (getWarningMode())
        {
            if (pDblParams->get(0) < 0.0e0)
            {
                sciprint(_("%ls: Warning: Wrong value for the first element of argument #%d: The default value will be used.\n"), L"int2d", 4);
            }

            if (pDblParams->get(2) < 1)
            {
                sciprint(_("%ls: Warning: Wrong value for the third element of argument #%d: The default value will be used.\n"), L"int2d", 4);
            }

            if (pDblParams->get(3) < 1)
            {
                sciprint(_("%ls: Warning: Wrong value for the fourth element of argument #%d: The default value will be used.\n"), L"int2d", 4);
            }
        }

        tol      = pDblParams->get(0) < 0.0e0 ? tol : pDblParams->get(0);
        iclose   = (int)pDblParams->get(1);
        maxtri   = pDblParams->get(2) < 1 ? maxtri : (int)pDblParams->get(2);
        mevals   = pDblParams->get(3) < 1 ? mevals : (int)pDblParams->get(3);
        iflag    = (int)pDblParams->get(4);
    }

    // *** Perform operation. ***
    int size = pDblX->getCols();

    double* dwork   = (double*)MALLOC(9 * maxtri * sizeof(double));
    int* iwork      = (int*)MALLOC(2 * maxtri * sizeof(int));

    try
    {
        C2F(twodq)(int2d_f, &size, pDblX->get(), pDblY->get(), &tol, &iclose, &maxtri, &mevals, &result, &err, &nu, &nd, &nevals, &iflag, dwork, iwork);
    }
    catch (ast::InternalError &ie)
    {
        os << ie.GetErrorMessage();
        bCatch = false;
    }

    FREE(dwork);
    FREE(iwork);
    DifferentialEquation::removeDifferentialEquationFunctions();

    if (bCatch)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "int2d", "twodq");
        os << szError;
        throw ast::InternalError(os.str());
    }

    if (iflag)
    {
        switch (iflag)
        {
            case 1 :
            {
                Scierror(999, _("%s: Means termination for lack of space to divide another triangle.\n"), "int2d");
                break;
            }
            case 2 :
            {
                Scierror(999, _("%s: Means termination because of roundoff noise.\n"), "int2d");
                break;
            }
            case 3 :
            {
                Scierror(999, _("%s: means termination with relative error <= 5.0* machine epsilon.\n"), "int2d");
                break;
            }
            case 4 :
            {
                Scierror(999, _("%s: Means termination because the number of function evaluations has exceeded MEVALS.\n"), "int2d");
                break;
            }
            case 9 :
            {
                Scierror(999, _("%s: Means termination because of error in input flag.\n"), "int2d");
                break;
            }
            default :// normaly nerver call.
            {
                Scierror(999, _("%s: twodq return with state %d.\n"), "int2d", iflag);
            }
        }
        return types::Function::Error;
    }

    // *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(result);
    out.push_back(pDblOut);

    if (_iRetCount > 1)
    {
        types::Double* pDblErrOut = new types::Double(err);
        out.push_back(pDblErrOut);
    }

    if (_iRetCount == 3)
    {
        types::Double* pDblNevalsOut = new types::Double((double)nevals);
        out.push_back(pDblNevalsOut);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

