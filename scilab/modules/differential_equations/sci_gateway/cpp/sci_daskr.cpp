/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
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
}

/*--------------------------------------------------------------------------*/
//  This code was inspired by sci_f_daskr.f from the same folder.
//  daskr() has more optional parameters than daskr(), so the gateway
//  has to adapt to the new options
/*--------------------------------------------------------------------------*/
//[y0,nvs[,hotdata]]=daskr(y0,t0,t1[,atol[,rtol]],res[,jac],nh,h[,info[,psol][,pjac]][,hotdata])
types::Function::ReturnValue sci_daskr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    // input args
    types::Double* pDblX0   = NULL;
    types::Double* pDblT0   = NULL;
    types::Double* pDblT    = NULL;
    types::Double* pDblRtol = NULL;
    types::Double* pDblAtol = NULL;
    types::Double* pDblHd   = NULL;
    types::Double* pDblNg   = NULL;
    types::Double* pDblE7   = NULL; // 7th element of list info
    types::Double* pDblE12  = NULL; // 12th element of list info


    // x0 = [y0, ydot0]
    double* pdYData         = NULL; // contain y0 following by all args data in list case.
    double* pdYdotData      = NULL;
    int sizeOfpdYData       = 0;

    int sizeOfYSize = 1;
    int* YSize      = NULL;    // YSize(1) = size of y0,
    // YSize(n) = size of Args(n) in list case.
    int iPos    = 0;
    int iOne    = 1;

    int info[20]    = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    double tstop    = 0;
    double maxstep  = 0;
    double stepin   = 0;
    double epli     = 0;
    double dTemp    = 0;
    double stptol   = 0;
    double epinit   = 0;

    int ng      = 0;
    int mu      = 0;
    int ml      = 0;
    int maxl    = 0;
    int kmp     = 0;
    int nrmax   = 0;
    int mxnit   = 0;
    int mxnj    = 0;
    int mxnh    = 0;
    int lsoff   = 0;
    int LID     = 0;
    int LENIWP  = 0;
    int LENWP   = 0;

    // Indicate if the function is given.
    bool bFuncF     = false;
    bool bFuncJac   = false;
    bool bFuncG     = false;
    bool bFuncPsol  = false;
    bool bFuncPjac  = false;

    // Indicate if info list is given.
    bool bListInfo  = false;

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if ((int)in.size() < 6 || (int)in.size() > 13)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "daskr", 6, 13);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount != 3 && _iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "daskr", 2, 3);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***
    // x0 = [y0, yd0]
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "daskr", iPos + 1);
        return types::Function::Error;
    }

    pDblX0 = in[iPos]->getAs<types::Double>();

    if (pDblX0->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "daskr", iPos + 1);
        return types::Function::Error;
    }

    if (pDblX0->getCols() > 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real matrix with %d to %d column(s) expected.\n"), "daskr", iPos + 1, 1, 2);
        return types::Function::Error;
    }

    if (pDblX0->getCols() == 1)
    {
        info[10] = 1;
    }

    YSize = (int*)MALLOC(sizeOfYSize * sizeof(int));
    *YSize = pDblX0->getRows();

    pdYData = (double*)MALLOC(*YSize * sizeof(double));
    pdYdotData = (double*)MALLOC(*YSize * sizeof(double));

    C2F(dcopy)(YSize, pDblX0->get(), &iOne, pdYData, &iOne);
    if (pDblX0->getCols() == 2)
    {
        C2F(dcopy)(YSize, pDblX0->get() + *YSize, &iOne, pdYdotData, &iOne);
    }
    else
    {
        memset(pdYdotData, 0x00, *YSize);
    }

    // t0
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "daskr", iPos + 1);
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    pDblT0 = in[iPos]->getAs<types::Double>();

    if (pDblT0->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "daskr", iPos + 1);
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    // t
    iPos++;
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "daskr", iPos + 1);
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    pDblT = in[iPos]->getAs<types::Double>();

    if (pDblT->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "daskr", iPos + 1);
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    // get next inputs
    DifferentialEquationFunctions deFunctionsManager(L"daskr");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);

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
                    Scierror(267, _("%s: Wrong size for input argument #%d: A scalar or a matrix of size %d expected.\n"), "daskr", iPos + 1, pDblX0->getRows());
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
                    Scierror(267, _("%s: Wrong size for input argument #%d: Atol and Rtol must have the same size.\n"), "daskr", iPos + 1, pDblX0->getRows());
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else if (pDblNg == NULL && bFuncF == true)
            {
                pDblNg = in[iPos]->getAs<types::Double>();
                if (pDblNg->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), "daskr", iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
                ng = (int)pDblNg->get(0);
            }
            else if (pDblHd == NULL && bFuncF == true)
            {
                pDblHd = in[iPos]->getAs<types::Double>();
                if ((int)in.size() != iPos + 1)
                {
                    Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "daskr", iPos + 1);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "daskr", iPos + 1);
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
            else if (bFuncJac == false && pDblNg == NULL)
            {
                deFunctionsManager.setJacFunction(pCall);
                bFuncJac = true;
            }
            else if (bFuncG == false && pDblNg)
            {
                deFunctionsManager.setGFunction(pCall);
                bFuncG = true;
            }
            else if (bFuncG && bFuncPsol == false)
            {
                deFunctionsManager.setPsolFunction(pCall);
                bFuncPsol = true;
            }
            else if (bFuncPsol && bFuncPjac == false)
            {
                deFunctionsManager.setPjacFunction(pCall);
                bFuncPjac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a list expected.\n"), "daskr", iPos + 1);
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
            else if (bFuncJac == false && pDblNg == NULL)
            {
                bOK = deFunctionsManager.setJacFunction(pStr);
                bFuncJac = true;
            }
            else if (bFuncG == false && pDblNg)
            {
                bOK = deFunctionsManager.setGFunction(pStr);
                bFuncG = true;
            }
            else if (bFuncG && bFuncPsol == false)
            {
                bOK = deFunctionsManager.setPsolFunction(pStr);
                bFuncPsol = true;
            }
            else if (bFuncPsol && bFuncPjac == false)
            {
                bOK = deFunctionsManager.setPjacFunction(pStr);
                bFuncPjac = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a list expected.\n"), "daskr", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bOK == false)
            {
                char* pst = wide_string_to_UTF8(pStr->get(0));
                Scierror(50, _("%s: Subroutine not found: %s\n"), "daskr", pst);
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
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "daskr", iPos + 1, "(string empty)");
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            if (bFuncF && bListInfo)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "daskr", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }

            // functions can be passed in a list
            if (pList->get(0)->isString())
            {
                // list(string,...  function case
                types::String* pStr = pList->get(0)->getAs<types::String>();
                bool bOK = false;
                sizeOfpdYData = *YSize;

                if (bFuncF == false)
                {
                    bFuncF = true;
                    bOK = deFunctionsManager.setFFunction(pStr);
                }
                else if (bFuncJac == false && pDblNg == NULL)
                {
                    bFuncJac = true;
                    bOK = deFunctionsManager.setJacFunction(pStr);
                }
                else if (bFuncG == false && pDblNg)
                {
                    bFuncG = true;
                    bOK = deFunctionsManager.setGFunction(pStr);
                }
                else if (bFuncG && bFuncPsol == false)
                {
                    bOK = deFunctionsManager.setPsolFunction(pStr);
                    bFuncPsol = true;
                }
                else if (bFuncPsol && bFuncPjac == false)
                {
                    bOK = deFunctionsManager.setPjacFunction(pStr);
                    bFuncPjac = true;
                }

                if (bOK == false)
                {
                    char* pst = wide_string_to_UTF8(pStr->get(0));
                    Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "daskr", iPos + 1, pst);
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
                        Scierror(999, _("%s: Wrong type for input argument #%d: Argument %d in the list must be a matrix.\n"), "daskr", iPos + 1, iter + 1);
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
                C2F(dcopy)(&sizeOfpdYData, pdYDataTemp, &iOne, pdYData, &iOne);

                int position = sizeOfpdYData;
                for (int iter = 0; iter < pList->getSize() - 1; iter++)
                {
                    C2F(dcopy)(&YSize[sizeOfYSize + iter], vpDbl[iter]->get(), &iOne, &pdYData[position], &iOne);
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
                // list(macro,...  function case
                if (bFuncF == false)
                {
                    bFuncF = true;
                    deFunctionsManager.setFFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setFArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncJac == false && pDblNg == NULL)
                {
                    bFuncJac = true;
                    deFunctionsManager.setJacFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setJacArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncG == false && pDblNg)
                {
                    bFuncG = true;
                    deFunctionsManager.setGFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setGArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncG && bFuncPsol == false)
                {
                    bFuncPsol = true;
                    deFunctionsManager.setPsolFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setPsolArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (bFuncPsol && bFuncPjac == false)
                {
                    bFuncPjac = true;
                    deFunctionsManager.setPjacFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setPjacArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
            }
            else if (pList->get(0)->isDouble() && bFuncF == true)
            {
                // list(double,... then this list is the info argument
                if (pList->getSize() != 14)
                {
                    Scierror(267, _("%s: Wrong size for input argument #%d: A list of size %d expected.\n"), "daskr", iPos + 1, 7);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }

                for (int i = 0; i < 14; i++) // info = list([],0,[],[],[],0,[],0,[],0,[],[],[],1)
                {
                    if (pList->get(i)->isDouble() == false || (pList->get(i)->getAs<types::Double>()->isScalar() == false && (i == 1 || i == 5 || i == 7 || i == 9 || i == 13)))
                    {
                        if (i == 1 || i == 5 || i == 7 || i == 9 || i == 13)
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: Element %d in the info list must be a scalar.\n"), "daskr", iPos + 1, i);
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: Element %d in the info list must be a matrix.\n"), "daskr", iPos + 1, i);
                        }
                        DifferentialEquation::removeDifferentialEquationFunctions();
                        FREE(pdYdotData);
                        FREE(pdYData);
                        FREE(YSize);
                        return types::Function::Error;
                    }
                }

                //  --   subvariable tstop(info) --
                types::Double* pDblTemp = pList->get(0)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[3] = 1;
                    tstop = pDblTemp->get(0);
                }

                //  --   subvariable imode(info) --
                info[2] = (int)pList->get(1)->getAs<types::Double>()->get(0);

                //  --   subvariable band(info) --
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
                    Scierror(267, _("%s: Wrong size for input argument #%d: Argument %d in the list must be of size %d.\n"), "daskr", iPos + 1, 3, 2);
                    DifferentialEquation::removeDifferentialEquationFunctions();
                    FREE(pdYdotData);
                    FREE(pdYData);
                    FREE(YSize);
                    return types::Function::Error;
                }

                //  --   subvariable maxstep(info) --
                pDblTemp = pList->get(3)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[6] = 1;
                    maxstep = pDblTemp->get(0);
                }

                //  --   subvariable stepin(info) --
                pDblTemp = pList->get(4)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[7] = 1;
                    stepin = pDblTemp->get(0);
                }

                //  --   subvariable nonneg(info) --
                info[9]  = (int)pList->get(5)->getAs<types::Double>()->get(0);

                //  --   subvariable consistent(info) --
                pDblTemp = pList->get(6)->getAs<types::Double>();
                if (pDblTemp->isEmpty() || (pDblTemp->isScalar() && pDblTemp->get(0) == 0))
                {
                    // info(11) is then [] or [0]
                    info[10] = 0;
                }
                else
                {
                    //info then looks like list(..., [+-1 +-1 ... +-1 +-1],...)
                    info[10] = 1;
                    if (info[9] == 0 || info[9] == 2)
                    {
                        LID = 40;
                    }
                    else
                    {
                        LID = 40 + pDblX0->getRows();
                    }
                }
                pDblE7 = pDblTemp;

                //  --   subvariable iteration(info) --
                if (pList->get(7)->getAs<types::Double>()->get(0) == 1)
                {
                    info[11] = 1;
                }

                //  --   subvariable defaultKrylov(info) --
                pDblTemp = pList->get(8)->getAs<types::Double>();
                if (pDblTemp->getSize() == 0)
                {
                    // maxl and kmp need default values
                    maxl = std::min(5, pDblX0->getRows());
                    kmp = maxl;
                }
                else
                {
                    // info then looks like list(..., [maxl kmp nrmax epli],...)
                    info[12] = 1;
                    maxl  = (int)pDblTemp->get(0);
                    kmp   = (int)pDblTemp->get(1);
                    nrmax = (int)pDblTemp->get(2);
                    epli  = pDblTemp->get(3);
                }

                //  --   subvariable justConsistentComp(info) --
                dTemp = pList->get(9)->getAs<types::Double>()->get(0);
                if (dTemp)
                {
                    // Check that info(11) = 1, meaning that the provided initial values
                    // are not consistent
                    if (info[10] == 1)
                    {
                        info[13] = 1;
                    }
                }

                //  --   subvariable psolJac(info) --
                dTemp = pList->get(10)->getAs<types::Double>()->get(0);
                if (dTemp)
                {
                    info[14] = 1;
                }

                //  --   subvariable excludeAlgebraic(info) --
                pDblTemp = pList->get(11)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    info[15] = 1;
                    if (info[9] == 0 || info[9] == 2)
                    {
                        LID = 40;
                    }
                    else
                    {
                        LID = 40 + pDblX0->getRows();
                    }
                }
                pDblE12 = pDblTemp;

                //  --   subvariable defaultHeuristic(info) --
                pDblTemp = pList->get(12)->getAs<types::Double>();
                if (pDblTemp->getSize() != 0)
                {
                    // info then looks like list(..., [mxnit mxnj lsoff stptol epinit],...)
                    info[16] = 1;
                    mxnit   = (int)pDblTemp->get(0);
                    mxnj    = (int)pDblTemp->get(1);
                    mxnh    = (int)pDblTemp->get(2);
                    lsoff   = (int)pDblTemp->get(3);
                    stptol  = pDblTemp->get(4);
                    epinit  = pDblTemp->get(5);
                }

                //  --   subvariable verbosity(info) --
                dTemp = pList->get(13)->getAs<types::Double>()->get(0);
                if (dTemp == 1 || dTemp == 2)
                {
                    info[17] = (int)dTemp;
                }

                bListInfo = true;
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string, a function or a matrix in case of argument info.\n"), "daskr", iPos + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(pdYdotData);
                FREE(pdYData);
                FREE(YSize);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "daskr", iPos + 1);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            return types::Function::Error;
        }
    }

    if (bFuncF == false)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "daskr", (int)in.size() + 3);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    if (pDblNg == NULL)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "daskr", (int)in.size() + 2);
        DifferentialEquation::removeDifferentialEquationFunctions();
        FREE(pdYdotData);
        FREE(pdYData);
        FREE(YSize);
        return types::Function::Error;
    }

    if (bFuncG == false)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "daskr", (int)in.size() + 1);
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
    double t0    = pDblT0->get(0);
    int idid     = 0;
    int maxord   = 5;

    //compute itol and set the tolerances rtol and atol.
    double* rtol    = NULL;
    double* atol    = NULL;
    double rpar[2] = {0, 0 };
    int ipar[30];
    memset(ipar, 0x00, 30 * sizeof(int));

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
    int rworksize   = 0;
    int* iwork      = NULL;
    double* rwork   = NULL;
    int* root       = NULL;

    // compute size of rwork
    rworksize = 60;
    if (info[11] == 0)
    {
        rworksize += std::max(maxord + 4, 7) * pDblX0->getRows() + 3 * ng;
        if (info[5] == 0)
        {
            // For the full (dense) JACOBIAN case
            rworksize += pDblX0->getRows() * pDblX0->getRows();
        }
        else if (info[4] == 1)
        {
            // For the banded user-defined JACOBIAN case
            rworksize += (2 * ml + mu + 1) * pDblX0->getRows();
        }
        else if (info[4] == 0)
        {
            // For the banded finite-difference-generated JACOBIAN case
            rworksize += (2 * ml + mu + 1) * pDblX0->getRows() + 2 * (pDblX0->getRows() / (ml + mu + 1) + 1);
        }
    }
    else // info[11] == 1
    {
        // LENWP is the length ot the rwork segment containing
        // the matrix elements of the preconditioner P

        LENWP = pDblX0->getRows() * pDblX0->getRows();
        rworksize += (maxord + 5) * pDblX0->getRows() + 3 * ng
                     + (maxl + 3 + std::min(1, maxl - kmp)) * pDblX0->getRows()
                     + (maxl + 3) * maxl + 1 + LENWP;
    }

    if (info[15] == 1)
    {
        rworksize += pDblX0->getRows();
    }

    // compute size of iwork
    // liw = 40, then augment size according to the case
    int iworksize = 40;
    if (info[11] == 0)
    {
        iworksize += pDblX0->getRows();
    }
    else // info[11] == 1
    {
        // LENIWP is the length ot the iwork segment containing
        // the matrix indexes of the preconditioner P
        // (compressed sparse row format)
        LENIWP = 2 * pDblX0->getRows() * pDblX0->getRows();
        iworksize += LENIWP;
    }

    if (info[9] == 1 || info[9] == 3)
    {
        iworksize += pDblX0->getRows();
    }

    if (info[10] == 1 || info[15] == 1)
    {
        iworksize += pDblX0->getRows();
    }

    iwork = (int*)MALLOC(iworksize * sizeof(int));
    rwork = (double*)MALLOC(rworksize * sizeof(double));
    root  = (int*)MALLOC(ng * sizeof(int));

    if (pDblHd != NULL)
    {
        if (iworksize + rworksize != pDblHd->getSize())
        {
            Scierror(77, _("%s: Wrong size for input argument(s) %d: %d expected.\n"), "daskr", in.size(), iworksize + rworksize);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            FREE(iwork);
            FREE(rwork);
            FREE(root);
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

        C2F(dcopy)(&rworksize, pDblHd->get(), &iOne, rwork, &iOne);

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

    if (info[10] == 1)
    {
        for (int i = 0; i < pDblX0->getRows(); i++)
        {
            iwork[LID + i] = static_cast<int>(pDblE7->get(i));
        }
    }

    if (info[11] == 1)
    {
        iwork[26] = LENWP;
        iwork[27] = LENIWP;
    }

    if (info[12] == 1)
    {
        iwork[23] = maxl;
        iwork[24] = kmp;
        iwork[25] = nrmax;
        rwork[10] = epli;
    }

    if (info[14] == 1)
    {
        // Set ipar and rper
        ipar[0] = pDblX0->getRows();
        ipar[1] = pDblX0->getRows();
        ipar[2] = 2;
        ipar[3] = 2;
        ipar[4] = 1;
        rpar[0] = 0.005;
        rpar[1] = 0.05;
    }

    if (info[15] == 1)
    {
        for (int i = 0; i < pDblX0->getRows(); i++)
        {
            iwork[LID + i] = static_cast<int>(pDblE12->get(i));
        }
    }

    if (info[16] == 1)
    {
        iwork[31] = mxnit;
        iwork[32] = mxnj;
        iwork[33] = mxnh;
        iwork[34] = lsoff;
        rwork[13] = stptol;
        rwork[14] = epinit;
    }

    iwork[16] = rworksize;
    iwork[17] = iworksize;

    // *** Perform operation. ***
    std::list<types::Double*> lpDblOut;
    int size = pDblX0->getRows();
    int rowsOut = 1 + pDblX0->getRows() * 2;
    int iret = 0;
    int ididtmp = 0;

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
            C2F(dcopy)(&size, pdYData, &iOne, pDblOut->get() + pos, &iOne);
            pos += pDblX0->getRows();
            C2F(dcopy)(&size, pdYdotData, &iOne, pDblOut->get() + pos, &iOne);

            continue;
        }

        try
        {
            // SUBROUTINE DDASKR(RES, NEQ, T, Y, YPRIME, TOUT, INFO, RTOL, ATOL,
            //                  IDID, RWORK, LRW, IWORK, LIW, RPAR, IPAR, JAC, PSOL,
            //                  RT, NRT, JROOT)

            C2F(ddaskr)(dassl_f, YSize, &t0, pdYData, pdYdotData, &t,
                        info, rtol, atol, &idid, rwork, &rworksize,
                        iwork, &iworksize, rpar, ipar,
                        info[14] == 1 ? (void*)daskr_pjac : (void*)dassl_jac,
                        daskr_psol, dasrt_g, &ng, root);

            // values of idid says the same things that in ddasrt function,
            // except these two values.
            ididtmp = idid;
            // The integration to TSTOP was successfully completed (T=TSTOP)
            if (idid == 4)
            {
                idid = 2;
            }

            // One or more root found (eq to idid = 4 for dassl)
            if (idid == 5)
            {
                idid = 4;
            }

            if (idid == 6)
            {
                idid = 3;
            }

            // use the same error function that dasrt
            iret = checkError(idid, "daskr");
            if (iret == 1) // error
            {
                Scierror(999, _("%s: %s return with state %d.\n"), "daskr", "ddaskr", ididtmp);
            }
        }
        catch (ast::InternalError &ie)
        {
            os << ie.GetErrorMessage();
            bCatch = true;
            iret = 1;
        }

        // free memory when error occurred
        if (iret == 1)
        {
            lpDblOut.clear();
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(pdYdotData);
            FREE(pdYData);
            FREE(YSize);
            FREE(iwork);
            FREE(rwork);
            FREE(root);
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
                os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "daskr", "ddaskr");
                os << szError;
                throw ast::InternalError(os.str());
            }

            return types::Function::Error;
        }

        pos++;
        C2F(dcopy)(&size, pdYData, &iOne, pDblOut->get() + pos, &iOne);
        pos += size;
        C2F(dcopy)(&size, pdYdotData, &iOne, pDblOut->get() + pos, &iOne);

        if (iret == 2) // warning
        {
            if (ididtmp == -1 || ididtmp == 5 || ididtmp == 6)
            {
                pDblOut->set(0, t0);
            }
            break;
        }

        // iret == 0, check idid flag
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
        C2F(dcopy)(&rowsOut, lpDblOut.front()->get(), &iOne, pDblOut->get() + pos, &iOne);
        lpDblOut.pop_front();
    }
    out.push_back(pDblOut);

    int sizeOfRoot = 1;
    for (int i = 0; i < ng; i++)
    {
        if (root[i])
        {
            sizeOfRoot++;
        }
    }
    types::Double* pDblRoot = new types::Double(1, sizeOfRoot);
    pDblRoot->set(0, t0);
    int j = 0;
    for (int i = 0; i < ng; i++)
    {
        if (root[i])
        {
            j++;
            pDblRoot->set(j, i + 1);
        }
    }
    out.push_back(pDblRoot);

    if (_iRetCount == 3)
    {
        types::Double* pDblHdOut = new types::Double(rworksize + iworksize, 1);
        C2F(dcopy)(&rworksize, rwork, &iOne, pDblHdOut->get(), &iOne);

        for (int i = 0; i < iworksize; i++)
        {
            pDblHdOut->set(rworksize + i, (double)iwork[i]);
        }

        out.push_back(pDblHdOut);
    }

    // *** FREE memory. ***
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
    FREE(root);

    DifferentialEquation::removeDifferentialEquationFunctions();

    return types::Function::OK;
}

