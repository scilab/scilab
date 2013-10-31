/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "optimization_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "list.hxx"
#include "optimizationfunctions.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
#include "common_structure_optimization.h"
#include "MALLOC.h"
#include "elem_common.h"
#include "scioptimfunctions.h"
#include "checkoptimerror.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_optim(types::typed_list &in, types::optional_list &opt, int _iRetCount, types::typed_list &out)
{
    types::Function::ReturnValue ret = types::Function::Error;
    OptimizationFunctions* opFunctionsManager = NULL;

    types::Double* pDblX0   = NULL;
    types::Double* pDblBinf = NULL;
    types::Double* pDblBsub = NULL;
    types::Double* pDblTi   = NULL;
    types::Double* pDblTd   = NULL;
    types::Double* pDblNap  = NULL;
    types::Double* pDblIter = NULL;
    types::Double* pDblEpsg = NULL;
    types::Double* pDblEpsf = NULL;
    types::Double* pDblEpsx = NULL;
    types::Double* pDblWork = NULL;
    //    types::Polynom* pPolyX0     = NULL;
    //    types::Polynom* pPolyBinf   = NULL;
    //    types::Polynom* pPolyBsub   = NULL;

    int* piIzs          = NULL;
    int* piWork         = NULL;
    float* pfRzs        = NULL;
    double* pdblDzs     = NULL;
    double* pdblWork    = NULL;
    double* pdblWork2   = NULL;
    double* pdblX0      = NULL;
    double* pdblEpsx    = NULL;
    double* pdblG       = NULL;
    double* pdblVar     = NULL;
    double* pdblBsub    = NULL;
    double* pdblBinf    = NULL;

    bool bMem       = false;
    int iEpsx       = 1;
    int iDzs        = 1;
    int iIzs        = 1;
    int iPos        = 0;
    int iContr      = 1;
    int iSizeX0     = 0;
    int iSizeBinf   = 0;
    int iSizeBsub   = 0;
    int iSim        = 0; // 1 : c function || 2 : macro
    int iAlgo       = 1; // 1 : qn || 2 : gc || 10 : nd
    int iMem        = 0;
    int iGetArgs    = 0;
    int iIndSim     = 0;
    int iIndOpt     = 0;
    int iSaveI      = 0;
    int iSaveD      = 0;
    int iArret      = 0;
    int iMode       = 1;
    int iWorkSize   = 0;
    int iWorkSizeI  = 0;
    int iNitv       = 0;
    int io          = 0; // not used in scilab 6 and more
    int iImp        = 0;
    int iZero       = 0;
    int iOne        = 1;
    double df0      = 1;
    double dF       = 0;

    // stop arguments "ar"
    int iNap        = 100;
    int iItMax      = 100;
    double dEpsg    = C2F(dlamch)((char*)"p", 1L); // p : eps*base
    double dTol     = dEpsg;
    double dEpsf    = 0;

    try
    {
        if (in.size() < 2 || in.size() > 18)
        {
            Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "optim", 2, 18);
            throw ScilabMessage();
        }

        if (_iRetCount > 6)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "optim", 1, 6);
            throw ScilabMessage();
        }

        /*** get inputs arguments ***/
        // get imp
        if (opt.size() == 1 && opt[0].first == L"imp")
        {
            if (opt[0].second->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: A scalar expected.\n"), "optim", "impl");
                throw ScilabMessage();
            }

            types::Double* pDblImpl = opt[0].second->getAs<types::Double>();

            if (pDblImpl->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%s: A scalar expected.\n"), "optim", "impl");
                throw ScilabMessage();
            }

            iImp = (int)pDblImpl->get(0);
        }

        // get costf
        opFunctionsManager = new OptimizationFunctions(L"optim");
        Optimization::addOptimizationFunctions(opFunctionsManager);

        if (in[iPos]->isCallable())
        {
            types::Callable* pCall = in[iPos]->getAs<types::Callable>();
            opFunctionsManager->setOptimCostfFunction(pCall);
            iSim = 2;
        }
        else if (in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            char* pst = wide_string_to_UTF8(pStr->get(0));
            bool bOK = opFunctionsManager->setOptimCostfFunction(pStr);
            iSim = 1;

            if (bOK == false)
            {
                Scierror(50, _("%s: Subroutine not found: %s\n"), "optim", pst);
                FREE(pst);
                throw ScilabMessage();
            }

            memcpy(C2F(optim).nomsub, pst, Max(6, strlen(pst)));
            FREE(pst);
        }
        else if (in[iPos]->isList())
        {
            types::List* pList = in[iPos]->getAs<types::List>();
            if (pList->getSize() == 0)
            {
                Scierror(50, _("%s: Argument #%d: Subroutine not found in list: %s\n"), "optim", iPos + 1, "(string empty)");
                throw ScilabMessage();
            }

            if (pList->get(0)->isString())
            {
                types::String* pStr = pList->get(0)->getAs<types::String>();
                char* pst = wide_string_to_UTF8(pStr->get(0));
                bool bOK = opFunctionsManager->setOptimCostfFunction(pStr);
                iSim = 1;

                if (bOK == false)
                {
                    Scierror(50, _("%s: Subroutine not found: %s\n"), "optim", pst);
                    FREE(pst);
                    throw ScilabMessage();
                }

                memcpy(C2F(optim).nomsub, pst, Max(6, strlen(pst)));
                FREE(pst);
            }
            else if (pList->get(0)->isCallable())
            {
                types::Callable* pCall = pList->get(0)->getAs<types::Callable>();
                opFunctionsManager->setOptimCostfFunction(pCall);
                iSim = 2;
                for (int iter = 1; iter < pList->getSize(); iter++)
                {
                    opFunctionsManager->setCostfArgs(pList->get(iter)->getAs<types::InternalType>());
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: The first argument in the list must be a string or a function.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or a function expected.\n"), "optim", iPos + 1);
            throw ScilabMessage();
        }

        iPos++;

        // if contr, get binf and bsup
        if (in[iPos]->isString())
        {
            types::String* pStrContr = in[iPos]->getAs<types::String>();
            if (pStrContr->isScalar() == false || wcscmp(pStrContr->get(0), L"b"))
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String \"b\" expected.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            if (in.size() < 5)
            {
                Scierror(77, _("%s: Wrong number of input argument(s): %d or more expected.\n"), "optim", 5);
                throw ScilabMessage();
            }

            iPos++;

            if (in[iPos]->isDouble())
            {
                pDblBinf = in[iPos]->getAs<types::Double>();
                iSizeBinf = pDblBinf->getSize();
                pdblBinf = pDblBinf->get();
            }
            //        else if(in[iPos]->isPoly())
            //        {
            //            pPolyBinf = in[iPos]->getAs<types::Polynom>();
            //            iSizeBinf = pPolyBinf->getSize();
            //        }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            iPos++;

            if (in[iPos]->isDouble())
            {
                pDblBsub = in[iPos]->getAs<types::Double>();
                iSizeBsub = pDblBsub->getSize();
                pdblBsub = pDblBsub->get();
            }
            //        else if(in[iPos]->isPoly())
            //        {
            //            pPolyBsub = in[iPos]->getAs<types::Polynom>();
            //            iSizeBsub = pPolyBsub->getSize();
            //        }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            iContr = 2;
            iPos++;
        }

        // get x0
        if (in[iPos]->isDouble())
        {
            pDblX0 = in[iPos]->getAs<types::Double>();
            iSizeX0 = pDblX0->getSize();
            if (pDblX0->isComplex())
            {
                iSizeX0 *= 2;
                pdblX0 = new double[iSizeX0];
                memcpy(pdblX0, pDblX0->get(), pDblX0->getSize() * sizeof(double));
                memcpy(pdblX0 + pDblX0->getSize(), pDblX0->getImg(), pDblX0->getSize() * sizeof(double));
            }
            else
            {
                pdblX0 = new double[iSizeX0];
                memcpy(pdblX0, pDblX0->get(), pDblX0->getSize() * sizeof(double));
            }

            opFunctionsManager->setXRows(pDblX0->getRows());
            opFunctionsManager->setXCols(pDblX0->getCols());
        }
        //    else if(in[iPos]->isPoly())
        //    {
        //
        //    }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "optim", iPos + 1);
            throw ScilabMessage();
        }

        if (iContr == 2 && (iSizeX0 != iSizeBinf || iSizeX0 != iSizeBsub))
        {
            Scierror(999, _("%s: Bounds and initial guess are incompatible.\n"), "optim");
            throw ScilabMessage();
        }

        // alloc g output data
        pdblG = new double[iSizeX0];

        iPos++;

        // get algo
        if (iPos < in.size() && in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            if (pStr->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: Scalar string expected.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            if (wcscmp(pStr->get(0), L"qn") == 0) // default case
            {
                iAlgo = 1;
                iPos++;
            }
            else if (wcscmp(pStr->get(0), L"gc") == 0)
            {
                iAlgo = 2;
                iPos++;
            }
            else if (wcscmp(pStr->get(0), L"nd") == 0)
            {
                iAlgo = 10;
                iPos++;
            }
            //        else
            //        {
            //            Scierror(999, _("%s: Wrong value for input argument #%d: \"qn\", \"gc\", \"nd\" expected.\n"), "optim", iPos + 1);
            //            throw ScilabMessage();
            //        }
        }

        // get df0 and mem
        if (iPos < in.size() && in[iPos]->isDouble() && in[iPos]->getAs<types::Double>()->isScalar())
        {
            df0 = in[iPos]->getAs<types::Double>()->get(0);
            iPos++;

            // get mem
            if (iPos < in.size() && iContr == 1 && (iAlgo == 2 || iAlgo == 10) && in[iPos]->isDouble())
            {
                types::Double* pDbl = in[iPos]->getAs<types::Double>();
                if (in[iPos]->isDouble() && pDbl->isScalar() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "optim", iPos + 1);
                    throw ScilabMessage();
                }

                iMem = (int)pDbl->get(0);
                bMem = true;
                iPos++;
            }
        }

        // management of work table
        if (iAlgo == 1)
        {
            // compute size
            if (iContr == 1)
            {
                iWorkSize   = iSizeX0 * (iSizeX0 + 13) / 2;
                iWorkSizeI  = 0;
            }
            else // iContr == 2
            {
                iWorkSize   = iSizeX0 * (iSizeX0 + 1) / 2 + 4 * iSizeX0 + 1;
                iWorkSizeI  = 2 * iSizeX0;
            }

            // alloc data
            pdblWork = new double[iWorkSize];

            if (iContr == 2)
            {
                piWork = new int[iWorkSizeI];
            }
        }

        // get work
        if (iPos < in.size() && in[iPos]->isDouble())
        {
            if (iAlgo != 1)
            {
                Scierror(137, _("%s: NO hot restart available in this method.\n"), "optim");
                throw ScilabMessage();
            }

            pDblWork = in[iPos]->getAs<types::Double>();

            if (pDblWork->getSize() != iWorkSize + iWorkSizeI)
            {
                Scierror(137, _("Hot restart: dimension of working table (argument n:%d).\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            double* pdbl = pDblWork->get();
            if (iContr == 1)
            {
                C2F(dcopy)(&iWorkSize, pdbl, &iOne, pdblWork, &iOne);
            }
            else
            {
                C2F(dcopy)(&iWorkSize, pdbl, &iOne, pdblWork, &iOne);

                for (int i = 0; i < iWorkSizeI; i++)
                {
                    piWork[i] = (int)pdbl[i];
                }
            }

            iMode = 3;
            iPos++;
        }

        // get stop
        while (iPos < in.size() && in[iPos]->isString())
        {
            types::String* pStr = in[iPos]->getAs<types::String>();
            if (wcscmp(pStr->get(0), L"ar") == 0)
            {
                iPos++;
                for (int i = iPos; i < in.size(); i++)
                {
                    // get nap, iter, epsg, apsf, epsx
                    if (in[i]->isDouble())
                    {
                        if (pDblNap == NULL)
                        {
                            pDblNap = in[i]->getAs<types::Double>();
                            if (pDblNap->isScalar() == false || pDblNap->isComplex())
                            {
                                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optim", i + 1);
                                throw ScilabMessage();
                            }

                            iNap = (int)pDblNap->get(0);
                        }
                        else if (pDblIter == NULL)
                        {
                            pDblIter = in[i]->getAs<types::Double>();
                            if (pDblIter->isScalar() == false || pDblIter->isComplex())
                            {
                                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optim", i + 1);
                                throw ScilabMessage();
                            }

                            iItMax = (int)pDblIter->get(0);
                        }
                        else if (pDblEpsg == NULL)
                        {
                            pDblEpsg = in[i]->getAs<types::Double>();
                            if (pDblEpsg->isScalar() == false || pDblEpsg->isComplex())
                            {
                                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optim", i + 1);
                                throw ScilabMessage();
                            }

                            dEpsg = pDblEpsg->get(0);
                        }
                        else if (pDblEpsf == NULL)
                        {
                            pDblEpsf = in[i]->getAs<types::Double>();
                            if (pDblEpsf->isScalar() == false || pDblEpsf->isComplex())
                            {
                                Scierror(999, _("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optim", i + 1);
                                throw ScilabMessage();
                            }

                            dEpsf = pDblEpsf->get(0);
                        }
                        else if (pDblEpsx == NULL)
                        {
                            pDblEpsx = in[i]->getAs<types::Double>();
                            if (pDblEpsx->getSize() != iSizeX0)
                            {
                                Scierror(999, _("%s: Wrong value for input argument #%d: Incorrect stopping parameters.\n"), "optim", i + 1);
                                throw ScilabMessage();
                            }

                            iEpsx = 0;
                            pdblEpsx = pDblEpsx->get();
                        }
                        else
                        {
                            Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "optim", i + 1);
                            throw ScilabMessage();
                        }
                    }
                    else if (in[i]->isString())
                    {
                        iPos = i - 1;
                        break;
                    }
                    else
                    {
                        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar of a string expected.\n"), "optim", i + 1);
                        throw ScilabMessage();
                    }
                }
            }
            else if (wcscmp(pStr->get(0), L"in") == 0)
            {
                if (iSim != 1)
                {
                    Scierror(999, _("%s: \"in\" not allowed with simulator defined by a function.\n"), "optim");
                    throw ScilabMessage();
                }

                iIndSim = 10;
                costf(&iIndSim, &iSizeX0, pDblX0->get(), &dF, pdblG, NULL, NULL, NULL);

                if (iIndSim == 0)
                {
                    Scierror(131, _("%s: Stop requested by simulator (ind=0).\n"), "optim");
                    throw ScilabMessage();
                }
                else if (iIndSim < 0)
                {
                    Scierror(134, _("%s: Problem with initial constants in simul.\n"), "optim");
                    throw ScilabMessage();
                }

                piIzs   = new int[C2F(nird).nizs];
                pfRzs   = new float[C2F(nird).nrzs];
                pdblDzs = new double[C2F(nird).ndzs];

                iIndSim = 11;
                costf(&iIndSim, &iSizeX0, pDblX0->get(), &dF, pdblG, piIzs, pfRzs, pdblDzs);

                if (iIndSim == 0)
                {
                    Scierror(131, _("%s: Stop requested by simulator (ind=0).\n"), "optim");
                    throw ScilabMessage();
                }
                else if (iIndSim < 0)
                {
                    Scierror(134, _("%s: Problem with initial constants in simul.\n"), "optim");
                    throw ScilabMessage();
                }
            }
            else if (wcscmp(pStr->get(0), L"ti") == 0)
            {
                iPos++;
                if (in[iPos]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "optim", iPos + 1);
                    throw ScilabMessage();
                }

                pDblTi = in[iPos]->getAs<types::Double>();
                C2F(nird).nizs = pDblTi->getSize();
                piIzs = new int[pDblTi->getSize()];

                for (int i = 0; i < pDblTi->getSize(); i++)
                {
                    piIzs[i] = (int)pDblTi->get(i);
                }

                iIzs = 0;
            }
            else if (wcscmp(pStr->get(0), L"td") == 0)
            {
                iPos++;
                if (in[iPos]->isDouble() == false)
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "optim", iPos + 1);
                    throw ScilabMessage();
                }

                pDblTd = in[iPos]->getAs<types::Double>();
                C2F(nird).ndzs = pDblTd->getSize();
                pdblDzs = pDblTd->get();
                iDzs = 0;
            }
            else if (wcscmp(pStr->get(0), L"si") == 0)
            {
                iSaveI = 1;
            }
            else if (wcscmp(pStr->get(0), L"sd") == 0)
            {
                iSaveD = 1;
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: \"ar\", \"in\", \"ti\" or \"td\" not allowed.\n"), "optim", iPos + 1);
                throw ScilabMessage();
            }

            iPos++;
        }

        // initialisation eventuelle de f et g
        if (iNap < 2 || iItMax < 1)
        {
            iArret = 1;
        }

        if (iContr == 1 && (iAlgo == 2 || iAlgo == 10) ||
                (iContr == 2 && iAlgo == 1 && pdblWork) ||
                (iArret == 1))
        {
            iIndSim = 4;
            costf(&iIndSim, &iSizeX0, pDblX0->get(), &dF, pdblG, piIzs, pfRzs, pdblDzs);

            if (iIndSim == 0)
            {
                Scierror(131, _("%s: Stop requested by simulator (ind=0).\n"), "optim");
                throw ScilabMessage();
            }
            else if (iIndSim < 0)
            {
                Scierror(134, _("%s: Problem with initial constants in simul.\n"), "optim");
                throw ScilabMessage();
            }

            if (iNap < 2 || iItMax < 1)
            {
                // skip perform operation part
                iContr = 3;
            }
        }

        /*** perform operations ***/
        // n1qn1 : Quasi-Newton without constraints
        if (iContr == 1 && iAlgo == 1) // bounds not setted && algo qn
        {
            pdblVar = new double[iSizeX0];
            for (int i = 0; i < iSizeX0; i++)
            {
                pdblVar[i] = 0.10;
            }

            int iItmax1   = iItMax;
            int iNap1     = iNap;
            double dEpsg1 = dEpsg;

            C2F(n1qn1)(costf, &iSizeX0, pdblX0, &dF, pdblG,
                       pdblVar, &dEpsg, &iMode, &iItMax, &iNap, &iImp, &io, pdblWork,
                       piIzs, pfRzs, pdblDzs);

            dEpsg = sqrt(dEpsg);

            iIndOpt = 9;
            if (dEpsg1 >= dEpsg)
            {
                iIndOpt = 1;
            }
            else if (iNap >= iNap1)
            {
                iIndOpt = 4;
            }
            else if (iItMax >= iItmax1)
            {
                iIndOpt = 5;
            }

            if (checkOptimError(iArret, iIndOpt, iImp, dEpsg))
            {
                throw ScilabMessage();
            }
        }
        // algorithme n1qn3 : Gradient Conjugate without constraints
        else if (iContr == 1 && iAlgo == 2) // bounds not setted && algo gc
        {
            if (bMem == false)
            {
                iMem = 10;
            }

            // compute epsrel
            double dZng = 0;
            for (int i = 0; i < iSizeX0; i++)
            {
                dZng += (pdblG[i] * pdblG[i]);
            }

            dZng = sqrt(dZng);

            if (dZng > 0)
            {
                dEpsg /= dZng;
            }

            // compute dxmin
            double dxmin = dEpsg;
            if (iEpsx == 0)
            {
                dxmin = pdblEpsx[0];
                if (iSizeX0 > 1)
                {
                    for (int i = 0; i < iSizeX0; i++)
                    {
                        dxmin = Min(dxmin, pdblEpsx[i]);
                    }
                }
            }

            // work table
            iWorkSize = 4 * iSizeX0 + iMem * (2 * iSizeX0 + 1);
            pdblWork = new double[iWorkSize];

            iIndSim = 4;
            costf(&iIndSim, &iSizeX0, pDblX0->get(), &dF, pdblG, piIzs, pfRzs, pdblDzs);
            C2F(n1qn3)( costf, C2F(fuclid), C2F(ctonb), C2F(ctcab), &iSizeX0, pdblX0,
                        &dF, pdblG, &dxmin, &df0, &dEpsg, &iImp, &io, &iMode, &iItMax,
                        &iNap, pdblWork, &iWorkSize, piIzs, pfRzs, pdblDzs);

            switch (iMode)
            {
                case 0 :
                    iIndOpt = 0;
                    break;
                case 1 :
                    iIndOpt = 1;
                    break;
                case 2 :
                    iIndOpt = -10;
                    break;
                case 7 :
                case 3 :
                    iIndOpt = 7;
                    break;
                case 4 :
                    iIndOpt = 5;
                    break;
                case 5 :
                    iIndOpt = 4;
                    break;
                case 6 :
                    iIndOpt = 3;
                    break;
                default :
                    iIndOpt = 9;
            }

            if (checkOptimError(iArret, iIndOpt, iImp, dEpsg))
            {
                throw ScilabMessage();
            }

        }
        // optimiseur n1fc1 : non smooth without constraints
        else if (iContr == 1 && iAlgo == 10) // bounds not setted && algo nd
        {
            if (bMem == false)
            {
                iMem = 10;
            }

            int iNitv   = 2 * iMem + 2;
            int iNtv1   = 5 * iSizeX0 + (iSizeX0 + 4) * iMem;
            int iNtv2   = (iMem + 9) * iMem + 8;

            piWork      = new int[iNitv];
            pdblWork    = new double[iNtv1];
            pdblWork2   = new double[iNtv2];

            if (iEpsx == 1)
            {
                pdblEpsx = new double[iSizeX0];
                C2F(dcopy)(&iSizeX0, &dTol, &iZero, pdblEpsx, &iOne);
            }

            C2F(n1fc1)(costf, C2F(fuclid), &iSizeX0, pdblX0, &dF, pdblG,
                       pdblEpsx, &df0, &dEpsf, &dTol, &iImp, &io, &iMode,
                       &iItMax, &iNap, &iMem, piWork, pdblWork, pdblWork2,
                       piIzs, pfRzs, pdblDzs);

            switch (iMode)
            {
                case 0 :
                    iIndOpt = 0;
                    break;
                case 1 :
                    iIndOpt = 2;
                    break;
                case 2 :
                    iIndOpt = -10;
                    break;
                case 4 :
                    iIndOpt = 5;
                    break;
                case 5 :
                    iIndOpt = 4;
                    break;
                case 6 :
                    iIndOpt = 3;
                    break;
                default :
                    iIndOpt = 9;
            }

            if (checkOptimError(iArret, iIndOpt, iImp, dEpsg))
            {
                throw ScilabMessage();
            }
        }
        // optimiseur qnbd : quasi-newton with bound constraints
        else if (iContr == 2 && iAlgo == 1) // bounds setted && algo qn
        {
            int iNfac = 0;
            if (iEpsx == 1)
            {
                pdblEpsx = new double[iSizeX0];
                C2F(dcopy)(&iSizeX0, &dTol, &iZero, pdblEpsx, &iOne);
            }

            iIndOpt = 1 + pDblWork ? 1 : 0;
            C2F(qnbd)(&iIndOpt, costf, &iSizeX0, pdblX0, &dF, pdblG, &iImp, &io, &dTol, &iNap,
                      &iItMax, &dEpsf, &dEpsg, pdblEpsx, &df0, pdblBinf, pdblBsub,
                      &iNfac, pdblWork, &iWorkSize, piWork, &iWorkSizeI, piIzs, pfRzs, pdblDzs);

            if (checkOptimError(iArret, iIndOpt, iImp, dEpsg))
            {
                throw ScilabMessage();
            }
        }
        // optimiseur gcbd : Gradient Conjugate with bound constraints
        else if (iContr == 2 && iAlgo == 2) // bounds setted && algo gc
        {
            int iNfac = 0;
            int nt = 2;
            if (bMem)
            {
                nt = Max(1, iMem / 3);
            }

            iWorkSize   = iSizeX0 * (5 + 3 * nt) + 2 * nt + 1;
            iWorkSizeI  = iSizeX0 + nt + 1;
            pdblWork    = new double[iWorkSize];
            piWork      = new int[iWorkSizeI];

            if (iEpsx == 1)
            {
                pdblEpsx = new double[iSizeX0];
                C2F(dcopy)(&iSizeX0, &dTol, &iZero, pdblEpsx, &iOne);
            }

            iIndOpt = 1;
            C2F(gcbd)(&iIndOpt, costf, C2F(optim).nomsub, &iSizeX0, pdblX0, &dF, pdblG,
                      &iImp, &io, &dTol, &iNap, &iItMax, &dEpsf, &dEpsg, pdblEpsx, &df0,
                      pdblBinf, pdblBsub, &iNfac, pdblWork, &iWorkSize, piWork, &iWorkSizeI,
                      piIzs, pfRzs, pdblDzs);

            if (checkOptimError(iArret, iIndOpt, iImp, dEpsg))
            {
                throw ScilabMessage();
            }
        }
        else if (iContr != 3) // bad algo
        {
            Scierror(136, _("%s: This method is NOT implemented.\n"), "optim");
            throw ScilabMessage();
        }

        /*** return output arguments ***/
        int iRetCount1 = _iRetCount - iSaveI - iSaveD;
        if (iRetCount1 == 0)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "optim", iSaveI + iSaveD, iSaveI + iSaveD + 1);
            throw ScilabMessage();
        }

        // return f
        out.push_back(new types::Double(dF));

        // return x
        if (iRetCount1 >= 2)
        {
            if (pDblX0)
            {
                types::Double* pDbl = new types::Double(pDblX0->getDims(), pDblX0->getDimsArray(), pDblX0->isComplex());
                double* pdblReal = pDbl->get();
                memcpy(pdblReal, pdblX0, pDbl->getSize() * sizeof(double));

                if (pDbl->isComplex())
                {
                    double* pdblImg = pDbl->getImg();
                    memcpy(pdblImg, pdblX0 + pDbl->getSize(), pDbl->getSize() * sizeof(double));
                }

                out.push_back(pDbl);
            }
            //        else // if (pPolyX0)
            //        {
            //
            //        }
        }

        // return g
        if (iRetCount1 >= 3)
        {
            if (pdblG)
            {
                types::Double* pDbl = new types::Double(pDblX0->getDims(), pDblX0->getDimsArray(), pDblX0->isComplex());
                double* pdblReal = pDbl->get();
                memcpy(pdblReal, pdblG, pDbl->getSize() * sizeof(double));

                if (pDbl->isComplex())
                {
                    double* pdblImg = pDbl->getImg();
                    memcpy(pdblImg, pdblG + pDbl->getSize(), pDbl->getSize() * sizeof(double));
                }

                out.push_back(pDbl);
            }
            //        else // if (pPolyX0)
            //        {
            //
            //        }
        }

        // return work
        if (iRetCount1 >= 4)
        {
            if (iAlgo != 1)
            {
                Scierror(137, _("%s: NO hot restart available in this method.\n"), "optim");
                throw ScilabMessage();
            }

            if (iContr == 1)
            {
                types::Double* pDbl = new types::Double(1, iWorkSize);
                double* pdbl = pDbl->get();
                C2F(dcopy)(&iWorkSize, pdblWork, &iOne, pdbl, &iOne);
                out.push_back(pDbl);
            }
            else if (iContr == 2)
            {
                types::Double* pDbl = new types::Double(1, iWorkSize + iWorkSizeI);
                double* pdbl = pDbl->get();
                C2F(dcopy)(&iWorkSize, pdblWork, &iOne, pdbl, &iOne);
                for (int i = iWorkSize; i < iWorkSize + iWorkSizeI; i++)
                {
                    pdbl[i] = (double)(piWork[i]);
                }

                out.push_back(pDbl);
            }
            else // iContr == 3
            {
                out.push_back(pDblWork->clone());
            }
        }

        if (iRetCount1 >= 5)
        {
            out.push_back(new types::Double((double)iItMax));
        }

        if (iRetCount1 >= 6)
        {
            out.push_back(new types::Double((double)iNap));
        }

        if (iRetCount1 >= 7)
        {
            out.push_back(new types::Double((double)iIndOpt));
        }

        if (iSaveI)
        {
            if (C2F(nird).nizs == 0)
            {
                out.push_back(types::Double::Empty());
            }
            else
            {
                types::Double* pDbl = new types::Double(1, C2F(nird).nizs);
                double* pdbl = pDbl->get();
                for (int i = 0; i < C2F(nird).nizs; i++)
                {
                    pdbl[i] = (double)piIzs[i];
                }

                out.push_back(pDbl);
            }
        }

        if (iSaveD)
        {
            if (C2F(nird).ndzs == 0)
            {
                out.push_back(types::Double::Empty());
            }
            else
            {
                types::Double* pDbl = new types::Double(1, C2F(nird).ndzs);
                memcpy(pDbl->get(), pdblDzs, C2F(nird).ndzs * sizeof(double));
                out.push_back(pDbl);
            }
        }

        ret = types::Function::OK;
    }
    catch (ScilabError &e)
    {
        char* pstrMsg = wide_string_to_UTF8(e.GetErrorMessage().c_str());
        sciprint(_("%s: exception caught in '%s' subroutine.\n"), "optim", "costf");
        Scierror(999, pstrMsg);
        FREE(pstrMsg);
    }
    catch (ScilabMessage &m)
    {
    }

    /*** free memory ***/
    if (opFunctionsManager)
    {
        Optimization::removeOptimizationFunctions();
    }

    if (piIzs && iIzs)
    {
        delete piIzs;
    }

    if (pfRzs)
    {
        delete pfRzs;
    }

    if (pdblG)
    {
        delete pdblG;
    }

    if (pdblDzs && iDzs)
    {
        delete pdblDzs;
    }

    if (pdblWork)
    {
        delete pdblWork;
    }

    if (pdblWork2)
    {
        delete pdblWork2;
    }

    if (piWork)
    {
        delete piWork;
    }

    if (pdblX0)
    {
        delete pdblX0;
    }

    if (pdblVar)
    {
        delete pdblVar;
    }

    if (pdblEpsx && iEpsx)
    {
        delete pdblEpsx;
    }

    return ret;
}

