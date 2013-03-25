/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "prod.hxx"
extern "C"
{
#include "matrix_multiplication.h"
}

types::Double* prod(types::Double* pIn, int iOrientation)
{
    types::Double* pOut = NULL;
    double* pdblInReal  = pIn->getReal();
    double* pdblInImg   = pIn->getImg();

    if (iOrientation == 0) // all
    {
        double dblR = pdblInReal[0];
        if (pIn->isComplex())
        {
            double dblI = pdblInImg[0];
            double dblRTmp = 0;
            double dblITmp = 0;
            for (int i = 1 ; i < pIn->getSize() ; i++)
            {
                dblRTmp = dblR;
                dblITmp = dblI;
                iMultiComplexMatrixByComplexMatrix( &pdblInReal[i], &pdblInImg[i], 1, 1,
                                                    &dblRTmp, &dblITmp, 1, 1, &dblR, &dblI);
            }

            pOut = new types::Double(dblR, dblI);
        }
        else
        {
            for (int i = 1 ; i < pIn->getSize() ; i++)
            {
                dblR *= pdblInReal[i];
            }

            pOut = new types::Double(dblR);
        }
    }
    else // prod on one dimension
    {
        // create output dimensions
        int iDims   = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            piDims[i] = pIn->getDimsArray()[i];
        }

        piDims[iOrientation - 1] = 1;

        // create output variable
        pOut = new types::Double(iDims, piDims, pIn->isComplex());
        delete piDims;
        piDims = NULL;

        // init output
        double* pdblOut = pOut->get();
        double* pdblOutImg = pOut->getImg();

        if (pOut->isComplex())
        {
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                pdblOut[i] = 1;
                pdblOutImg[i] = 0;
            }
        }
        else
        {
            for (int i = 0 ; i < pOut->getSize() ; i++)
            {
                pdblOut[i] = 1;
            }
        }

        // perform operations
        int* piIndex = new int[iDims];
        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                double dblRTmp = pdblOut[iIndex];
                double dblITmp = pdblOutImg[iIndex];

                iMultiComplexMatrixByComplexMatrix( &pdblInReal[i], &pdblInImg[i], 1, 1,
                                                    &dblRTmp, &dblITmp, 1, 1,
                                                    &pdblOut[iIndex], &pdblOutImg[iIndex]);
            }
        }
        else
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);
                pdblOut[iIndex] *= pdblInReal[i];
            }
        }

        delete piIndex;
        piIndex = NULL;
    }

    return pOut;
}

// polynom
types::Polynom* prod(types::Polynom* pIn, int iOrientation)
{
    types::Polynom* pOut = NULL;
    if (iOrientation == 0) // sum of all element
    {
        // get rank of all input single poly
        int* piRanks = new int[pIn->getSize()];
        pIn->getRank(piRanks);

        // compute the output rank
        int iRankMax = 0;
        for (int i = 0; i < pIn->getSize(); i++)
        {
            iRankMax += piRanks[i];
        }

        // create output matrix of poly
        pOut = new types::Polynom(pIn->getVariableName(), 1, 1, &iRankMax);
        pOut->setComplex(pIn->isComplex());

        // do prod
        double* pdblRealOut = pOut->get(0)->getCoef()->getReal();
        if (pIn->isComplex())
        {
            double* pdblImgOut = pOut->get(0)->getCoef()->getImg();

            // alloc temporary workspace
            double* pdblTempReal = new double[iRankMax];
            double* pdblTempImg  = new double[iRankMax];

            // init output to 1 + 0s + 0s²...
            pdblRealOut[0] = 1;
            pdblImgOut[0]  = 0;
            for (int iRankN = 1; iRankN < iRankMax; iRankN++)
            {
                pdblRealOut[iRankN] = 0;
                pdblImgOut[iRankN]  = 0;
            }

            // perform operations
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* pdblRealIn = pIn->get(i)->getCoef()->getReal();
                double* pdblImgIn = pIn->get(i)->getCoef()->getImg();
                memcpy(pdblTempReal, pdblRealOut, iRankMax * sizeof(double));
                memcpy(pdblTempImg,  pdblImgOut,  iRankMax * sizeof(double));
                iMultiComplexPolyByComplexPoly(pdblTempReal, pdblTempImg, iRankMax,
                                               pdblRealIn, pdblImgIn, piRanks[i],
                                               pdblRealOut, pdblImgOut, iRankMax);
            }

            delete pdblTempReal;
            delete pdblTempImg;
        }
        else
        {
            // alloc temporary workspace
            double* pdblTempReal = new double[iRankMax];

            // init output to 1 + 0s + 0s²...
            pdblRealOut[0] = 1;
            for (int iRankN = 1; iRankN < iRankMax; iRankN++)
            {
                pdblRealOut[iRankN] = 0;
            }

            // perform operations
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* pdblRealIn = pIn->get(i)->getCoef()->getReal();
                memcpy(pdblTempReal, pdblRealOut, iRankMax * sizeof(double));
                iMultiRealPolyByRealPoly(pdblTempReal, iRankMax,
                                         pdblRealIn, piRanks[i],
                                         pdblRealOut, iRankMax);
            }

            delete pdblTempReal;
        }
    }
    else // sum following a dimension
    {
        // create output dimensions
        int iDims = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            piDims[i] = pIn->getDimsArray()[i];
        }

        piDims[iOrientation - 1] = 1;

        // get input ranks in int*
        int* piRanks = new int[pIn->getSize()];
        pIn->getRank(piRanks);

        // get input ranks in types::Double
        types::Double* pDblRanks = new types::Double(pIn->getDims(), pIn->getDimsArray());
        for (int i = 0; i < pDblRanks->getSize(); i++)
        {
            pDblRanks->set(i, static_cast<double>(piRanks[i]));
        }

        // create output max ranks
        types::Double* pDblRanksOut = new types::Double(iDims, piDims);
        double* pdblOut = pDblRanksOut->get();
        for (int i = 0; i < pDblRanksOut->getSize(); i++)
        {
            pdblOut[i] = 0;
        }

        // compute the maximum ranks for the dim n
        int*    piIndex = new int[iDims];
        double* pdblIn  = pDblRanks->get();
        for (int i = 0 ; i < pDblRanks->getSize() ; i++)
        {
            //get array of dim
            pDblRanks->getIndexes(i, piIndex);

            //convert indexes for result
            piIndex[iOrientation - 1] = 0;
            int iIndex = pDblRanksOut->getIndex(piIndex);
            pdblOut[iIndex] += pdblIn[i];
        }

        // move output ranks from types::Double to int*
        int* piRankMax = new int[pDblRanksOut->getSize()];
        int iMaxOutputRank = 0;
        for (int i = 0; i < pDblRanksOut->getSize(); i++)
        {
            piRankMax[i] = static_cast<int>(pdblOut[i]);
            iMaxOutputRank = std::max(iMaxOutputRank, piRankMax[i]);
        }

        // create the outpout polynom
        pOut = new types::Polynom(pIn->getVariableName(), iDims, piDims, piRankMax);
        pOut->setComplex(pIn->isComplex());

        if (pIn->isComplex())
        {
            // alloc temporary workspace
            double* pdblTempReal = new double[iMaxOutputRank];
            double* pdblTempImg  = new double[iMaxOutputRank];

            // init output to a matrix of 1 + 0s + 0s²...
            for (int i = 0; i < pOut->getSize(); i++)
            {
                double* pdblRealOut = pOut->get(i)->getCoef()->getReal();
                double* pdblImgOut = pOut->get(i)->getCoef()->getImg();
                pdblRealOut[0] = 1;
                pdblImgOut[0]  = 0;
                for (int iRankN = 1; iRankN < piRankMax[i]; iRankN++)
                {
                    pdblRealOut[iRankN] = 0;
                    pdblImgOut[iRankN]  = 0;
                }
            }

            // perform operations
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                // make sum on each ranks
                double* pdblRealIn = pIn->get(i)->getCoef()->getReal();
                double* pdblRealOut = pOut->get(iIndex)->getCoef()->getReal();
                double* pdblImgIn = pIn->get(i)->getCoef()->getImg();
                double* pdblImgOut = pOut->get(iIndex)->getCoef()->getImg();
                memcpy(pdblTempReal, pdblRealOut, piRankMax[iIndex] * sizeof(double));
                memcpy(pdblTempImg,  pdblImgOut,  piRankMax[iIndex] * sizeof(double));
                iMultiComplexPolyByComplexPoly(pdblTempReal, pdblTempImg, piRankMax[iIndex],
                                               pdblRealIn, pdblImgIn, piRanks[i],
                                               pdblRealOut, pdblImgOut, piRankMax[iIndex]);
            }

            delete pdblTempReal;
            delete pdblTempImg;
        }
        else
        {
            // alloc temporary workspace
            double* pdblTempReal = new double[iMaxOutputRank];
            double* pdblTempImg  = new double[iMaxOutputRank];

            // init output to a matrix of 1 + 0s + 0s²...
            for (int i = 0; i < pOut->getSize(); i++)
            {
                double* pdblRealOut = pOut->get(i)->getCoef()->getReal();
                pdblRealOut[0] = 1;
                for (int iRankN = 1; iRankN < piRankMax[i]; iRankN++)
                {
                    pdblRealOut[iRankN] = 0;
                }
            }

            // perform operations
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                // make sum on each ranks
                double* pdblRealIn = pIn->get(i)->getCoef()->getReal();
                double* pdblRealOut = pOut->get(iIndex)->getCoef()->getReal();
                memcpy(pdblTempReal, pdblRealOut, piRankMax[iIndex] * sizeof(double));
                iMultiRealPolyByRealPoly(pdblTempReal, piRankMax[iIndex],
                                         pdblRealIn, piRanks[i],
                                         pdblRealOut, piRankMax[iIndex]);
            }

            delete pdblTempReal;
        }
    }

    pOut->updateRank();
    return pOut;
}

