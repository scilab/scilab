/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include "sum.hxx"

types::Double* sum(types::Double* pIn, int iOrientation)
{
    types::Double* pOut = NULL;
    double* pdblInReal  = pIn->getReal();
    double* pdblInImg   = pIn->getImg();

    if (iOrientation == 0) // all
    {
        double dblR = 0;
        double dblI = 0;

        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                dblR += pdblInReal[i];
                dblI += pdblInImg[i];
            }

            pOut = new types::Double(dblR, dblI);
        }
        else
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                dblR += pdblInReal[i];
            }

            pOut = new types::Double(dblR);
        }
    }
    else // sum on one dimension
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
        // init output
        pOut->setZeros();

        delete[] piDims;
        piDims = NULL;

        // perform operations
        double* pdblOut = pOut->get();
        double* pdblOutImg = NULL;
        int* piIndex = new int[iDims];

        if (pIn->isComplex())
        {
            pdblOutImg = pOut->getImg();
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);
                pdblOutImg[iIndex]  += pdblInImg[i];
                pdblOut[iIndex]     += pdblInReal[i];
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
                pdblOut[iIndex] += pdblInReal[i];
            }
        }

        delete[] piIndex;
        piIndex = NULL;
    }

    return pOut;
}

// polynom
types::Polynom* sum(types::Polynom* pIn, int iOrientation)
{
    types::Polynom* pOut = NULL;
    if (iOrientation == 0) // sum of all element
    {
        // get rank of all input single poly
        int* piRanks = new int[pIn->getSize()];
        pIn->getRank(piRanks);

        // create output matrix of poly
        int iRankMax = pIn->getMaxRank();
        pOut = new types::Polynom(pIn->getVariableName(), 1, 1, &iRankMax);
        pOut->setComplex(pIn->isComplex());
        pOut->setZeros();

        // do sum
        double* dblRealOut = pOut->get(0)->get();
        if (pIn->isComplex())
        {
            double* dblImgOut = pOut->get(0)->get();
            // perform operations
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* dblRealIn = pIn->get(i)->get();
                double* dblImgIn = pIn->get(i)->getImg();
                for (int iRankN = 0; iRankN < piRanks[i] + 1; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                    dblImgOut[iRankN]  += dblImgIn[iRankN];
                }
            }
        }
        else
        {
            // perform operations
            for (int i = 0; i < pIn->getSize(); i++)
            {
                double* dblRealIn = pIn->get(i)->get();
                for (int iRankN = 0; iRankN < piRanks[i] + 1; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                }
            }
        }

        delete[] piRanks;
    }
    else // sum on one dimension
    {
        // create output dimensions
        int iDims = pIn->getDims();
        int* piDims = new int[iDims];

        for (int i = 0 ; i < iDims ; i++)
        {
            piDims[i] = pIn->getDimsArray()[i];
        }

        piDims[iOrientation - 1] = 1;

        // get ranks of all polynom
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
        pDblRanksOut->setZeros();

        // compute the maximum ranks for the dim n
        int*    piIndex = new int[iDims];
        double* pdblIn  = pDblRanks->get();
        double* pdblOut = pDblRanksOut->get();
        for (int i = 0 ; i < pDblRanks->getSize() ; i++)
        {
            //get array of dim
            pDblRanks->getIndexes(i, piIndex);

            //convert indexes for result
            piIndex[iOrientation - 1] = 0;
            int iIndex = pDblRanksOut->getIndex(piIndex);
            pdblOut[iIndex] = std::max(pdblOut[iIndex], pdblIn[i]);
        }

        pDblRanks->killMe();

        // move output ranks from types::Double to int*
        int* piRankMax = new int[pDblRanksOut->getSize()];
        for (int i = 0; i < pDblRanksOut->getSize(); i++)
        {
            piRankMax[i] = static_cast<int>(pdblOut[i]);
        }

        pDblRanksOut->killMe();

        // create the outpout polynom
        pOut = new types::Polynom(pIn->getVariableName(), iDims, piDims, piRankMax);
        delete[] piRankMax;
        pOut->setComplex(pIn->isComplex());
        pOut->setZeros();

        if (pIn->isComplex())
        {
            for (int i = 0 ; i < pIn->getSize() ; i++)
            {
                //get array of dim
                pIn->getIndexes(i, piIndex);

                //convert indexes for result
                piIndex[iOrientation - 1] = 0;
                int iIndex = pOut->getIndex(piIndex);

                // make the sum for each ranks
                double* dblRealIn = pIn->get(i)->get();
                double* dblRealOut = pOut->get(iIndex)->get();
                double* dblImgIn = pIn->get(i)->getImg();
                double* dblImgOut = pOut->get(iIndex)->getImg();
                for (int iRankN = 0; iRankN < piRanks[i] + 1; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                    dblImgOut[iRankN]  += dblImgIn[iRankN];
                }
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

                // make sum on each ranks
                double* dblRealIn = pIn->get(i)->get();
                double* dblRealOut = pOut->get(iIndex)->get();
                for (int iRankN = 0; iRankN < piRanks[i] + 1; iRankN++)
                {
                    dblRealOut[iRankN] += dblRealIn[iRankN];
                }
            }
        }

        delete[] piRanks;
        delete[] piIndex;
        delete[] piDims;
    }

    return pOut;
}
