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
#include "runvisitor.hxx"
#include "checkodeerror.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "elem_common.h"
#include "sciprint.h"
#include "common_structure.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_impl(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // Methode
    types::String* pStrType     = NULL;
    const wchar_t * wcsType     = L"lsoda";
    int meth                    = 2;// default methode is stiff

    // y0
    types::Double* pDblY0       = NULL;
    double* pdYData             = NULL; // contain y0 following by all args data in list case.
    int sizeOfpdYData           = 0;

    // Other input args
    types::Double* pDblYdot0    = NULL;
    types::Double* pDblT0       = NULL;
    types::Double* pDblT        = NULL;
    types::Double* pDblRtol     = NULL;
    types::Double* pDblAtol     = NULL;
    types::Double* pDblW        = NULL;
    types::Double* pDblIw       = NULL;

    // Result
    types::Double* pDblYOut     = NULL;

    // Indicate if the function is given.
    bool bFuncF     = false; // res
    bool bFuncJac   = false; // jac
    bool bFuncG     = false; // adda

    int iPos        = 0; // Position in types::typed_list in
    int maxord      = 5; // maxord = 12 (if meth = 1) or 5 (if meth = 2)

    int sizeOfYSize = 1;
    int* YSize      = NULL;    // YSize(1) = size of y0,
    // YSize(n) = size of Args(n) in list case.

    C2F(eh0001).mesflg  = 1; // flag to control printing of error messages in lapack routine.
    // 1 means print, 0 means no printing.
    C2F(eh0001).lunit   = 6; // 6 = stdout

    int one = 1; // use in dcopy

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 6 || in.size() > 12)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "impl", 6, 12);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount > 3 || _iRetCount == 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "impl", 1, 3);
        return types::Function::Error;
    }

    // *** Get the methode. ***
    if (in[0]->isString())
    {
        pStrType = in[0]->getAs<types::String>();
        wcsType = pStrType->get(0);
        iPos++;
    }

    if (iPos)
    {
        if (wcscmp(wcsType, L"adams") == 0)
        {
            meth = 1;
            maxord = 12;
        }
        else if (wcscmp(wcsType, L"stiff") == 0)
        {
            meth = 2;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: It must be one of the following strings: adams or stiff.\n"), "impl", 1);
            return types::Function::Error;
        }
    }

    // *** check type of input args and get it. ***
    // y0
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    pDblY0 = in[iPos]->getAs<types::Double>();

    if (pDblY0->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    if (pDblY0->getCols() != 1 && pDblY0->getRows() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    // ydot0
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    pDblYdot0 = in[iPos]->getAs<types::Double>();

    if (pDblYdot0->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    if (pDblYdot0->getCols() != 1 && pDblYdot0->getRows() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    // t0
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    pDblT0 = in[iPos]->getAs<types::Double>();

    if (pDblT0->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    // t
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
        return types::Function::Error;
    }

    pDblT = in[iPos]->getAs<types::Double>();

    // get next inputs
    DifferentialEquationFunctions deFunctionsManager(L"impl");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

    YSize = (int*)malloc(sizeOfYSize * sizeof(int));
    *YSize = pDblY0->getSize();
    pdYData = (double*)malloc(pDblY0->getSize() * sizeof(double));
    C2F(dcopy)(YSize, pDblY0->get(), &one, pdYData, &one);

    for (iPos++; iPos < in.size(); iPos++)
    {
        if (in[iPos]->isDouble())
        {
            if (pDblAtol == NULL && bFuncF == false)
            {
                pDblAtol = in[iPos]->getAs<types::Double>();
                if (pDblAtol->getSize() != pDblY0->getSize() && pDblAtol->isScalar() == false)
                {
                    Scierror(267, _("%s: Arg %d and arg %d must have equal dimensions.\n"), "impl", pStrType ? 2 : 1, iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    free(pdYData);
                    free(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblRtol == NULL && bFuncF == false)
            {
                pDblRtol = in[iPos]->getAs<types::Double>();
                if (pDblRtol->getSize() != pDblY0->getSize() && pDblRtol->isScalar() == false)
                {
                    Scierror(267, _("%s: Arg %d and arg %d must have equal dimensions.\n"), "impl", pStrType ? 2 : 1, iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    free(pdYData);
                    free(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblW == NULL && bFuncG == true)
            {
                if (in.size() == iPos + 2)
                {
                    if (in[iPos + 1]->isDouble() == false)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 2);
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        free(pdYData);
                        free(YSize);
                        return types::Function::Error;
                    }

                    pDblW = in[iPos]->getAs<types::Double>();
                    pDblIw = in[iPos + 1]->getAs<types::Double>();
                    iPos++;
                }
                else
                {
                    Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "impl", iPos + 2);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    free(pdYData);
                    free(YSize);
                    return types::Function::Error;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "impl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }
        }
        else if (in[iPos]->isCallable())
        {
            types::Callable* pCall = in[iPos]->getAs<types::Callable>();
            if (bFuncF == false)
            {
                deFunctionsManager.setFFunction(pCall);
                bFuncF = true;
            }
            else if (bFuncG == false)
            {
                deFunctionsManager.setGFunction(pCall);
                bFuncG = true;
            }
            else if (bFuncJac == false)
            {
                deFunctionsManager.setJacFunction(pCall);
                bFuncJac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }
        }
        else if (in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            bool bOK = false;

            if (bFuncF == false)
            {
                bOK = deFunctionsManager.setFFunction(pStr);
                bFuncF = true;
            }
            else if (bFuncG == false)
            {
                bOK = deFunctionsManager.setGFunction(pStr);
                bFuncG = true;
            }
            else if (bFuncJac == false)
            {
                bOK = deFunctionsManager.setJacFunction(pStr);
                bFuncJac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }

            if (bOK == false)
            {
                char* pst = wide_string_to_UTF8(pStr->get(0));
                Scierror(50, _("%s: Subroutine not found: %s\n"), "impl", pst);
                FREE(pst);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }
        }
        else if (in[iPos]->isList())
        {
            types::List* pList = in[iPos]->getAs<types::List>();

            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "impl", iPos + 1, "(string empty)");
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }

            if (bFuncF && bFuncG && bFuncJac)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "impl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }

            if (pList->get(0)->isString())
            {
                types::String* pStr = pList->get(0)->getAs<types::String>();
                bool bOK = false;

                if (bFuncF == false)
                {
                    bFuncF = true;
                    bOK = deFunctionsManager.setFFunction(pStr);
                    sizeOfpdYData = *YSize;
                }
                else if (bFuncG == false)
                {
                    bFuncG = true;
                    bOK = deFunctionsManager.setGFunction(pStr);
                    if (sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }
                else if (bFuncJac == false)
                {
                    bFuncJac = true;
                    bOK = deFunctionsManager.setJacFunction(pStr);
                    if (sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }

                if (bOK == false)
                {
                    char* pst = wide_string_to_UTF8(pStr->get(0));
                    Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "impl", iPos + 1, pst);
                    FREE(pst);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    free(pdYData);
                    free(YSize);
                    return types::Function::Error;
                }

                int* sizeTemp = YSize;
                int totalSize = sizeOfpdYData;

                YSize = (int*)malloc((sizeOfYSize + pList->getSize() - 1) * sizeof(int));
                memcpy(YSize, sizeTemp, sizeOfYSize * sizeof(int));

                std::vector<types::Double*> vpDbl;
                for (int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    if (pList->get(iter + 1)->isDouble() == false)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: Argument %d in the list must be a matrix.\n"), "impl", iPos + 1, iter + 1);
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        free(pdYData);
                        free(YSize);
                        return types::Function::Error;
                    }

                    vpDbl.push_back(pList->get(iter + 1)->getAs<types::Double>());
                    YSize[sizeOfYSize + iter] = vpDbl[iter]->getSize();
                    totalSize += YSize[sizeOfYSize + iter];
                }

                double* pdYDataTemp = pdYData;
                pdYData = (double*)malloc(totalSize * sizeof(double));
                C2F(dcopy)(&sizeOfpdYData, pdYDataTemp, &one, pdYData, &one);

                int position = sizeOfpdYData;
                for (int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    C2F(dcopy)(&YSize[sizeOfYSize + iter], vpDbl[iter]->get(), &one, &pdYData[position], &one);
                    position += vpDbl[iter]->getSize();
                }
                vpDbl.clear();
                sizeOfpdYData = totalSize;
                sizeOfYSize += pList->getSize() - 1;
                free(pdYDataTemp);
                free(sizeTemp);
            }
            else if (pList->get(0)->isCallable())
            {
                if (bFuncF == false)
                {
                    bFuncF = true;
                    deFunctionsManager.setFFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setFArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncG == false)
                {
                    bFuncG = true;
                    deFunctionsManager.setGFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setGArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncJac == false)
                {
                    bFuncJac = true;
                    deFunctionsManager.setJacFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setJacArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "impl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                free(pdYData);
                free(YSize);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "impl", iPos + 1);
            DifferentialEquation::removeDifferentialEquationFunctions();
            free(pdYData);
            free(YSize);
            return types::Function::Error;
        }
    }

    if (bFuncF == false)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "impl", in.size() + 1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        free(pdYData);
        free(YSize);
        return types::Function::Error;
    }

    if (bFuncG == false)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "impl", in.size() + 1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        free(pdYData);
        free(YSize);
        return types::Function::Error;
    }

    // *** Initialization. ***
    double t0   = pDblT0->get(0);
    int itol    = 1;
    int iopt    = 0;
    int istate  = 1;
    int itask   = 1;
    int jt = bFuncJac ? 1 : 2;
    int jacType = 10 * meth + jt;

    pDblYOut = new types::Double(pDblY0->getRows(), pDblT->getSize());

    // work tab
    double* rwork   = NULL;
    int* iwork      = NULL;
    int rworkSize   = 0;
    int iworkSize   = 0;

    // contain ls0001, lsa001 and eh0001 structures
    double* dStructTab  = NULL;
    int* iStructTab     = NULL;
    int dStructTabSize  = 219;  // number of double in ls0001
    int iStructTabSize  = 41;   // number of int in ls0001 (39) + eh0001 (2)

    int rwSize  = 0; // rwSize = dStructTab + rworkSize
    int iwSize  = 0; // iwSize = iStructTab + iworkSize

    // structures used by lsoda and lsode
    double* ls0001d = &(C2F(ls0001).tret);
    int* ls0001i    = &(C2F(ls0001).illin);
    int* eh0001i    = &(C2F(eh0001).mesflg);

    //compute itol and set the tolerances rtol and atol.
    double* rtol = NULL;
    double* atol = NULL;

    if (pDblRtol)
    {
        if (pDblRtol->isScalar())
        {
            rtol = (double*)malloc(sizeof(double));
            *rtol = pDblRtol->get(0);
        }
        else
        {
            rtol = pDblRtol->get();
            itol += 2;
        }
    }
    else
    {
        rtol = (double*)malloc(sizeof(double));
        *rtol = 1.e-9;
    }

    if (pDblAtol)
    {
        if (pDblAtol->isScalar())
        {
            atol = (double*)malloc(sizeof(double));
            *atol = pDblAtol->get(0);
        }
        else
        {
            atol = pDblAtol->get();
            itol ++;
        }
    }
    else
    {
        atol = (double*)malloc(sizeof(double));
        *atol = 1.e-7;
    }

    // Compute rwork, iwork size.
    // Create them.

    int nyh = (*YSize);
    if (pDblW) // structure ls0001 have been restored.
    {
        nyh = C2F(ls0001).nyh;
    }

    rworkSize = 20 + nyh * (maxord + 1) + 3 * *YSize + *YSize **YSize + 2;
    iworkSize = 20 + *YSize;

    rwSize = rworkSize + dStructTabSize;
    iwSize = iworkSize + iStructTabSize;

    rwork = (double*)malloc(rworkSize * sizeof(double));
    iwork = (int*)malloc(iworkSize * sizeof(int));

    if (pDblW && pDblIw)
    {
        if (pDblW->getSize() != rwSize || pDblIw->getSize() != iwSize)
        {
            Scierror(9999, _("%s: Wrong size for w and iw: w = %d and iw = %d expected.\n"), "impl", rwSize, iwSize);
            DifferentialEquation::removeDifferentialEquationFunctions();
            free(pdYData);
            free(YSize);
            free(rwork);
            free(iwork);
            if (itol == 1 || itol == 3)
            {
                free(atol);
            }
            if (itol < 3)
            {
                free(rtol);
            }
            return types::Function::Error;
        }

        istate = 2; // 1 means this is the first call | 2  means this is not the first call

        // restore rwork from pDblW
        C2F(dcopy)(&rworkSize, pDblW->get(), &one, rwork, &one);

        // restore iwork from pDblIw
        iStructTab = (int*)malloc(iStructTabSize * sizeof(int));
        for (int i = 0; i < iworkSize; i++)
        {
            iwork[i] = (int)pDblIw->get(i);
        }

        //restore ls0001d from pDblW
        C2F(dcopy)(&dStructTabSize, pDblW->get() + rworkSize, &one, ls0001d, &one);

        //restore ls0001i from pDblIw
        for (int i = 0; i < iStructTabSize; i++)
        {
            iStructTab[i] = (int)pDblIw->get(i + iworkSize);
        }
        memcpy(ls0001i, iStructTab, 39 * sizeof(int));
    }

    // *** Perform operation. ***
    int err = 0;
    for (int i = 0; i < pDblT->getSize(); i++)
    {
        double t = pDblT->get(i);
        try
        {
            C2F(lsodi)(impl_f, impl_g, impl_jac, YSize, pdYData, pDblYdot0->get(), &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, &jacType);

            // check error
            if (istate == 3)
            {
                sciprint(_("The user-supplied subroutine res signalled lsodi to halt the integration and return (ires=2). Execution of the external function has failed.\n"));
                err = 1;
                Scierror(999, _("%s: %s exit with state %d.\n"), "impl", "lsodi", istate);
            }
            else
            {
                err = checkOdeError(meth, istate);
                if (err == 1)
                {
                    Scierror(999, _("%s: %s exit with state %d.\n"), "impl", "lsodi", istate);
                }
            }
        }
        catch (ast::InternalError &ie)
        {
            os << ie.GetErrorMessage();
            bCatch = true;
            err = 1;
        }

        if (err == 1)
        {
            DifferentialEquation::removeDifferentialEquationFunctions();
            free(pdYData);
            free(YSize);
            free(rwork);
            free(iwork);
            if (iStructTab)
            {
                free(iStructTab);
            }
            if (itol == 1 || itol == 3)
            {
                free(atol);
            }
            if (itol < 3)
            {
                free(rtol);
            }

            if (bCatch)
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "impl", "lsodi");
                os << szError;
                throw ast::InternalError(os.str());
            }

            return types::Function::Error;
        }

        for (int j = 0; j < *YSize; j++)
        {
            pDblYOut->set(i * (*YSize) + j, pdYData[j]);
        }
    }

    if (_iRetCount > 2) //save ls0001 and eh0001 following pDblW and pDblIw.
    {
        int dSize   = 219;

        if (iStructTab == NULL)
        {
            iStructTab = (int*)malloc(iStructTabSize * sizeof(int));
        }

        if (dStructTab == NULL)
        {
            dStructTab = (double*)malloc(dStructTabSize * sizeof(double));
        }

        // save ls0001
        C2F(dcopy)(&dSize, ls0001d, &one, dStructTab, &one);
        memcpy(iStructTab, ls0001i, 39 * sizeof(int));

        // save eh0001
        memcpy(&iStructTab[39], eh0001i, 2 * sizeof(int));
    }

    // *** Return result in Scilab. ***
    out.push_back(pDblYOut);

    if (_iRetCount > 2)
    {
        types::Double* pDblWOut = new types::Double(1, rwSize);
        C2F(dcopy)(&rworkSize, rwork, &one, pDblWOut->get(), &one);
        C2F(dcopy)(&dStructTabSize, dStructTab, &one, pDblWOut->get() + rworkSize, &one);

        types::Double* pDblIwOut = new types::Double(1, iwSize);
        for (int i = 0; i < iworkSize; i++)
        {
            pDblIwOut->set(i, (double)iwork[i]);
        }

        for (int i = 0; i < iStructTabSize; i++)
        {
            pDblIwOut->set(iworkSize + i, (double)iStructTab[i]);
        }

        out.push_back(pDblWOut);
        out.push_back(pDblIwOut);
    }

    // *** free. ***
    if (itol == 1 || itol == 3) // atol is scalar
    {
        free(atol);
    }

    if (itol < 3) // rtol is scalar
    {
        free(rtol);
    }

    free(pdYData);
    free(YSize);
    free(rwork);
    free(iwork);

    if (dStructTab)
    {
        free(dStructTab);
    }

    if (iStructTab)
    {
        free(iStructTab);
    }

    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}

