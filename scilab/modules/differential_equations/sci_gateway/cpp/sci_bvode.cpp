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
#include "sciprint.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_bvode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iPos = 0;

    // input
    types::Double* pDblXpts     = NULL;
    types::Double* pDblM        = NULL;
    types::Double* pDblZeta     = NULL;
    types::Double* pDblIpar     = NULL;
    types::Double* pDblLtol     = NULL;
    types::Double* pDblTol      = NULL;
    types::Double* pDblFixpnt   = NULL;

    double aleft    = 0;
    double aright   = 0;
    int ncomp       = 0;
    int sumM        = 0;
    int maxM        = 0;
    int iflag       = 0;
    int ipar[11];

    // error message catched
    std::wostringstream os;
    bool bCatch = false;

    // *** check the minimal number of input args. ***
    if (in.size() != 15)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "bvode", 15);
        return types::Function::Error;
    }

    // *** check number of output args ***
    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "bvode", 1);
        return types::Function::Error;
    }

    // *** check type of input args and get it. ***

    // xpoints
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        return types::Function::Error;
    }

    pDblXpts = in[iPos]->getAs<types::Double>();
    iPos++;

    // ncomp
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        return types::Function::Error;
    }

    types::Double* pDblN = in[iPos]->getAs<types::Double>();

    if (pDblN->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        return types::Function::Error;
    }

    ncomp = (int)pDblN->get(0);

    if (ncomp > 20)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Value at most 20 expected.\n"), "bvode", iPos + 1);
        return types::Function::Error;
    }
    iPos++;

    // m
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        return types::Function::Error;
    }

    pDblM = in[iPos]->getAs<types::Double>();

    if (pDblM->getSize() != ncomp)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of size %d (N) expected.\n"), "bvode", iPos + 1, ncomp);
        return types::Function::Error;
    }

    int* M = (int*)MALLOC(pDblM->getSize() * sizeof(int));
    for (int i = 0; i < pDblM->getSize(); i++)
    {
        M[i] = (int)pDblM->get(i);
        sumM += (int)pDblM->get(i);
        maxM = std::max(maxM, (int)pDblM->get(i));
    }

    if (sumM > 40)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Sum of m (%d) must be less than 40.\n"), "bvode", iPos + 1, sumM);
        FREE(M);
        return types::Function::Error;
    }

    iPos++;

    // aleft
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    types::Double* pDblXLow = in[iPos]->getAs<types::Double>();

    if (pDblXLow->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    aleft = pDblXLow->get(0);
    iPos++;

    // aright
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    types::Double* pDblXUp = in[iPos]->getAs<types::Double>();

    if (pDblXUp->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    aright = pDblXUp->get(0);
    iPos++;

    // zeta
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    pDblZeta = in[iPos]->getAs<types::Double>();

    for (int i = 0; i < pDblZeta->getSize() - 1; i++)
    {
        if (pDblZeta->get(i) > pDblZeta->get(i + 1))
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: zeta(j) lower or equal to zeta(j+1) expected.\n"), "bvode", iPos + 1);
            FREE(M);
            return types::Function::Error;
        }
    }

    iPos++;

    // ipar
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    pDblIpar = in[iPos]->getAs<types::Double>();

    if (pDblIpar->getSize() != 11)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A vector of size 11 expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    for (int i = 0; i < 11; i++)
    {
        ipar[i] = (int)pDblIpar->get(i);
    }

    // check ipar arguments

    // ipar(1) : 0 => linear  1 => nonlinear
    if (ipar[0] != 0 && ipar[0] != 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %s: %d or %d expected.\n"), "bvode", iPos + 1, 1, 0, 1);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(2)=0 then collpnt is set to max(max(m(j))+1, 5-max(m(j))).
    if (ipar[1] == 0)
    {
        ipar[1] = std::max(maxM + 1, 5 - maxM);
    }

    if (ipar[1] < 0 || ipar[1] > 7)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %s: %d to %d expected.\n"), "bvode", iPos + 1, 2, 0, 7);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(3)
    if (ipar[2] == 0)
    {
        ipar[2] = 5;
    }

    // 0 < ipar(4) <= M
    if (ipar[3] < 0 || ipar[3] > sumM)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %s: %d to sum of m (=%d) expected.\n"), "bvode", iPos + 1, 4, 0, sumM);
        FREE(M);
        return types::Function::Error;
    }

    // bvode: ipar(5)>=nmax*nsizef where
    // nsizef=4 + 3*M + (5+collpnt*N)*(collpnt*N+M) + (2*M-nrec)*2*M
    int nmax = ipar[2];
    int kdm  = ipar[1] * ncomp;
    int nrec = 0;
    for (int i = 0; i < sumM; i++)
    {
        int ib = sumM + 1 - i;
        if (pDblZeta->get(ib - 1) >= aright)
        {
            nrec = i;
        }
    }

    int nsizef = 4 + 3 * sumM + (5 + kdm) * (kdm + sumM) + (2 * sumM - nrec) * 2 * sumM;
    if (ipar[4] < nmax * nsizef)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d.\n"), "bvode", iPos + 1, 5);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(6)>=nmax*nsizei where nsizei= 3 + collpnt*N + M.
    int nsizei = 3 + kdm + sumM;
    if (ipar[5] < nmax * nsizei)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d.\n"), "bvode", iPos + 1, 6);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(7) output control
    if (ipar[6] < -1 || ipar[6] > 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d: %d to %d expected.\n"), "bvode", iPos + 1, 7, -1, 1);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(8)
    if (ipar[7] < 0 || ipar[7] > 2)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d: %d to %d expected.\n"), "bvode", iPos + 1, 8, 0, 2);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(9)
    if (ipar[8] < 0 || ipar[8] > 4)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d: %d to %d expected.\n"), "bvode", iPos + 1, 9, 0, 4);
        FREE(M);
        return types::Function::Error;
    }

    // ipar(10)
    if (ipar[9] < 0 || ipar[9] > 2)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Wrong value for element %d: %d to %d expected.\n"), "bvode", iPos + 1, 9, 0, 2);
        FREE(M);
        return types::Function::Error;
    }


    iPos++;

    // ltol
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    pDblLtol = in[iPos]->getAs<types::Double>();

    if (pDblLtol->getSize() != ipar[3])
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: An array of size %d (ipar(4)) expected.\n"), "bvode", iPos + 1, ipar[3]);
        FREE(M);
        return types::Function::Error;
    }

    // verify following cases :
    // 1 <= ltol(1) < ltol(2) < ... < ltol(NTOL) <= M where M=sum(m)
    // M is mstar and NTOL is the size of ltol
    //
    // 1 <= ltol(1) <= M
    double* pdblLtol = pDblLtol->get();
    if (pdblLtol[0] < 1 || pdblLtol[0] > sumM)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: %d to sum of m (=%d) expected.\n"), "bvode", iPos + 1, 1, sumM);
        FREE(M);
        return types::Function::Error;
    }

    // ltol(1) < ltol(2) < ... < ltol(NTOL) <= M
    for (int i = 1; i < pDblLtol->getSize(); i++)
    {
        if (pdblLtol[i - 1] >= pdblLtol[i] || pdblLtol[i - 1] > sumM)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Bad value for ltol(%d): ltol(1) < ltol(2) < ... < ltol(NTOL) <= M (sum of m) expected.\n"), "bvode", iPos + 1, i);
            FREE(M);
            return types::Function::Error;
        }
    }

    iPos++;

    // tol
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    pDblTol = in[iPos]->getAs<types::Double>();

    if (pDblTol->getSize() != ipar[3])
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: An array of size %d (ipar(4)) expected.\n"), "bvode", iPos + 1, ipar[3]);
        FREE(M);
        return types::Function::Error;
    }

    iPos++;

    // fixpnt
    if (in[iPos]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "bvode", iPos + 1);
        FREE(M);
        return types::Function::Error;
    }

    pDblFixpnt = in[iPos]->getAs<types::Double>();

    if (pDblFixpnt->getSize() != ipar[10] && ipar[10] != 0)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: An array of size %d (ipar(11)) expected.\n"), "bvode", iPos + 1, ipar[10]);
        FREE(M);
        return types::Function::Error;
    }

    iPos++;

    // functions: fsub,dfsub,gsub,dgsub,guess
    DifferentialEquationFunctions deFunctionsManager(L"bvode");
    DifferentialEquation::addDifferentialEquationFunctions(&deFunctionsManager);
    deFunctionsManager.setBvodeM(sumM);
    deFunctionsManager.setBvodeN(ncomp);

    for (int i = iPos; i < 15; i++)
    {
        if (in[i]->isCallable())
        {
            types::Callable* pCall = in[i]->getAs<types::Callable>();
            if (i == 10) // fsub
            {
                deFunctionsManager.setFsubFunction(pCall);
            }
            else if (i == 11) // dfsub
            {
                deFunctionsManager.setDfsubFunction(pCall);
            }
            else if (i == 12) // gsub
            {
                deFunctionsManager.setGsubFunction(pCall);
            }
            else if (i == 13) // dgsub
            {
                deFunctionsManager.setDgsubFunction(pCall);
            }
            else if (i == 14 && ipar[8] == 1) // guess is needed only if ipar(9) == 1
            {
                deFunctionsManager.setGuessFunction(pCall);
            }
        }
        else if (in[i]->isString())
        {
            bool bOK = false;
            types::String* pStr = in[i]->getAs<types::String>();
            if (i == 10) // fsub
            {
                bOK = deFunctionsManager.setFsubFunction(pStr);
            }
            else if (i == 11) // dfsub
            {
                bOK = deFunctionsManager.setDfsubFunction(pStr);
            }
            else if (i == 12) // gsub
            {
                bOK = deFunctionsManager.setGsubFunction(pStr);
            }
            else if (i == 13) // dgsub
            {
                bOK = deFunctionsManager.setDgsubFunction(pStr);
            }
            else if (i == 14 && ipar[8] == 1) // guess is needed only if ipar(9) == 1
            {
                bOK = deFunctionsManager.setGuessFunction(pStr);
            }

            if (bOK == false)
            {
                char* pst = wide_string_to_UTF8(pStr->get(0));
                Scierror(50, _("%s: Subroutine not found: %s\n"), "bvode", pst);
                FREE(pst);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(M);
                return types::Function::Error;
            }
        }
        else if (in[i]->isList())
        {
            types::List* pList = in[i]->getAs<types::List>();

            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "bvode", i + 1, "(string empty)");
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(M);
                return types::Function::Error;
            }

            if (pList->get(0)->isCallable())
            {
                if (i == 10) // fsub
                {
                    deFunctionsManager.setFsubFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setFsubArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (i == 11) // dfsub
                {
                    deFunctionsManager.setDfsubFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setDfsubArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (i == 12) // gsub
                {
                    deFunctionsManager.setGsubFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setGsubArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (i == 13) // dgsub
                {
                    deFunctionsManager.setDgsubFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setDgsubArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
                else if (i == 14 && ipar[8] == 1) // guess is needed only if ipar(9) == 1
                {
                    deFunctionsManager.setGuessFunction(pList->get(0)->getAs<types::Callable>());
                    for (int iter = 1; iter < pList->getSize(); iter++)
                    {
                        deFunctionsManager.setGuessArgs(pList->get(iter)->getAs<types::InternalType>());
                    }
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a Scilab function.\n"), "bvode", i + 1);
                DifferentialEquation::removeDifferentialEquationFunctions();
                FREE(M);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A function expected.\n"), "bvode", i + 1);
            DifferentialEquation::removeDifferentialEquationFunctions();
            FREE(M);
            return types::Function::Error;
        }
    }

    // *** Perform operation. ***
    double* rwork   = (double*)MALLOC(ipar[4] * sizeof(double));
    int* iwork      = (int*)MALLOC(ipar[5] * sizeof(int));
    int* ltol       = (int*)MALLOC(pDblLtol->getSize() * sizeof(int));

    for (int i = 0; i < pDblLtol->getSize(); i++)
    {
        ltol[i] = (int)pDblLtol->get(i);
    }

    try
    {
        C2F(colnew)(&ncomp, M, &aleft, &aright, pDblZeta->get(), ipar, ltol, pDblTol->get(), pDblFixpnt->get(), iwork, rwork, &iflag, bvode_fsub, bvode_dfsub, bvode_gsub, bvode_dgsub, bvode_guess);
    }
    catch (ast::InternalError &ie)
    {
        os << ie.GetErrorMessage();
        bCatch = true;
    }

    if (bCatch)
    {
        FREE(iwork);
        FREE(rwork);
        FREE(M);
        FREE(ltol);
        DifferentialEquation::removeDifferentialEquationFunctions();

        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("%s: An error occured in '%s' subroutine.\n").c_str(), "bvode", "bvode");
        os << szError;
        throw ast::InternalError(os.str());
    }

    if (iflag != 1)
    {
        if (iflag == 0)
        {
            Scierror(999, _("%s: The collocation matrix is singular.\n"), "bvode");
        }
        else if (iflag == -1)
        {
            Scierror(999, _("%s: The expected no. of subintervals exceeds storage specifications.\n"), "bvode");
        }
        else if (iflag == -2)
        {
            Scierror(999, _("%s: The nonlinear iteration has not converged.\n"), "bvode");
        }
        else if (iflag == -3)
        {
            Scierror(999, _("%s: There is an input data error.\n"), "bvode");
        }

        FREE(iwork);
        FREE(rwork);
        FREE(M);
        FREE(ltol);
        DifferentialEquation::removeDifferentialEquationFunctions();

        return types::Function::Error;
    }

    types::Double* pDblRes = new types::Double(sumM, pDblXpts->getSize());
    double* res = (double*)MALLOC(pDblXpts->getSize() * sumM * sizeof(double));
    for (int i = 0; i < pDblXpts->getSize(); i++)
    {
        double val = pDblXpts->get(i);
        C2F(appsln)(&val, &res[i * sumM], rwork, iwork);
    }

    pDblRes->set(res);

    FREE(iwork);
    FREE(rwork);
    FREE(M);
    FREE(ltol);
    FREE(res);
    DifferentialEquation::removeDifferentialEquationFunctions();

    // *** Return result in Scilab. ***
    out.push_back(pDblRes);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

