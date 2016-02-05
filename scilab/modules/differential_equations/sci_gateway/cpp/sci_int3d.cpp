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
#include "elem_common.h"
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "configvariable_interface.h"
#include "sciprint.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_int3d(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //input
    types::Double* pDblX = NULL;
    types::Double* pDblY = NULL;
    types::Double* pDblZ = NULL;

    int nf          = 1;
    int minpts      = 0;
    int maxpts      = 1000;
    double epsabs   = 0.0;
    double epsrel   = 1.0e-5;
    int irestar     = 0;

    // output
    int ifail = 0;
    int nevals = 0;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 4 || in.size() > 6)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "int3d", 4, 6);
        return types::Function::Error;
    }

    // *** check number of output args according the methode. ***
    if (_iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "int3d", 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // X
    if (in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 1);
        return types::Function::Error;
    }
    pDblX = in[0]->getAs<types::Double>();
    if (pDblX->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 1);
        return types::Function::Error;
    }

    if (pDblX->getRows() != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 4 by N matrix expected.\n"), "int3d", 1);
        return types::Function::Error;
    }

    // Y
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 2);
        return types::Function::Error;
    }
    pDblY = in[1]->getAs<types::Double>();
    if (pDblY->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 2);
        return types::Function::Error;
    }

    if (pDblY->getRows() != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 4 by N matrix expected.\n"), "int3d", 2);
        return types::Function::Error;
    }

    if (pDblY->getCols() != pDblX->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Same size of input argument %d expected.\n"), "int3d", 2, 1);
        return types::Function::Error;
    }

    // Z
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 3);
        return types::Function::Error;
    }
    pDblZ = in[2]->getAs<types::Double>();
    if (pDblZ->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 3);
        return types::Function::Error;
    }

    if (pDblZ->getRows() != 4)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A 4 by N matrix expected.\n"), "int3d", 4);
        return types::Function::Error;
    }

    if (pDblZ->getCols() != pDblX->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: Same size of input argument %d expected.\n"), "int3d", 3, 1);
        return types::Function::Error;
    }

    // function
    DifferentialEquationFunctions deFunctionsManager(L"int3d");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

    if (in[3]->isCallable())
    {
        types::Callable* pCall = in[3]->getAs<types::Callable>();
        deFunctionsManager.setFFunction(pCall);
    }
    else if (in[3]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[3]->getAs<types::String>();
        bOK = deFunctionsManager.setFFunction(pStr);

        if (bOK == false)
        {
            char* pst = wide_string_to_UTF8(pStr->get(0));
            Scierror(50, _("%s: Subroutine not found: %s\n"), "int3d", pst);
            FREE(pst);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else if (in[3]->isList())
    {
        types::List* pList = in[3]->getAs<types::List>();

        if (pList->getSize() == 0)
        {
            Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "int3d", 4, "(string empty)");
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
            Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a Scilab function.\n"), "int3d", 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "int3d", 4);
        DifferentialEquation::removeDifferentialEquationFunctions();
        return types::Function::Error;
    }

    // nf (optional)
    if (in.size() > 4)
    {
        if (in[4]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
        types::Double* pDblNf = in[4]->getAs<types::Double>();
        if (pDblNf->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (pDblNf->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "int3d", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
        nf = (int)pDblNf->get(0);

        if (nf < 1)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A positive value expected.\n"), "int3d", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }

    // params (optional)
    if (in.size() == 6)
    {
        if (in[5]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 6);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        types::Double* pDblParams = in[5]->getAs<types::Double>();
        if (pDblParams->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "int3d", 6);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (pDblParams->getSize() != 4)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: %d expected.\n"), "int3d", 6, 4);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }

        if (getWarningMode())
        {
            for (int i = 0; i < 4; i++)
            {
                if (pDblParams->get(i) < 0)
                {
                    sciprint(_("%ls: Warning: Wrong value for the element %d of argument #%d: The default value will be used.\n"), L"int3d", i + 1, 6);
                }
            }
        }

        minpts = pDblParams->get(0) < 0 ? minpts : (int)pDblParams->get(0);
        maxpts = pDblParams->get(1) < 0 ? maxpts : (int)pDblParams->get(1);
        epsabs = pDblParams->get(2) < 0.0 ? epsabs : pDblParams->get(2);
        epsrel = pDblParams->get(3) < 0.0 ? epsrel : pDblParams->get(3);

        if (pDblParams->get(2) == 0.0 && pDblParams->get(3) == 0.0)
        {
            if (getWarningMode())
            {
                sciprint(_("%ls: Warning: Wrong value for the element %d and %d of argument #%d: The default value will be used.\n"), L"int3d", 3, 4, 6);
            }
            epsabs = 0.0;
            epsrel = 1.0e-5;
        }

        if (minpts > maxpts)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: minpts smaller than maxpts expected.\n"), "int3d", 6);
            DifferentialEquation::removeDifferentialEquationFunctions();
            return types::Function::Error;
        }
    }

    maxpts = std::max(maxpts, 43 * pDblX->getCols());

    // *** Perform operation. ***
    int one     = 1;
    int three   = 3;
    int maxsub  = 7 * (maxpts - 43 * pDblX->getCols()) / (8 * 43) + pDblX->getCols();
    int size    = 4 * pDblX->getCols();

    // input data: 3 rows, 4*maxsub cols
    int dataSize = 3 * 4 * maxsub;
    double* pdData = (double*)MALLOC(dataSize * sizeof(double));
    C2F(dcopy)(&size, pDblX->get(), &one, pdData, &three);
    C2F(dcopy)(&size, pDblY->get(), &one, pdData + 1, &three);
    C2F(dcopy)(&size, pDblZ->get(), &one, pdData + 2, &three);

    // output result
    double* pdResult = (double*)MALLOC(nf * sizeof(double));

    // output err
    double* pdErr = (double*)MALLOC(nf * sizeof(double));

    // workspace
    int mdiv = 1; // default value, but can be changed on parallel computers. (see dcutet.f)
    int dworkSize   = maxsub * (2 * nf + 1) + 7 * std::max(8 * mdiv, pDblX->getCols()) * nf + 1;
    int iworkSize   = maxsub + mdiv;
    double* dwork   = (double*)MALLOC(dworkSize * sizeof(double));
    int* iwork      = (int*)MALLOC(iworkSize * sizeof(int));

    int cols = pDblX->getCols();
    try
    {
        C2F(dcutet)(int3d_f, &nf, pdData, &cols, &minpts, &maxpts, &epsabs, &epsrel, &maxsub, &dworkSize, &irestar, pdResult, pdErr, &nevals, &ifail, dwork, iwork);
    }
    catch (ast::InternalError &ie)
    {
        os << ie.GetErrorMessage();
        bCatch = true;
    }

    FREE(pdData);
    FREE(dwork);
    FREE(iwork);
    DifferentialEquation::removeDifferentialEquationFunctions();

    if (bCatch)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "int3d", "dcutet");
        os << szError;
        throw ast::InternalError(os.str());
    }

    if (ifail)
    {
        if (ifail == 1)
        {
            if (getWarningMode())
            {
                sciprint(_("%ls: Warning: maxpts was too small to obtain the required accuracy.\n"), L"int3d");
            }
        }
        else if (ifail == 3)
        {
            Scierror(999, _("%s: The volume of one of the initially given tetrahedrons is zero.\n"), "int3d");
            FREE(pdResult);
            FREE(pdErr);
            return types::Function::Error;
        }
        else // normaly nerver call.
        {
            Scierror(999, _("%s: dcutet return with error %d.\n"), "int3d", ifail);
            FREE(pdResult);
            FREE(pdErr);
            return types::Function::Error;
        }
    }

    // *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(nf, 1);
    pDblOut->set(pdResult);
    out.push_back(pDblOut);

    if (_iRetCount > 1)
    {
        types::Double* pDblErrOut = new types::Double(nf, 1);
        pDblErrOut->set(pdErr);
        out.push_back(pDblErrOut);
    }

    if (_iRetCount == 3)
    {
        types::Double* pDblNevalsOut = new types::Double((double)nevals);
        out.push_back(pDblNevalsOut);
    }

    FREE(pdResult);
    FREE(pdErr);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

