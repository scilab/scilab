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
#include "sciprint.h"
#include "scifunctions.h"
#include "elem_common.h"
#include "xerhlt.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_dassl(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input args
    types::Double* pDblX0   = NULL;
    types::Double* pDblT0   = NULL;
    types::Double* pDblT    = NULL;
    types::Double* pDblRtol = NULL;
    types::Double* pDblAtol = NULL;
    types::Double* pDblHd   = NULL;

    // x0 = [y0, ydot0]
    double* pdYData         = NULL; // contain y0 following by all args data in list case.
    double* pdYdotData      = NULL;
    int sizeOfpdYData       = 0;

    int iPos    = 0;
    int one     = 1;

    int info[15]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    double tstop    = 0;
    double maxstep  = 0;
    double stepin   = 0;
    int mu          = 0;
    int ml          = 0;

    int sizeOfYSize = 1;
    int* YSize      = NULL;    // YSize(1) = size of y0,
    // YSize(n) = size of Args(n) in list case.

    // Indicate if the function is given.
    bool bFuncF     = false;
    bool bFuncJac   = false;

    // Indicate if info list is given.
    bool bListInfo  = false;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() < 4 || in.size() > 9)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "dassl", 4, 9);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "dassl", 1, 2);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // x0 = [y0, yd0]
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    pDblX0 = in[iPos]->getAs<types::Double>();

    if (pDblX0->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    if (pDblX0->getCols() > 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real matrix with %d to %d column(s) expected.\n"), "dassl", iPos + 1, 1, 2);
        return types::Function::Error;
    }

    if (pDblX0->getCols() == 1)
    {
        info[10] = 1;
    }

    // t0
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    pDblT0 = in[iPos]->getAs<types::Double>();

    if (pDblT0->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    // t
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    pDblT = in[iPos]->getAs<types::Double>();

    if (pDblT->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "dassl", iPos + 1);
        return types::Function::Error;
    }

    // get next inputs
    DifferentialEquationFunctions deFunctionsManager(L"dassl");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

    YSize = (int*)MALLOC(sizeOfYSize * sizeof(int));
    *YSize = pDblX0->getRows();

    pdYData = (double*)MALLOC(*YSize * sizeof(double));
    pdYdotData = (double*)MALLOC(*YSize * sizeof(double));

    C2F(dcopy)(YSize, pDblX0->get(), &one, pdYData, &one);
    if (pDblX0->getCols() == 2)
    {
        C2F(dcopy)(YSize, pDblX0->get() + *YSize, &one, pdYdotData, &one);
    }
    else
    {
        memset(pdYdotData, 0x00, *YSize);
    }

    deFunctionsManager.setOdeYRows(pDblX0->getRows());

    for (iPos++; iPos < in.size(); iPos++)
    {
        if (in[iPos]->isDouble())
        {
            if (pDblAtol == NULL && bFuncF == false)
            {
                pDblAtol = in[iPos]->getAs<types::Double>();
                if (pDblAtol->getSize() != pDblX0->getRows() && pDblAtol->isScalar() == false)
                {
                    Scierror(267, _("%s: Wrong size for input argument #%d: A scalar or a matrix of size %d expected.\n"), "dassl", iPos + 1, pDblX0->getRows());
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblRtol == NULL && bFuncF == false)
            {
                pDblRtol = in[iPos]->getAs<types::Double>();
                if (pDblAtol->getSize() != pDblRtol->getSize())
                {
                    Scierror(267, _("%s: Wrong size for input argument #%d: Atol and Rtol must have the same size.\n"), "dassl", iPos + 1, pDblX0->getRows());
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblHd == NULL && bFuncF == true)
            {
                pDblHd = in[iPos]->getAs<types::Double>();
                if (in.size() != iPos + 1)
                {
                    Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "dassl", iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "dassl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
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
            else if (bFuncJac == false)
            {
                deFunctionsManager.setJacFunction(pCall);
                bFuncJac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a list expected.\n"), "dassl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
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
            else if (bFuncJac == false)
            {
                bOK = deFunctionsManager.setJacFunction(pStr);
                bFuncJac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a list expected.\n"), "dassl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bOK == false)
            {
                char* pst = wide_string_to_UTF8(pStr->get(0));
                Scierror(50, _("%s: Subroutine not found: %s\n"), "dassl", pst);
                FREE(pst);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
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
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "dassl", iPos + 1, "(string empty)");
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bFuncF && bListInfo)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "dassl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
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
                    Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "dassl", iPos + 1, pst);
                    FREE(pst);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
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
                        Scierror(999, _("%s: Wrong type for input argument #%d: Argument %d in the list must be a matrix.\n"), "dassl", iPos + 1, iter + 1);
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        FREE(pdYdotData);
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
            else if (pList->get(0)->isDouble() && bFuncF == true)
            {
                if (pList->getSize() != 7)
                {
                    Scierror(267, _("%s: Wrong size for input argument #%d: A list of size %d expected.\n"), "dassl", iPos + 1, 7);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }

                for (int i = 0; i < 7; i++) // info = list([],0,[],[],[],0,0)
                {
                    if (pList->get(i)->isDouble() == false || (pList->get(i)->getAs<types::Double>()->isScalar() == false && (i == 1 || i == 5 || i == 6)))
                    {
                        if (i == 1 || i == 5 || i == 6)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: Element %d in the info list must be a scalar.\n"), "dassl", iPos + 1, i);
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: Element %d in the info list must be a matrix.\n"), "dassl", iPos + 1, i);
                        }
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        FREE(pdYdotData);
                        FREE(pdYData);
                        FREE(YSize);
                        return types::Function::Error;
                    }
                }

                types::Double* pDblTemp = pList->get(0)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[3] = 1;
                    tstop = pDblTemp->get(0);
                }

                info[2] = (int)pList->get(1)->getAs<types::Double>()->get(0);

                pDblTemp = pList->get(2)->getAs<types::Double>();
                if (pDblTemp->getSize() == 2)
                {
                    info[5] = 1;
                    ml = (int)pDblTemp->get(0);
                    mu = (int)pDblTemp->get(1);
                    deFunctionsManager.setMl(ml);
                    deFunctionsManager.setMu(mu);
                }
                else if (pDblTemp->getSize() != 0)
                {
                    Scierror(267, _("%s: Wrong size for input argument #%d: Argument %d in the list must be of size %d.\n"), "dassl", iPos + 1, 3, 2);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }

                pDblTemp = pList->get(3)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[6] = 1;
                    maxstep = pDblTemp->get(0);
                }

                pDblTemp = pList->get(4)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[7] = 1;
                    stepin = pDblTemp->get(0);
                }

                info[9]  = (int)pList->get(5)->getAs<types::Double>()->get(0);
                if (pList->get(6)->getAs<types::Double>()->get(0) == 1)
                {
                    info[10] = 1;
                }

                bListInfo = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string, a function or a matrix in case of argument info.\n"), "dassl", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "dassl", iPos + 1);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            return types::Function::Error;
        }
    }

    if (bFuncF == false)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "dassl", in.size() + 1);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    if (bFuncJac == true)
    {
        info[4] = 1;
    }

    // *** Initialization. ***
    double t0   = pDblT0->get(0);
    double rpar = 0;
    int ipar    = 0;
    int idid    = 0;
    int maxord  = 5;

    //compute itol and set the tolerances rtol and atol.
    double* rtol = NULL;
    double* atol = NULL;

    if (pDblAtol)
    {
        if (pDblAtol->isScalar())
        {
            atol  = (double*)MALLOC(sizeof(double));
            *atol = pDblAtol->get(0);
        }
        else
        {
            atol    = pDblAtol->get();
            info[1] = 1;
        }
    }
    else
    {
        atol  = (double*)MALLOC(sizeof(double));
        *atol = 1.e-7;
    }

    if (pDblRtol)
    {
        if (pDblRtol->isScalar())
        {
            rtol  = (double*)MALLOC(sizeof(double));
            *rtol = pDblRtol->get(0);
        }
        else
        {
            rtol = pDblRtol->get();
        }
    }
    else // if rtol is not given atol will be used as a scalar.
    {
        if (pDblAtol && pDblAtol->isScalar() == false) // info[1] == 1
        {
            double dblSrc = 1.e-9;
            int iSize = pDblAtol->getSize();
            int iOne = 1;
            int iZero = 0;

            rtol = (double*)MALLOC(iSize * sizeof(double));
            C2F(dcopy)(&iSize, &dblSrc, &iZero, rtol, &iOne);
        }
        else
        {
            rtol    = (double*)MALLOC(sizeof(double));
            *rtol   = 1.e-9;
        }
    }

    // Compute rwork, iwork size.
    // Create them.
    int iworksize   = 20 + pDblX0->getRows();
    int rworksize   = 0;
    int* iwork      = NULL;
    double* rwork   = NULL;

    if (info[5] == 0)
    {
        rworksize = 40 + (maxord + 4) * pDblX0->getRows() + pDblX0->getRows() * pDblX0->getRows();
    }
    else if (info[4] == 1)
    {
        rworksize = 40 + (maxord + 4) * pDblX0->getRows() + (2 * ml + mu + 1) * pDblX0->getRows();
    }
    else if (info[4] == 0)
    {
        rworksize = 40 + (maxord + 4) * pDblX0->getRows() + (2 * ml + mu + 1) * pDblX0->getRows() + 2 * (pDblX0->getRows() / (ml + mu + 1) + 1);
    }

    iwork = (int*)MALLOC(iworksize * sizeof(int));
    rwork = (double*)MALLOC(rworksize * sizeof(double));

    if (pDblHd != NULL)
    {
        if (iworksize + rworksize != pDblHd->getSize())
        {
            Scierror(77, _("%s: Wrong size for input argument(s) %d: %d expected.\n"), "dassl", in.size(), iworksize + rworksize);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            FREE(iwork);
            FREE(rwork);
            if (pDblAtol == NULL || pDblAtol->isScalar())
            {
                FREE(atol);
            }
            if (pDblRtol == NULL || pDblRtol->isScalar())
            {
                FREE(rtol);
            }
            return types::Function::Error;
        }

        C2F(dcopy)(&rworksize, pDblHd->get(), &one, rwork, &one);

        for (int i = 0; i < iworksize; i++)
        {
            iwork[i] = (int)pDblHd->get(rworksize + i);
        }

        info[0] = 1;
    }

    if (info[3] == 1)
    {
        rwork[0] = tstop;
    }

    if (info[6] == 1)
    {
        rwork[1] = maxstep;
    }

    if (info[7] == 1)
    {
        rwork[2] = stepin;
    }

    if (info[5] == 1)
    {
        iwork[0] = ml;
        iwork[1] = mu;
    }

    // *** Perform operation. ***
    std::list<types::Double*> lpDblOut;
    int size = pDblX0->getRows();
    int rowsOut = 1 + pDblX0->getRows() * 2;
    int iret = 0;

    for (int i = 0; i < pDblT->getSize(); i++)
    {
        types::Double* pDblOut = new types::Double(rowsOut, 1);
        lpDblOut.push_back(pDblOut);

        double t = pDblT->get(i);
        int pos  = 0;
        pDblOut->set(pos, t);

        if (t == t0)
        {
            pos++;
            C2F(dcopy)(&size, pdYData, &one, pDblOut->get() + pos, &one);
            pos += pDblX0->getRows();
            C2F(dcopy)(&size, pdYdotData, &one, pDblOut->get() + pos, &one);

            continue;
        }

        try
        {
            C2F(dassl)(dassl_f, YSize, &t0, pdYData, pdYdotData, &t, info, rtol, atol, &idid, rwork, &rworksize, iwork, &iworksize, &rpar, &ipar, dassl_jac);

            iret = checkError(idid, "dassl");
            if (iret == 1) // error
            {
                Scierror(999, _("%s: %s return with state %d.\n"), "dassl", "dassl",  idid);
            }
        }
        catch (ast::InternalError &ie)
        {
            os << ie.GetErrorMessage();
            bCatch = true;
            iret = 1;
        }

        if (iret == 1) // error
        {
            lpDblOut.clear();
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            FREE(iwork);
            FREE(rwork);
            if (pDblAtol == NULL || pDblAtol->isScalar())
            {
                FREE(atol);
            }
            if (pDblRtol == NULL || pDblRtol->isScalar())
            {
                FREE(rtol);
            }

            if (bCatch)
            {
                wchar_t szError[bsiz];
                os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "dassl", "dassl");
                os << szError;
                throw ast::InternalError(os.str());
            }

            return types::Function::Error;
        }

        pos++;
        C2F(dcopy)(&size, pdYData, &one, pDblOut->get() + pos, &one);
        pos += size;
        C2F(dcopy)(&size, pdYdotData, &one, pDblOut->get() + pos, &one);

        if (iret == 2) // warning
        {
            break;
        }

        // iret == 0
        if (idid == 1)
        {
            pDblOut->set(0, t0);
            i--;
        }
        else if (idid == -2)
        {
            t0 = t;
            i--;
        }
        else
        {
            t0 = t;
        }

        info[0] = 1;
    }

    // *** Return result in Scilab. ***
    types::Double* pDblOut = new types::Double(rowsOut, (int)lpDblOut.size());

    int sizeOfList = (int)lpDblOut.size();
    for (int i = 0; i < sizeOfList; i++)
    {
        int pos = i * rowsOut;
        C2F(dcopy)(&rowsOut, lpDblOut.front()->get(), &one, pDblOut->get() + pos, &one);
        lpDblOut.pop_front();
    }
    out.push_back(pDblOut);

    if (_iRetCount == 2)
    {
        types::Double* pDblHdOut = new types::Double(rworksize + iworksize, 1);
        C2F(dcopy)(&rworksize, rwork, &one, pDblHdOut->get(), &one);

        for (int i = 0; i < iworksize; i++)
        {
            pDblHdOut->set(rworksize + i, (double)iwork[i]);
        }

        out.push_back(pDblHdOut);
    }

    // *** FREE. ***
    if (pDblAtol == NULL || pDblAtol->isScalar())
    {
        FREE(atol);
    }

    if (pDblRtol == NULL || pDblRtol->isScalar())
    {
        FREE(rtol);
    }

    FREE(pdYData);
    FREE(pdYdotData);
    FREE(YSize);
    FREE(rwork);
    FREE(iwork);

    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}

