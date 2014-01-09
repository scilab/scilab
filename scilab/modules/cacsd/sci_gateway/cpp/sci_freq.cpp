/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "cacsd_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

extern void C2F(dfrmg)( int*, int*, int*, int*, int*, int*, int*,
                        double*, double*, double*, double*, double*,
                        double*, double*, double*, double*, int*);

extern void C2F(dadd)(int*, double*, int*, double*, int*);
extern void C2F(horner)(double*, int*, double*, double*, double*, double*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue freqRational(types::typed_list &in, int _iRetCount, types::typed_list &out);
types::Function::ReturnValue freqState(types::typed_list &in, int _iRetCount, types::typed_list &out);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_freq(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblA = NULL;
    types::Double* pDblB = NULL;
    types::Double* pDblP = NULL;

    if (in.size() < 3 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "freq", 3, 5);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "freq", 1);
        return types::Function::Error;
    }

    if(in.size() == 3) // systeme defini par sa representation rationnelle n./d
    {
        return freqRational(in, _iRetCount, out);
    }
    else // systeme defini par sa representation d'etat
    {
        return freqState(in, _iRetCount, out);
    }
}

types::Function::ReturnValue freqRational(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRowNum     = 0;
    int iColNum     = 0;
    int iRowDen     = 0;
    int iColDen     = 0;
    int iSizeF      = 0;
    int iOne        = 1;
    int iComplex    = 0;
    int iError      = 0;
    double dZero    = 0;

    double** pdblDen    = NULL;
    double** pdblNum    = NULL;
    double* pdblF       = NULL;
    double* pdblFImg    = NULL;
    double* pdblR       = NULL;
    double* pdblI       = NULL;
    int* piRankDen      = NULL;
    int* piRankNum      = NULL;

    /*** get inputs arguments ***/
    // get f
    if(in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", 3);
        return types::Function::Error;
    }

    types::Double* pDblF = in[2]->getAs<types::Double>();
    iSizeF = pDblF->getSize();
    pdblF = pDblF->get();

    if(pDblF->isComplex())
    {
        pdblFImg = pDblF->getImg();
        iComplex = 1;
    }
    else
    {
        pdblFImg = &dZero;
    }

    try
    {
        // get DEN
        if(in[1]->isDouble())
        {
            types::Double* pDblDen = in[1]->getAs<types::Double>();
            double* pdbl = pDblDen->get();
            if(pDblDen->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 2);
                return types::Function::Error;
            }

            iRowDen = pDblDen->getRows();
            iColDen = pDblDen->getCols();

            piRankDen = new int[pDblDen->getSize()];
            memset(piRankDen, 0x00, pDblDen->getSize() * sizeof(int));

            pdblDen = new double*[pDblDen->getSize()];
            for(int i = 0; i < pDblDen->getSize(); i++)
            {
                pdblDen[i] = pdbl + i;
            }
        }
        else if(in[1]->isPoly())
        {
            types::Polynom* pPolyDen = in[1]->getAs<types::Polynom>();
            if(pPolyDen->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "freq", 2);
                return types::Function::Error;
            }

            iRowDen = pPolyDen->getRows();
            iColDen = pPolyDen->getCols();

            piRankDen = new int[pPolyDen->getSize()];
            pPolyDen->getRealRank(piRankDen);

            pdblDen = new double*[pPolyDen->getSize()];
            for(int i = 0; i < pPolyDen->getSize(); i++)
            {
                pdblDen[i] = pPolyDen->get(i)->getCoef()->get();
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "freq", 2);
            return types::Function::Error;
        }

        // get NUM
        if(in[0]->isDouble())
        {
            types::Double* pDblNum = in[0]->getAs<types::Double>();
            double* pdbl = pDblNum->get();
            if(pDblNum->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 1);
                throw 1;
            }

            iRowNum = pDblNum->getRows();
            iColNum = pDblNum->getCols();

            piRankNum = new int[pDblNum->getSize()];
            memset(piRankNum, 0x00, pDblNum->getSize() * sizeof(int));

            pdblNum = new double*[pDblNum->getSize()];
            for(int i = 0; i < pDblNum->getSize(); i++)
            {
                pdblNum[i] = pdbl + i;
            }
        }
        else if(in[0]->isPoly())
        {
            types::Polynom* pPolyNum = in[0]->getAs<types::Polynom>();
            if(pPolyNum->isComplex())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "freq", 1);
                throw 1;
            }

            iRowNum = pPolyNum->getRows();
            iColNum = pPolyNum->getCols();

            piRankNum = new int[pPolyNum->getSize()];
            pPolyNum->getRealRank(piRankNum);

            pdblNum = new double*[pPolyNum->getSize()];
            for(int i = 0; i < pPolyNum->getSize(); i++)
            {
                pdblNum[i] = pPolyNum->get(i)->getCoef()->get();
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix or polynom expected.\n"), "freq", 1);
            return types::Function::Error;
        }

        if(iRowNum != iRowDen || iColNum != iColDen)
        {
            Scierror(60, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "freq");
            throw 1;
        }

        /*** perform operations ***/
        double dVr  = 0;
        double dVi  = 0;
        double dUr  = 0;
        double dUi  = 0;
        int iSize   = iRowDen * iColDen * iSizeF;

        pdblR = new double[iSize];
        pdblI = new double[iSize];

        double* pdblRTemp = pdblR;
        double* pdblITemp = pdblI;

        for(int i = 0; i < iSizeF; i++)
        {
            for(int j = 0; j < iRowDen * iColDen; j++)
            {
                C2F(horner)(pdblNum[j], piRankNum + j, pdblF, pdblFImg, &dVr, &dVi);
                C2F(horner)(pdblDen[j], piRankDen + j, pdblF, pdblFImg, &dUr, &dUi);
                if(dUr * dUr + dUi * dUi == 0)
                {
                    Scierror(27, _("%s: Division by zero...\n"), "freq");
                    throw 1;
                }

                if(iComplex)
                {
                    C2F(wdiv)(&dVr, &dVi, &dUr, &dUi, pdblRTemp, pdblITemp);
                }
                else
                {
                    *pdblRTemp = dVr / dUr;
                }

                pdblRTemp++;
                pdblITemp++;
            }

            pdblF++;
            pdblFImg += iComplex;
        }

        /*** retrun output arguments ***/
        types::Double* pDblOut = new types::Double(iRowDen, iColDen * iSizeF, iComplex);
        double* pdblOut = pDblOut->get();
        int iSizeOut = pDblOut->getSize();
        C2F(dcopy)(&iSizeOut, pdblR, &iOne, pdblOut, &iOne);

        if(iComplex)
        {
            double* pdblOutImg = pDblOut->getImg();
            C2F(dcopy)(&iSizeOut, pdblI, &iOne, pdblOutImg, &iOne);
        }

        out.push_back(pDblOut);
    }
    catch(int iErr)
    {
        iError = iErr;
    }

    // free memory
    delete[] piRankDen;
    delete[] pdblDen;

    if(pdblR)
    {
        delete[] pdblR;
    }
    if(pdblI)
    {
        delete[] pdblI;
    }

    if(piRankNum)
    {
        delete[] piRankNum;
    }
    if(pdblNum)
    {
        delete[] pdblNum;
    }

    if(iError)
    {
        return types::Function::Error;
    }

    return types::Function::OK;
}

types::Function::ReturnValue freqState(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblA = NULL;
    types::Double* pDblB = NULL;
    types::Double* pDblC = NULL;
    types::Double* pDblD = NULL;
    types::Double* pDblF = NULL;

    double dZero = 0;

    int iRowsA      = 0;
    int iColsB      = 0;
    int iRowsC      = 0;
    int iSizeF      = 0;
    int iOne        = 1;
    int iComplex    = 0;

    double* pdblF       = NULL;
    double* pdblFImg    = NULL;

    /*** get inputs arguments ***/
    int iNbInputArg = in.size();
    // get f
    if(in[iNbInputArg-1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", iNbInputArg);
        return types::Function::Error;
    }

    pDblF = in[iNbInputArg-1]->getAs<types::Double>();
    pdblF = pDblF->get();
    if(pDblF->isComplex())
    {
        pdblFImg = pDblF->getImg();
        iComplex = 1;
    }
    else
    {
        pdblFImg = &dZero;
    }


    if(iNbInputArg == 5)
    {   //get D
        if(in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", 4);
            return types::Function::Error;
        }

        pDblD = in[3]->getAs<types::Double>();
        if(pDblD->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 4);
            return types::Function::Error;
        }
    }

    // get C
    if(in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", 3);
        return types::Function::Error;
    }

    pDblC = in[2]->getAs<types::Double>();

    if(pDblC->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 3);
        return types::Function::Error;
    }

    // get B
    if(in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", 2);
        return types::Function::Error;
    }

    pDblB = in[1]->getAs<types::Double>();

    if(pDblB->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 2);
        return types::Function::Error;
    }

    // get A
    if(in[0]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "freq", 1);
        return types::Function::Error;
    }

    pDblA = in[0]->getAs<types::Double>();

    if(pDblA->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "freq", 1);
        return types::Function::Error;
    }

    if(pDblA->getRows() != pDblA->getCols())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), "freq", 1);
        return types::Function::Error;
    }

    iRowsA = pDblA->getRows();
    iColsB = pDblB->getCols();
    iRowsC = pDblC->getRows();
    iSizeF = pDblF->getSize();

    if(iRowsA != pDblB->getRows() || iRowsA != pDblC->getCols())
    {
        Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "ppol");
        return types::Function::Error;
    }

    if(iNbInputArg == 5 && (pDblD->getRows() != pDblC->getRows() || pDblD->getCols() != pDblB->getCols()))
    {
        Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "ppol");
        return types::Function::Error;
    }

    /*** perform operations ***/
    int iJob        = 0;
    bool bFirst     = true;
    double dRcond   = 0;

    int* pdblW1     = new int[iRowsA];
    double* pdblW   = new double[2 * iRowsA * iRowsA + 2 * iRowsA];
    double* pdblWgr = new double[iColsB * iSizeF * iRowsC];
    double* pdblWgi = new double[iColsB * iSizeF * iRowsC];

    for(int i = 0; i < iSizeF; i++)
    {
        int ig = i * iColsB * iRowsC;
        C2F(dfrmg)( &iJob, &iRowsA, &iRowsA, &iRowsC, &iRowsC, &iColsB, &iRowsA,
                    pDblA->get(), pDblB->get(), pDblC->get(), pdblF,
                    pdblFImg, pdblWgr + ig, pdblWgi + ig, &dRcond, pdblW, pdblW1);

        if(bFirst && dRcond + 1 == 1)
        {
            char strValue[256];
            sprintf(strValue, "%lf", dRcond);
            sciprint(_("Warning :\n"));
            sciprint(_("matrix is close to singular or badly scaled. rcond = %s\n"), strValue);
            bFirst = false;
        }

        if(iNbInputArg == 5)
        {
            int iSize = iColsB * iRowsC;
            C2F(dadd)(&iSize, pDblD->get(), &iOne, pdblWgr + ig, &iOne);
        }

        pdblF++;
        pdblFImg += iComplex;
    }

    /*** retrun output arguments ***/
    types::Double* pDblOut  = new types::Double(iRowsC, iColsB * iSizeF, iComplex);
    double* pdblOutReal     = pDblOut->get();
    double* pdblOutImg      = pDblOut->getImg();
    int iSizeOut            = pDblOut->getSize();

    C2F(dcopy)(&iSizeOut, pdblWgr, &iOne, pdblOutReal, &iOne);
    if(iComplex)
    {
        C2F(dcopy)(&iSizeOut, pdblWgi, &iOne, pdblOutImg, &iOne);
    }

    // free memory
    delete[] pdblW;
    delete[] pdblW1;
    delete[] pdblWgr;
    delete[] pdblWgi;

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
