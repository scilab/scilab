/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "cumsum.hxx"

int cumsum(types::Double* pIn, int iOrientation, types::Double* pOut)
{
    double* pdblInR = pIn->get();
    double* pdblOutR = pOut->get();

    double* pdblInI = pIn->getImg();
    double* pdblOutI = pOut->getImg();

    if (iOrientation == 0) // all
    {
        pdblOutR[0] = pdblInR[0];

        for (int i = 1; i < pIn->getSize(); i++)
        {
            pdblOutR[i] = pdblOutR[i - 1] + pdblInR[i];
        }

        if (pIn->isComplex())
        {
            pdblOutI[0] = pdblInI[0];
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblOutI[i] = pdblOutI[i - 1] + pdblInI[i];
            }
        }
    }
    else // cumsum on one dimension
    {
        int iSizeOfDimN = pIn->getDimsArray()[iOrientation - 1];
        int iIncrement = 1;

        for (int i = 0; i < iOrientation - 1; i++)
        {
            iIncrement *= pIn->getDimsArray()[i];
        }

        for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
        {
            for (int i = j; i < iIncrement + j; i++) // set the first values in out
            {
                pdblOutR[i] = pdblInR[i];
            }

            for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
            {
                for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                {
                    pdblOutR[i] = pdblInR[i] + pdblOutR[i - iIncrement];
                }
            }
        }

        if (pIn->isComplex())
        {
            for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblOutI[i] = pdblInI[i];
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        pdblOutI[i] = pdblInI[i] + pdblOutI[i - iIncrement];
                    }
                }
            }
        }
    }

    return 0;
}

// polynom
int cumsum(types::Polynom* pIn, int iOrientation, types::Polynom* pOut)
{
    int iErr        = 0;
    int iRank       = 0;
    int iLastRank   = 0;
    double* pdRData = NULL;
    double* pdIData = NULL;
    bool bComplex   = pIn->isComplex();

    types::SinglePoly* pSP = NULL;

    if (iOrientation == 0) // all
    {
        // set first element
        iRank = pIn->get(0)->getRank();

        if (bComplex)
        {
            pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
        }
        else
        {
            pSP = new types::SinglePoly(&pdRData, iRank);
        }

        for (int j = 0; j < iRank; j++)
        {
            pdRData[j] = pIn->get(0)->getCoefReal()[j];
            if (bComplex)
            {
                pdIData[j] = pIn->get(0)->getCoefImg()[j];
            }
        }

        pOut->set(0, pSP);
        iLastRank = iRank;

        // compute next elements
        for (int i = 1; i < pIn->getSize(); i++)
        {
            if (iRank < pIn->get(i)->getRank())
            {
                iRank = pIn->get(i)->getRank();
            }

            if (bComplex)
            {
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
            }
            else
            {
                pSP = new types::SinglePoly(&pdRData, iRank);
            }

            for (int j = 0; j < iRank; j++)
            {
                if (j >= pIn->get(i)->getRank())
                {
                    pdRData[j] = pOut->get(i - 1)->getCoefReal()[j];
                    if (bComplex)
                    {
                        pdIData[j] = pOut->get(i - 1)->getCoefImg()[j];
                    }
                    continue;
                }

                if ( j >= iLastRank)
                {
                    pdRData[j] = pIn->get(i)->getCoefReal()[j];
                    if (bComplex)
                    {
                        pdIData[j] = pIn->get(i)->getCoefImg()[j];
                    }
                    continue;
                }

                pdRData[j] = pIn->get(i)->getCoefReal()[j] + pOut->get(i - 1)->getCoefReal()[j];
                if (bComplex)
                {
                    pdIData[j] = pIn->get(i)->getCoefImg()[j] + pOut->get(i - 1)->getCoefImg()[j];
                }
            }
            pOut->set(i, pSP);
            iLastRank = iRank;
        }
    }
    else // cumsum on one dimension
    {
        int iSizeOfDimN = pIn->getDimsArray()[iOrientation - 1];
        int iIncrement = 1;

        for (int i = 0; i < iOrientation - 1; i++)
        {
            iIncrement *= pIn->getDimsArray()[i];
        }

        for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
        {
            for (int i = j; i < iIncrement + j; i++) // set the first values in out
            {
                iRank = pIn->get(i)->getRank();

                if (bComplex)
                {
                    pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                }
                else
                {
                    pSP = new types::SinglePoly(&pdRData, iRank);
                }

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(i)->getCoefReal()[j];
                    if (bComplex)
                    {
                        pdIData[j] = pIn->get(i)->getCoefImg()[j];
                    }
                }
                pOut->set(i, pSP);
            }

            for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
            {
                for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                {
                    iRank = pOut->get(i - iIncrement)->getRank();
                    if (iRank < pIn->get(i)->getRank())
                    {
                        iRank = pIn->get(i)->getRank();
                    }

                    if (bComplex)
                    {
                        pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                    }
                    else
                    {
                        pSP = new types::SinglePoly(&pdRData, iRank);
                    }

                    for (int j = 0; j < iRank; j++)
                    {
                        if (j >= pIn->get(i)->getRank())
                        {
                            pdRData[j] = pOut->get(i - iIncrement)->getCoefReal()[j];
                            if (bComplex)
                            {
                                pdIData[j] = pOut->get(i - iIncrement)->getCoefImg()[j];
                            }
                            continue;
                        }

                        if (j >= pOut->get(i - iIncrement)->getRank())
                        {
                            pdRData[j] = pIn->get(i)->getCoefReal()[j];
                            if (bComplex)
                            {
                                pdIData[j] = pIn->get(i)->getCoefImg()[j];
                            }
                            continue;
                        }

                        pdRData[j] = pIn->get(i)->getCoefReal()[j] + pOut->get(i - iIncrement)->getCoefReal()[j];
                        if (bComplex)
                        {
                            pdIData[j] = pIn->get(i)->getCoefImg()[j] + pOut->get(i - iIncrement)->getCoefImg()[j];
                        }
                    }
                    pOut->set(i, pSP);
                }
            }
        }
    }

    return iErr;
}

