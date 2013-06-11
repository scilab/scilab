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

#include "differential_equations_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "callable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "sci_warning.h"
#include "sciprint.h"
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

    DifferentialEquationFunctions* deFunctionsManager = new DifferentialEquationFunctions(L"intg");
    ConfigVariable::addDifferentialEquationFunctions(deFunctionsManager);

    // *** check the minimal number of input args. ***
    if (in.size() < 3 || in.size() > 5)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d expected.\n"), L"intg", 3);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount > 2)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"intg", 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // A
    if (in[0]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"intg", 1);
        return types::Function::Error;
    }

    types::Double* pDblA = in[0]->getAs<types::Double>();

    if (pDblA->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"intg", 1);
        return types::Function::Error;
    }

    pdA = pDblA->get(0);

    // B
    if (in[1]->isDouble() == false)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"intg", 2);
        return types::Function::Error;
    }

    types::Double* pDblB = in[1]->getAs<types::Double>();

    if (pDblB->isScalar() == false)
    {
        ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"intg", 2);
        return types::Function::Error;
    }

    pdB = pDblB->get(0);

    // function
    if (in[2]->isCallable())
    {
        types::Callable* pCall = in[2]->getAs<types::Callable>();
        deFunctionsManager->setFFunction(pCall);

    }
    else if (in[2]->isString())
    {
        bool bOK = false;
        types::String* pStr = in[2]->getAs<types::String>();
        bOK = deFunctionsManager->setFFunction(pStr);

        if (bOK == false)
        {
            ScierrorW(50, _W("%ls: Subroutine not found: %ls\n"), L"intg", pStr->get(0));
            return types::Function::Error;
        }
    }
    /*    else if(in[2]->isList())
        {
            types::List* pList = in[2]->getAs<types::List>();

            if(pList->getSize() == 0)
            {
                ScierrorW(50,_W("%ls: Argument #%d : Subroutine not found in list: %ls\n"), L"intg", 3, L"(string empty)");
                return types::Function::Error;
            }

            if(pList->get(0)->isString())
            {
                bool bOK = false;
                types::String* pStr = pList->get(0)->getAs<types::String>();
                bOK = deFunctionsManager->setOdeFFunction(pStr);

                if(bOK == false)
                {
                    ScierrorW(50,_W("%ls: Argument #%d : Subroutine not found in list: %ls\n"), L"intg", 3, pStr->get(0));
                    return types::Function::Error;
                }

                // realloc YSize to store the arguments size
                int totalSize = *YSize;
                free(YSize);
                YSize = (int*)malloc((pList->getSize() - 1) * sizeof(int));
                YSize[0] = totalSize;

                // get arguments and store their size in YSize
                std::vector<types::Double*> vpDbl;
                for(int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    if(pList->get(iter + 1)->isDouble() == false)
                    {
                        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : Argument %d in the list must be a matrix.\n"), L"intg", 3, iter+1);
                        return types::Function::Error;
                    }

                    vpDbl.push_back(pList->get(iter+1)->getAs<types::Double>());
                    YSize[iter + 1] = vpDbl[iter]->getSize();
                    totalSize += YSize[iter + 1];
                }

                // realloc Y to store arguments data in pdYData
                double* pdYDataTemp = pdYData;
                pdYData = (double*)malloc(totalSize * sizeof(double));
                C2F(dcopy)(&YSize[0], pdYDataTemp, &one, pdYData, &one);
                free(pdYDataTemp);

                int position = YSize[0];
                for(int iter = 0; iter < pList->getSize()-1; iter++)
                {
                    C2F(dcopy)(&YSize[iter + 1], vpDbl[iter]->get(), &one, &pdYData[position], &one);
                    position += vpDbl[iter]->getSize();
                }
                vpDbl.clear();
            }
            else if(pList->get(0)->isCallable())
            {
                deFunctionsManager->setOdeFFunction(pList->get(0)->getAs<types::Callable>());
                for(int iter = 1; iter < pList->getSize(); iter++)
                {
                    deFunctionsManager->setOdeFArgs(pList->get(iter)->getAs<types::InternalType>());
                }
            }
            else
            {
                ScierrorW(999, _W("%ls: Wrong type for input argument #%d : The first argument in the list must be a string or a function.\n"), L"intg", 3);
                return types::Function::Error;
            }
        }
    */
    else
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A function expected.\n"), L"intg", 3);
        return types::Function::Error;
    }

    if (in.size() > 3)
    {
        if (in[3]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"intg", 4);
            return types::Function::Error;
        }

        types::Double* pDblEpsR = in[3]->getAs<types::Double>();

        if (pDblEpsR->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"intg", 4);
            return types::Function::Error;
        }

        pdEpsR = pDblEpsR->get(0);
    }

    if (in.size() == 5)
    {
        if (in[4]->isDouble() == false)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d : A matrix expected.\n"), L"intg", 5);
            return types::Function::Error;
        }

        types::Double* pDblEpsA = in[4]->getAs<types::Double>();

        if (pDblEpsA->isScalar() == false)
        {
            ScierrorW(999, _W("%ls: Wrong size for input argument #%d : A scalar expected.\n"), L"intg", 5);
            return types::Function::Error;
        }
        pdEpsA = pDblEpsA->get(0);
    }

    // *** Create working table. ***
    int limit = 3000 / 4;
    int workSize = 3000;
    int iworkSize = (3000 / 8 + 2) * 2 + 1;

    double* work = (double*)malloc(workSize * sizeof(double));
    int* iwork = (int*)malloc(iworkSize * sizeof(int));
    double epsabs = fabs(pdEpsA);
    double epsrel = fabs(pdEpsR);

    // *** Perform operation. ***
    //C2F(dqag0)(intg_f, &pdA, &pdB, &pdEpsa, &pdEpsr, val, abserr, stk(lpal), lw, stk(lpali), liw, ifail);

    double* alist = work;
    double* blist = work  + limit;
    double* elist = blist + limit;
    double* rlist = elist + limit;

    int ier = 0;

    C2F(dqags)(intg_f, &pdA, &pdB, &epsabs, &epsrel, alist, blist, elist, rlist, &limit, iwork, &iworkSize, &result, &abserr, &ier);

    if (ier)
    {
        switch (ier)
        {
            case 1 :
            {
                ScierrorW(999, _W("%ls: Maximum number of subdivisions allowed has been achieved.\n"), L"intg");
                return types::Function::Error;
            }
            case 2 :
            {
                if (getWarningMode())
                {
                    sciprintW(_W("%ls: Warning : The occurrence of roundoff error is detected, which prevents the requested tolerance from being achieved. The error may be under-estimated.\n"), L"intg");
                }
                break;
            }
            case 3 :
            {
                ScierrorW(999, _W("%ls: Extremely bad integrand behaviour occurs at some points of the integration interval.\n"), L"intg");
                return types::Function::Error;
            }
            case 4 :
            {
                if (getWarningMode())
                {
                    sciprintW(_W("%ls: Warning : The algorithm does not converge. Roundoff error is detected in the extrapolation table. It is presumed that the requested tolerance cannot be achieved, and that the returned result is the best which can be obtained.\n"), L"intg");
                }
                break;
            }
            case 5 :
            {
                ScierrorW(999, _W("%ls: The integral is probably divergent, or slowly convergent.\n"), L"intg");
                return types::Function::Error;
            }
        }
    }

    // *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(result);
    out.push_back(pDblOut);

    if (_iRetCount == 2)
    {
        types::Double* pDblErrOut = new types::Double(abserr);
        out.push_back(pDblErrOut);
    }

    // *** free. ***
    free(work);
    free(iwork);

    ConfigVariable::removeDifferentialEquationFunctions();

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

