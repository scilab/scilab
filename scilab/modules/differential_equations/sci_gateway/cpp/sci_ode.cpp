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
#include "context.hxx"
#include "checkodeerror.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "scifunctions.h"
#include "elem_common.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "common_structure.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_ode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // Methode
    types::String* pStrType     = NULL;
    const wchar_t* wcsType      = L"lsoda";
    int meth                    = 0;

    // y0
    //    types::Polynom* pPolyY0     = NULL;
    types::Double* pDblY0       = NULL;
    double* pdYData             = NULL; // contain y0 following by all args data in list case.
    int sizeOfpdYData           = 0;

    // Other input args
    types::Double* pDblT0       = NULL;
    types::Double* pDblT        = NULL;
    types::Double* pDblRtol     = NULL;
    types::Double* pDblAtol     = NULL;
    types::Double* pDblNg       = NULL;
    types::Double* pDblW        = NULL;
    types::Double* pDblIw       = NULL;

    // %ODEOPTIONS
    types::Double* pDblOdeOptions = NULL;

    // Result
    types::Double* pDblYOut     = NULL;
    //    types::Double* pDblTOut     = NULL;
    //    types::Polynom* pPolyYOut   = NULL;

    // Indicate if the function is given.
    bool bFuncF     = false;
    bool bFuncJac   = false;
    bool bFuncG     = false;

    int iPos        = 0; // Position in types::typed_list in

    int sizeOfYSize = 1;
    int* YSize      = NULL;    // YSize(1) = size of y0,
    // YSize(n) = size of Args(n) in list case.

    C2F(eh0001).mesflg = 1; // flag to control printing of error messages in lapack routine.
    // 1 means print, 0 means no printing.
    C2F(eh0001).lunit = 6;  // 6 = stdout

    int one = 1; // used in C2F(dcopy)

    // For root methode
    int* jroot = NULL;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", 4);
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
        }
        else if (wcscmp(wcsType, L"stiff") == 0)
        {
            meth = 2;
        }
        else if (wcscmp(wcsType, L"root") == 0)
        {
            meth = 3;
        }
        else if (wcscmp(wcsType, L"discrete") == 0)
        {
            meth = 4;
        }
        else if (wcscmp(wcsType, L"rk") == 0)
        {
            meth = 5;
        }
        else if (wcscmp(wcsType, L"rkf") == 0)
        {
            meth = 6;
        }
        else if (wcscmp(wcsType, L"fix") == 0)
        {
            meth = 7;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: It must be one of the following strings: adams, stiff, rk, rkf, fix, root or discrete.\n"), "ode", 1);
            return types::Function::Error;
        }
    }

    // *** check number of output args according the methode. ***
    if (meth < 3)
    {
        if (_iRetCount != 1 && _iRetCount != 3)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "ode", 1, 3);
            return types::Function::Error;
        }
    }
    else if (meth == 3)
    {
        if (_iRetCount == 3 || _iRetCount > 4)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d, %d or %d expected.\n"), "ode", 1, 2, 4);
            return types::Function::Error;
        }
    }
    else // meth > 3
    {
        if (_iRetCount != 1)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "ode", 1);
            return types::Function::Error;
        }
    }

    // *** check type of input args and get it. ***
    // y0
    if (in[iPos]->isDouble())
    {
        pDblY0 = in[iPos]->getAs<types::Double>();
        if (pDblY0->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "ode", iPos + 1);
            return types::Function::Error;
        }
    }
    /*
        else if(in[iPos]->isPoly())
        {
            pPolyY0 = in[iPos]->getAs<types::Polynom>();
            if(pPolyY0->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "ode", iPos+1);
                return types::Function::Error;
            }

            if(pPolyY0->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar polynom expected.\n"), "ode", iPos+1);
                return types::Function::Error;
            }

            double* dbl = (double*)MALLOC(pPolyY0->getCoef()->getSize() * sizeof(double));
            vTransposeRealMatrix(   pPolyY0->getCoef()->get(),
                                    pPolyY0->getCoef()->getRows(),
                                    pPolyY0->getCoef()->getCols(),
                                    dbl);

            pDblY0 = new types::Double(pPolyY0->getCoef()->getCols(), pPolyY0->getCoef()->getRows());
            pDblY0->set(dbl);
        }
    */
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 1);
        return types::Function::Error;
    }

    // t0
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "ode", iPos + 1);
        return types::Function::Error;
    }

    pDblT0 = in[iPos]->getAs<types::Double>();

    if (pDblT0->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "ode", iPos + 1);
        return types::Function::Error;
    }

    // t
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 1);
        return types::Function::Error;
    }

    pDblT = in[iPos]->getAs<types::Double>();

    // get next inputs
    DifferentialEquationFunctions deFunctionsManager(L"ode");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);
    deFunctionsManager.setOdeYRows(pDblY0->getRows());
    deFunctionsManager.setOdeYCols(pDblY0->getCols());

    YSize = (int*)MALLOC(sizeOfYSize * sizeof(int));
    *YSize = pDblY0->getSize();
    pdYData = (double*)MALLOC(pDblY0->getSize() * sizeof(double));
    C2F(dcopy)(YSize, pDblY0->get(), &one, pdYData, &one);

    if (meth == 4)
    {
        if (in.size() != 5)
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYData);
            FREE(YSize);
            return types::Function::Error;
        }

        if (in[4]->isCallable() == false && in[4]->isString() == false && in[4]->isList() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "ode", 5);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYData);
            FREE(YSize);
            return types::Function::Error;
        }
    }

    for (iPos++; iPos < (int)in.size(); iPos++)
    {
        if (in[iPos]->isDouble())
        {
            if (pDblRtol == NULL && bFuncF == false)
            {
                pDblRtol = in[iPos]->getAs<types::Double>();
                if (pDblRtol->getSize() != pDblY0->getSize() && pDblRtol->isScalar() == false)
                {
                    Scierror(267, _("%s: Arg %d and arg %d must have equal dimensions.\n"), "ode", pStrType ? 2 : 1, iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblAtol == NULL && bFuncF == false)
            {
                pDblAtol = in[iPos]->getAs<types::Double>();
                if (pDblAtol->getSize() != pDblY0->getSize() && pDblAtol->isScalar() == false)
                {
                    Scierror(267, _("%s: Arg %d and arg %d must have equal dimensions.\n"), "ode", pStrType ? 2 : 1, iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblNg == NULL && bFuncF == true && meth == 3)
            {
                pDblNg = in[iPos]->getAs<types::Double>();
            }
            else if (pDblW == NULL && bFuncF == true && (bFuncG == true || meth != 3))
            {
                if ((int)in.size() == iPos + 2)
                {
                    if (in[iPos + 1]->isDouble() == false)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 2);
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        FREE(pdYData);
                        FREE(YSize);
                        return types::Function::Error;
                    }

                    pDblW = in[iPos]->getAs<types::Double>();
                    pDblIw = in[iPos + 1]->getAs<types::Double>();
                    iPos++;
                }
                else
                {
                    Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", iPos + 2);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "ode", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
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
            else if (bFuncJac == false && (pDblNg == NULL || meth != 3))
            {
                deFunctionsManager.setJacFunction(pCall);
                bFuncJac = true;
            }
            else if (bFuncG == false && meth == 3)
            {
                deFunctionsManager.setGFunction(pCall);
                bFuncG = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
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
            else if (bFuncJac == false && (pDblNg == NULL || meth != 3))
            {
                bOK = deFunctionsManager.setJacFunction(pStr);
                bFuncJac = true;
            }
            else if (bFuncG == false && meth == 3)
            {
                bOK = deFunctionsManager.setGFunction(pStr);
                bFuncG = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bOK == false)
            {
                char* pst = wide_string_to_UTF8(pStr->get(0));
                Scierror(50, _("%s: Subroutine not found: %s\n"), "ode", pst);
                FREE(pst);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }
        }
        else if (in[iPos]->isList())
        {
            types::List* pList = in[iPos]->getAs<types::List>();

            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "ode", iPos + 1, "(string empty)");
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bFuncF && (bFuncJac || pDblNg) && (bFuncG || meth != 3))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "ode", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
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
                else if (bFuncJac == false && (pDblNg == NULL || meth != 3))
                {
                    bFuncJac = true;
                    bOK = deFunctionsManager.setJacFunction(pStr);
                    if (sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }
                else if (bFuncG == false && meth == 3)
                {
                    bFuncG = true;
                    bOK = deFunctionsManager.setGFunction(pStr);
                    if (sizeOfpdYData == 0)
                    {
                        sizeOfpdYData = *YSize;
                    }
                }

                if (bOK == false)
                {
                    char* pst = wide_string_to_UTF8(pStr->get(0));
                    Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "ode", iPos + 1, pst);
                    FREE(pst);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }

                int* sizeTemp = YSize;
                int totalSize = sizeOfpdYData;

                YSize = (int*)MALLOC((sizeOfYSize + pList->getSize() - 1) * sizeof(int));
                memcpy(YSize, sizeTemp, sizeOfYSize * sizeof(int));

                std::vector<types::Double*> vpDbl;
                for (int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    if (pList->get(iter + 1)->isDouble() == false)
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: Argument %d in the list must be a matrix.\n"), "ode", iPos + 1, iter + 1);
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        FREE(pdYData);
                        FREE(YSize);
                        return types::Function::Error;
                    }

                    vpDbl.push_back(pList->get(iter + 1)->getAs<types::Double>());
                    YSize[sizeOfYSize + iter] = vpDbl[iter]->getSize();
                    totalSize += YSize[sizeOfYSize + iter];
                }

                double* pdYDataTemp = pdYData;
                pdYData = (double*)MALLOC(totalSize * sizeof(double));
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
                FREE(pdYDataTemp);
                FREE(sizeTemp);
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
                else if (bFuncJac == false && (pDblNg == NULL || meth != 3))
                {
                    bFuncJac = true;
                    deFunctionsManager.setJacFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setJacArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncG == false && meth == 3)
                {
                    bFuncG = true;
                    deFunctionsManager.setGFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setGArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "ode", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "ode", iPos + 1);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYData);
            FREE(YSize);
            return types::Function::Error;
        }
    }

    if (bFuncF == false)
    {
        int val = (meth == 3) ? 3 : 1;
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", in.size() + val);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }
    if (pDblNg == NULL && meth == 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", in.size() + 2);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }
    if (bFuncG == false && meth == 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "ode", in.size() + 1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    // *** Initialization. ***

    int itol        = 1;
    int itask       = 1; // itask = 1 for normal computation of output values of y at t = tout.
    int istate      = 1; // istate = integer flag (input and output).  set istate = 1.
    int iopt        = 0; // iopt   = 0 to indicate no optional inputs used.
    int jt          = 2; // jacobian type indicator.  set jt = 2.
    int ml          = -1;
    int mu          = -1;

    // work tab
    double* rwork   = NULL;
    int* iwork      = NULL;
    int rworkSize   = 0;
    int iworkSize   = 0;

    // contain ls0001, lsa001 and eh0001 structures
    double* dStructTab  = NULL;
    int* iStructTab     = NULL;
    int dStructTabSize  = 0;
    int iStructTabSize  = 0;

    int rwSize  = 0; // rwSize = dStructTab + rworkSize
    int iwSize  = 0; // iwSize = iStructTab + iworkSize

    // structures used by lsoda and lsode
    double* ls0001d = &(C2F(ls0001).tret);
    int* ls0001i    = &(C2F(ls0001).illin);
    double* lsa001d = &(C2F(lsa001).tsw);
    int* lsa001i    = &(C2F(lsa001).insufr);
    double* lsr001d = &(C2F(lsr001).rownr3[0]);
    int* lsr001i    = &(C2F(lsr001).lg0);
    int* eh0001i    = &(C2F(eh0001).mesflg);

    // get %ODEOPTIONS
    types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(L"%ODEOPTIONS"));
    if (pIT != NULL && pIT->isDouble())
    {
        pDblOdeOptions = pIT->getAs<types::Double>();
        if (pDblOdeOptions->getSize() == 12)
        {
            iopt    = 1;
            itask   = (int)pDblOdeOptions->get(0);
            jt      = (int)pDblOdeOptions->get(5);
            ml      = (int)pDblOdeOptions->get(10);
            mu      = (int)pDblOdeOptions->get(11);
        }
    }

    if (iopt == 1 && (pDblOdeOptions->get(4) > pDblOdeOptions->get(3))) // hmin > hmax ?
    {
        Scierror(9999, _("%s: Wrong value of hmin and hmax: hmin = %d is greater than hmax = %d.\n"), "ode", pDblOdeOptions->get(4), pDblOdeOptions->get(3));
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    if (jt < 0 || jt > 5)
    {
        Scierror(9999, _("%s: Wrong value of Jacobian type: A number between %d and %d expected.\n"), "ode", 0, 5);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    if (iopt == 0 && bFuncJac)
    {
        jt = 1;
    }

    if (bFuncJac && (jt == 2 || jt == 5) && getWarningMode())
    {
        sciprint(_("%s: Warning: Jacobian is given, but not used.\n"), "ode");
    }

    if (bFuncJac == false && (jt == 1 || jt == 4))
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Warning: No Jacobian external given, but one is required by %ODEOPTIONS(6) value. Jacobian will be estimated.\n"), "ode");
        }

        jt = 2;
    }

    //compute itol and set the tolerances rtol and atol.
    double* rtol = NULL;
    double* atol = NULL;

    if (pDblRtol)
    {
        if (pDblRtol->isScalar())
        {
            rtol = (double*)MALLOC(sizeof(double));
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
        rtol = (double*)MALLOC(sizeof(double));
        if (meth == 6 || meth == 7)
        {
            *rtol = 1.e-3;
        }
        else
        {
            *rtol = 1.0e-7;
        }
    }

    if (pDblAtol)
    {
        if (pDblAtol->isScalar())
        {
            atol = (double*)MALLOC(sizeof(double));
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
        atol = (double*)MALLOC(sizeof(double));
        if (meth == 6 || meth == 7)
        {
            *atol = 1.e-4;
        }
        else
        {
            *atol = 1.0e-9;
        }
    }

    // Compute rwork, iwork size.
    // Create them.
    int nyh = (*YSize);
    if (pDblW) // structure ls0001 have been restored.
    {
        nyh = C2F(ls0001).nyh;
    }

    int mxordn = 12;
    int mxords = 5;

    if (iopt == 1)
    {
        mxordn = (int)pDblOdeOptions->get(7);
        mxords = (int)pDblOdeOptions->get(8);
    }

    if (mxordn > 12 || mxords > 5 || mxordn < 1 || mxords < 1)
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Warning: Wrong value for maximum stiff/non-stiff order allowed :\nAt most %d for mxordn, %d for mxords and no null value for both expected.\nWrong value will be reduced to the default value.\n"), "ode", 12, 5);
        }

        mxordn = 12;
        mxords = 5;
    }

    int maxord  = mxords;
    int lmat    = 0;
    int lrn     = 0;
    int lrs     = 0;

    switch (meth)
    {
        case 3: // lsodar (root)
        {
            //             lrn = 20 + nyh*(mxordn+1) + 3*neq + 3*ng,
            //             lrs = 20 + nyh*(mxords+1) + 3*neq + lmat + 3*ng,
            //          where
            //             nyh    = the initial value of neq,
            //             mxordn = 12, unless a smaller value is given as an
            //                      optional input,
            //             mxords = 5, unless a smaller value is given as an
            //                      optional input,
            //             lmat   = length of matrix work space..
            //             lmat   = neq**2 + 2              if jt = 1 or 2,
            //             lmat   = (2*ml + mu + 1)*neq + 2 if jt = 4 or 5.

            lrn = 3 * (int)pDblNg->get(0);
            lrs = lrn;
            dStructTabSize = 246 - 241;
            iStructTabSize = 59 - 50;
            jroot = (int*)MALLOC((int)pDblNg->get(0) * sizeof(int));
        }
        case 0: // lsoda
        {
            //             lrn = 20 + nyh*(mxordn+1) + 3*neq,
            //             lrs = 20 + nyh*(mxords+1) + 3*neq + lmat,
            //          where
            //             nyh    = the initial value of neq,
            //             mxordn = 12, unless a smaller value is given as an
            //                      optional input,
            //             mxords = 5, unless a smaller value is given as an
            //                      optional input,
            //             lmat   = length of matrix work space..
            //             lmat   = neq**2 + 2              if jt = 1 or 2,
            //             lmat   = (2*ml + mu + 1)*neq + 2 if jt = 4 or 5.

            if (jt == 1 || jt == 2)
            {
                lmat = (*YSize) * (*YSize) + 2;       //  if jt = 1 or 2,
            }
            else if (jt == 4 || jt == 5)
            {
                //ml and mu = -1 in all cases
                lmat = (2 * ml + mu + 1) * (*YSize) + 2; //  if jt = 4 or 5.
            }

            lrn += 20 + nyh * (mxordn + 1) + 3 * (*YSize);
            lrs += 20 + nyh * (mxords + 1) + 3 * (*YSize) + lmat;

            rworkSize = std::max(lrn, lrs);
            iworkSize = 20 + *YSize;

            dStructTabSize += 241;
            iStructTabSize += 50;

            break;
        }
        case 1: // lsode (adams) (non stiff)
        {
            maxord = mxordn;
        }
        case 2: // lsode (stiff)
        {
            //          20 + nyh*(maxord + 1) + 3*neq + lmat
            //        where
            //          nyh    = the initial value of neq,
            //          maxord = 12 (if meth = 1) or 5 (if meth = 2) (unless a
            //                   smaller value is given as an optional input),
            //          lmat   = 0             if miter = 0,
            //          lmat   = neq**2 + 2    if miter is 1 or 2,
            //          lmat   = neq + 2       if miter = 3, and
            //          lmat   = (2*ml+mu+1)*neq + 2 if miter is 4 or 5.

            if (jt == 1 || jt == 2)
            {
                lmat = (*YSize) * (*YSize) + 2;
            }
            else if (jt == 3)
            {
                lmat = (*YSize) + 2;
            }
            else if (jt == 4 || jt == 5)
            {
                lmat = (2 * ml + mu + 1) * (*YSize) + 2;
            }

            rworkSize = 20 + nyh * (maxord + 1) + 3 * (*YSize) + lmat;
            iworkSize = 20; // if jt = 0 or 3
            if (jt == 1 || jt == 2 || jt == 4 || jt == 5) // iSize = 20 + neq  otherwise
            {
                iworkSize += (*YSize);
            }

            dStructTabSize = 219;
            iStructTabSize = 41;
            break;
        }
        case 4: // lsdisc (discrete)
        {
            rworkSize = *YSize;
            iworkSize = 1;
            break;
        }
        case 5: // lsrgk (rk)
        {
            rworkSize = 9 * (*YSize);
            iworkSize = 1;
            break;
        }
        case 6: // rkf45 (rkf)
        {
            rworkSize = 3 + 8 * (*YSize);
            iworkSize = 5;
            break;
        }
        case 7: // rksimp (fix)
        {
            rworkSize = 3 + 8 * (*YSize);
            iworkSize = 1;
            break;
        }
    }

    rwSize = rworkSize + dStructTabSize;
    iwSize = iworkSize + iStructTabSize;

    rwork = (double*)MALLOC(rworkSize * sizeof(double));
    iwork = (int*)MALLOC(iworkSize * sizeof(int));

    if (meth < 4)
    {
        if (pDblW && pDblIw)
        {
            if (pDblW->getSize() != rwSize || pDblIw->getSize() != iwSize)
            {
                Scierror(9999, _("%s: Wrong size for w and iw: w = %d and iw = %d expected.\n"), "ode", rwSize, iwSize);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                FREE(rwork);
                FREE(iwork);
                if (jroot)
                {
                    FREE(jroot);
                }
                if (itol == 1 || itol == 3)
                {
                    FREE(atol);
                }
                if (itol < 3)
                {
                    FREE(rtol);
                }
                return types::Function::Error;
            }

            istate = 2; // 1 means this is the first call | 2  means this is not the first call

            dStructTab = (double*)MALLOC(dStructTabSize * sizeof(double));
            iStructTab = (int*)MALLOC(iStructTabSize * sizeof(int));

            C2F(dcopy)(&rworkSize, pDblW->get(), &one, rwork, &one);
            C2F(dcopy)(&dStructTabSize, pDblW->get() + rworkSize, &one, dStructTab, &one);

            for (int i = 0; i < iworkSize; i++)
            {
                iwork[i] = (int)pDblIw->get(i);
            }

            for (int i = 0; i < iStructTabSize; i++)
            {
                iStructTab[i] = (int)pDblIw->get(i + iworkSize);
            }
        }
        else
        {
            //if iopt = 0 default value are used without set it.
            if (iopt == 1)
            {
                if (itask >= 4)
                {
                    rwork[0] = pDblOdeOptions->get(1); // tcrit
                }

                rwork[4] = pDblOdeOptions->get(2);      // h0 =0
                rwork[5] = pDblOdeOptions->get(3);      // hmax = %inf
                rwork[6] = pDblOdeOptions->get(4);      // hmin = 0

                if (jt == 4 || jt == 5)
                {
                    iwork[0] = (int)pDblOdeOptions->get(10);// ml = -1
                    iwork[1] = (int)pDblOdeOptions->get(11);// mu = -1
                }

                if (meth == 0 || meth == 3)
                {
                    // lsoda/lsodar
                    iwork[4] = (int)pDblOdeOptions->get(9); // ixpr = 0
                    iwork[7] = (int)pDblOdeOptions->get(7); // mxordn = 12
                    iwork[8] = (int)pDblOdeOptions->get(8); // mxords = 5
                }
                else // 1 or 2
                {
                    // lsode
                    if (meth == 1)
                    {
                        iwork[4] = (int)pDblOdeOptions->get(7); // mxordn = 12
                    }
                    else // meth == 2
                    {
                        iwork[4] = (int)pDblOdeOptions->get(8); // mxords = 5
                    }
                }

                iwork[5] = (int)pDblOdeOptions->get(6); // mxstep = 500
                iwork[6] = 10;  // mxhnil = 10 maximum number of messages printed per problem
            }
        }
    }

    if (pDblOdeOptions && pDblOdeOptions->get(9) == 1)
    {
        sciprint(_("itask = %d\tmeth = %d\tjactyp = %d\tml = %d\tmu = %d\tiopt = %d\n"), itask, meth, jt, ml, mu, iopt);
        sciprint(_("tcrit = %lf\th0 = %lf\thmax = %lf\thmin = %lf\n"), pDblOdeOptions->get(1), pDblOdeOptions->get(2), pDblOdeOptions->get(3), pDblOdeOptions->get(4));
    }

    if (meth < 4)
    {
        if (pDblW && pDblIw)
        {
            int dPos    = 0;
            int iPos    = 0;
            int dSize   = 219;

            //restore ls0001 and eh0001 from w (rwork) and iw (iwork).
            C2F(dcopy)(&dSize, dStructTab, &one, ls0001d, &one);
            memcpy(ls0001i, iStructTab, 39 * sizeof(int));

            dPos = dSize;
            iPos = 39;

            //restore lsa001 from w (rwork) and iw (iwork).
            if (meth == 0 || meth == 3)
            {
                dSize = 22;
                C2F(dcopy)(&dSize, dStructTab + dPos, &one, lsa001d, &one);
                memcpy(lsa001i, &iStructTab[iPos], 9 * sizeof(int));

                dPos += dSize;
                iPos += 9;
            }

            //restore lsr001
            if (meth == 3)
            {
                dSize = 5;
                C2F(dcopy)(&dSize, dStructTab + dPos, &one, lsr001d, &one);
                memcpy(lsr001i, &iStructTab[iPos], 9 * sizeof(int));
            }
        }
    }

    // *** Perform operation. ***
    int err = 0;
    double t0 = pDblT0->get(0);
    bool bOneStep = false;

    if (itask == 5 || itask == 3 || itask == 2)
    {
        bOneStep = true;
        if (getWarningMode() && pDblT->isScalar() == false)
        {
            sciprint(_("itask = %d: At most one value of t is allowed, the last element of t is used.\n"), itask);
        }
    }

    if (bOneStep)
    {
        std::list<double*> pDblYOutList = std::list<double*>();
        std::list<double> pDblTOutList = std::list<double>();
        int iLastT = pDblT->getSize() - 1;
        double t = pDblT->get(iLastT);
        int iDir = t - t0 < 0 ? -1 : 1;

        do
        {
            std::string strMeth;
            try
            {
                switch (meth)
                {
                    case 1: // lsode (adams)
                    case 2: // lsode (stiff)
                    {
                        strMeth = "lsode";
                        int jacType = 10 * meth + jt;
                        C2F(lsode)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jacType);
                        break;
                    }
                    case 3: // lsodar
                    {
                        strMeth = "lsodar";
                        int ng = (int)pDblNg->get(0);
                        C2F(lsodar)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jt, ode_g, &ng, jroot);
                        break;
                    }
                    case 4: // lsdisc (discrete)
                    {
                        strMeth = "lsdisc";
                        C2F(lsdisc)(ode_f, YSize, pdYData, &t0, &t, rwork, &rworkSize, &istate);
                        break;
                    }
                    case 5: // lsrgk (rk)
                    {
                        strMeth = "lsrgk";
                        C2F(lsrgk)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    case 6: // rkf45 (rkf)
                    {
                        strMeth = "rkf45";
                        C2F(rkf45)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    case 7: // rksimp (fix)
                    {
                        strMeth = "rksimp";
                        C2F(rksimp)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    default: // case 0: // lsoda
                    {
                        strMeth = "lsoda";
                        C2F(lsoda)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jt);
                    }
                }

                // check error
                err = checkOdeError(meth, istate);
                if (err == 1)
                {
                    Scierror(999, _("%s: %s exit with state %d.\n"), "ode", strMeth.c_str(), istate);
                }
            }
            catch (ast::InternalError &ie)
            {
                os << ie.GetErrorMessage();
                bCatch = true;
                err = 1;
            }

            // FREE allocated data
            if (err == 1) // error case
            {
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                FREE(rwork);
                FREE(iwork);
                if (jroot)
                {
                    FREE(jroot);
                }
                if (dStructTab)
                {
                    FREE(dStructTab);
                }
                if (iStructTab)
                {
                    FREE(iStructTab);
                }
                if (itol == 1 || itol == 3)
                {
                    FREE(atol);
                }
                if (itol < 3)
                {
                    FREE(rtol);
                }

                if (bCatch)
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "ode", strMeth.c_str());
                    os << szError;
                    throw ast::InternalError(os.str());
                }

                return types::Function::Error;
            }

            pDblYOutList.push_back(pdYData);
            pDblTOutList.push_back(t0);

            if (err == 2) // warning case
            {
                if (getWarningMode())
                {
                    sciprint(_("Integration was stopped at t = %lf.\n"), t0);
                }
                break;
            }

            if (meth == 3 && istate == 3)
            {
                // istate == 3  means the integration was successful, and one or more
                //              roots were found before satisfying the stop condition
                //              specified by itask.  see jroot.
                if (getWarningMode())
                {
                    sciprint(_("%s: Warning: At t = %lf, y is a root, jroot = "), "ode", t0);
                    for (int k = 0; k < pDblNg->get(0); k++)
                    {
                        sciprint("\t%d", jroot[k]);
                    }
                    sciprint("\n");
                }

                break;
            }
        }
        while ((t0 - t) * iDir < 0);

        int iSizeList = (int)pDblYOutList.size();
        /*
                if(pPolyY0) // pPolyY0 is scalar
                {
                    // result format = [2 x iSizeList]
                    // first lime is the time of result stored in second line.
                    int size = 2 * iSizeList;
                    int* iRanks = (int*)MALLOC(size * sizeof(int));
                    int iMaxRank = pPolyY0->getMaxRank();
                    for(int i = 0; i < iSizeList; i++)
                    {
                        iRanks[i * 2] = 1; // time rank
                        iRanks[i * 2 + 1] = iMaxRank; // result rank
                    }

                    pPolyYOut = new types::Polynom(pPolyY0->getVariableName(), 2, iSizeList, iRanks);
                    pDblYOut = new types::Double(pDblY0->getRows(), 1);
                    pDblTOut = new types::Double(1, 1);

                    for(int i = 0; i < iSizeList; i++)
                    {
                        // pDblY0 contain pPolyY0 (ie: pPolyY0 = 2 + x => pDblY0 = [2 1])
                        for(int j = 0; j < pDblY0->getRows(); j++)
                        {
                            pDblYOut->set(j, pDblYOutList.front()[j]);
                        }
                        pDblTOut->set(0, pDblTOutList.front());

                        pPolyYOut->setCoef(i * 2, pDblTOut);
                        pPolyYOut->setCoef(i * 2 + 1, pDblYOut);

                        pDblYOutList.pop_front();
                        pDblTOutList.pop_front();
                    }
                }
                else
                {
        */
        pDblYOut = new types::Double(pDblY0->getRows() + 1, iSizeList);
        for (int i = 0; i < iSizeList; i++)
        {
            pDblYOut->set(i * (*YSize + 1), pDblTOutList.front());
            for (int j = 0; j < *YSize; j++)
            {
                pDblYOut->set(i * (*YSize + 1) + (j + 1), pDblYOutList.front()[j]);
            }
            pDblYOutList.pop_front();
            pDblTOutList.pop_front();
        }
        //        }
    }
    else
    {
        // Create result
        /*        if(pPolyY0)
                {
                    int size = pDblT->getSize();
                    int* iRanks = (int*)MALLOC(size * sizeof(int));
                    int iMaxRank = pPolyY0->getMaxRank();
                    for(int i = 0; i < size; i++)
                    {
                        iRanks[i] = iMaxRank;
                    }

                    pPolyYOut = new types::Polynom(pPolyY0->getVariableName(), 1, pDblT->getSize(), iRanks);

                    pDblYOut = new types::Double(pDblY0->getRows(), 1);
                }
                else
                {
        */
        pDblYOut = new types::Double(pDblY0->getRows(), pDblY0->getCols() * pDblT->getSize());
        //        }
        bool bBreak = false;
        for (int i = 0; i < pDblT->getSize(); i++)
        {
            double t = pDblT->get(i);
            std::string strMeth;

            if (itask >= 4 && t > rwork[0]) // rwork[0] => tcrit
            {
                t = rwork[0];
                bBreak = true;
            }

            try
            {
                switch (meth)
                {
                    case 1: // lsode (adams)
                    case 2: // lsode (stiff)
                    {
                        strMeth = "lsode";
                        int jacType = 10 * meth + jt;
                        C2F(lsode)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jacType);
                        break;
                    }
                    case 3: // lsodar
                    {
                        strMeth = "lsodar";
                        int ng = (int)pDblNg->get(0);
                        C2F(lsodar)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jt, ode_g, &ng, jroot);
                        break;
                    }
                    case 4: // lsdisc (discrete)
                    {
                        strMeth = "lsdisc";
                        C2F(lsdisc)(ode_f, YSize, pdYData, &t0, &t, rwork, &rworkSize, &istate);
                        break;
                    }
                    case 5: // lsrgk (rk)
                    {
                        strMeth = "lsrgk";
                        C2F(lsrgk)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    case 6: // rkf45 (rkf)
                    {
                        strMeth = "rkf45";
                        C2F(rkf45)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    case 7: // rksimp (fix)
                    {
                        strMeth = "rksimp";
                        C2F(rksimp)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &meth);
                        break;
                    }
                    default: // case 0: // lsoda
                    {
                        strMeth = "lsoda";
                        C2F(lsoda)(ode_f, YSize, pdYData, &t0, &t, &itol, rtol, atol, &itask, &istate, &iopt, rwork, &rworkSize, iwork, &iworkSize, bFuncJac ? ode_jac : NULL, &jt);
                    }
                }

                // check error
                err = checkOdeError(meth, istate);
                if (err == 1)
                {
                    Scierror(999, _("%s: %s exit with state %d.\n"), "ode", strMeth.c_str(), istate);
                }
            }
            catch (ast::InternalError &ie)
            {
                os << ie.GetErrorMessage();
                bCatch = true;
                err = 1;
            }
            // FREE allocated data
            if (err == 1) // error case
            {
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYData);
                FREE(YSize);
                FREE(rwork);
                FREE(iwork);
                if (jroot)
                {
                    FREE(jroot);
                }
                if (dStructTab)
                {
                    FREE(dStructTab);
                }
                if (iStructTab)
                {
                    FREE(iStructTab);
                }
                if (itol == 1 || itol == 3)
                {
                    FREE(atol);
                }
                if (itol < 3)
                {
                    FREE(rtol);
                }

                if (bCatch)
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "ode", strMeth.c_str());
                    os << szError;
                    throw ast::InternalError(os.str());
                }

                return types::Function::Error;
            }

            /*            if(pPolyYOut)
                        {
                            for(int j = 0; j < pDblY0->getRows(); j++)
                            {
                                pDblYOut->set(j, pdYData[j]);
                            }
                            pPolyYOut->setCoef(i, pDblYOut);
                        }
                        else
                        {
            */
            for (int j = 0; j < *YSize; j++)
            {
                pDblYOut->set(i * (*YSize) + j, pdYData[j]);
            }
            //            }

            if (err == 2) // warning case
            {
                if (getWarningMode())
                {
                    sciprint(_("Integration was stopped at t = %lf.\n"), t0);
                }

                types::Double* pDblYOutTemp = pDblYOut;
                pDblYOut = new types::Double(pDblYOutTemp->getRows(), i + 1);
                for (int k = 0; k < pDblYOut->getSize(); k++)
                {
                    pDblYOut->set(k, pDblYOutTemp->get(k));
                }
                delete pDblYOutTemp;

                break;
            }

            if (meth == 3 && istate == 3)
            {
                // istate == 3  means the integration was successful, and one or more
                //              roots were found before satisfying the stop condition
                //              specified by itask.  see jroot.

                if (getWarningMode())
                {
                    sciprint(_("%s: Warning: At t = %lf, y is a root, jroot = "), "ode", t0);
                    for (int k = 0; k < pDblNg->get(0); k++)
                    {
                        sciprint("\t%d", jroot[k]);
                    }
                    sciprint("\n");
                }

                types::Double* pDblYOutTemp = pDblYOut;
                pDblYOut = new types::Double(pDblYOutTemp->getRows(), i + 1);
                for (int k = 0; k < pDblYOut->getSize(); k++)
                {
                    pDblYOut->set(k, pDblYOutTemp->get(k));
                }
                delete pDblYOutTemp;
                break;
            }

            if (itask >= 4 && bBreak)
            {
                types::Double* pDblYOutTemp = pDblYOut;
                pDblYOut = new types::Double(pDblYOutTemp->getRows(), i + 1);
                for (int k = 0; k < pDblYOut->getSize(); k++)
                {
                    pDblYOut->set(k, pDblYOutTemp->get(k));
                }
                delete pDblYOutTemp;
                break;
            }
        }
    }

    if (meth < 4)
    {
        if (_iRetCount > 2) //save ls0001 and eh0001 following w and iw.
        {
            int dPos    = 0;
            int iPos    = 0;
            int dSize   = 219;

            if (dStructTab == NULL)
            {
                dStructTab = (double*)MALLOC(dStructTabSize * sizeof(double));
                iStructTab = (int*)MALLOC(iStructTabSize * sizeof(int));
            }

            C2F(dcopy)(&dSize, ls0001d, &one, dStructTab, &one);
            memcpy(iStructTab, ls0001i, 39 * sizeof(int));

            dPos = dSize;
            iPos = 39;

            //save lsa001
            if (meth == 0 || meth == 3)
            {
                dSize = 22;
                C2F(dcopy)(&dSize, lsa001d, &one, dStructTab + dPos, &one);
                memcpy(&iStructTab[iPos], lsa001i, 9 * sizeof(int));

                dPos += dSize;
                iPos += 9;
            }

            //save lsr001
            if (meth == 3)
            {
                dSize = 5;
                C2F(dcopy)(&dSize, lsr001d, &one, dStructTab + dPos, &one);
                memcpy(&iStructTab[iPos], lsr001i, 9 * sizeof(int));

                iPos += 9;
            }

            memcpy(&iStructTab[iPos], eh0001i, 2 * sizeof(int));
        }
    }

    // *** Return result in Scilab. ***
    /*
        if(pPolyYOut)
        {
            out.push_back(pPolyYOut); // y
        }
        else
        {
    */
    out.push_back(pDblYOut); // y
    //    }

    if (meth == 3 && _iRetCount >= 2)
    {
        int sizeOfRd = 1;
        int k = 0;

        for (int i = 0; i < pDblNg->get(0); i++)
        {
            if (jroot[i])
            {
                sizeOfRd++;
            }
        }

        types::Double* pDblRd = NULL;
        if (sizeOfRd == 1) // Not root found, return empty matrix
        {
            pDblRd = types::Double::Empty();
        }
        else
        {
            pDblRd = new types::Double(1, sizeOfRd);
            //rd: The first entry contains the stopping time.
            pDblRd->set(0, C2F(lsr001).tlast);
            for (int i = 0; i < pDblNg->get(0); i++)
            {
                if (jroot[i])
                {
                    k++;
                    pDblRd->set(k, (double)i + 1);
                }
            }
        }
        out.push_back(pDblRd); // rd
    }

    if ((meth < 3 && _iRetCount == 3) || (meth == 3 && _iRetCount == 4))
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

        out.push_back(pDblWOut);  // w
        out.push_back(pDblIwOut); // iw
    }

    // *** FREE. ***
    if (itol == 1 || itol == 3) // atol is scalar
    {
        FREE(atol);
    }

    if (itol < 3) // rtol is scalar
    {
        FREE(rtol);
    }

    FREE(pdYData);
    FREE(YSize);
    FREE(rwork);
    FREE(iwork);

    if (jroot)
    {
        FREE(jroot);
    }

    if (dStructTab)
    {
        FREE(dStructTab);
        FREE(iStructTab);
    }

    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}

