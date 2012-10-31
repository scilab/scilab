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

        if (pIn->isComplex())
        {
            pdblOutI[0] = pdblInI[0];
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblOutR[i] = pdblOutR[i - 1] + pdblInR[i];
                pdblOutI[i] = pdblOutI[i - 1] + pdblInI[i];
            }
        }
        else
        {
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblOutR[i] = pdblOutR[i - 1] + pdblInR[i];
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

        if (pIn->isComplex())
        {
            for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblOutR[i] = pdblInR[i];
                    pdblOutI[i] = pdblInI[i];
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        pdblOutR[i] = pdblInR[i] + pdblOutR[i - iIncrement];
                        pdblOutI[i] = pdblInI[i] + pdblOutI[i - iIncrement];
                    }
                }
            }
        }
        else
        {
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
        }
    }

    return 0;
}

// polynom
int cumsum(types::Polynom* pIn, int iOrientation, types::Polynom* pOut)
{
    int iErr        = 0;
    int iRank       = 0;
    int iOutRank    = 0;
    int iLastRank   = 0;
    int iMin        = 0;

    double* pdRData         = NULL;
    double* pdIData         = NULL;
    double* pdblReal        = NULL;
    double* pdblImg         = NULL;
    double* pdblLastReal    = NULL;
    double* pdblLastImg     = NULL;

    bool bComplex   = pIn->isComplex();

    types::SinglePoly* pSP = NULL;

    if (iOrientation == 0) // all
    {
        // set first element
        iRank = pIn->get(0)->getRank();
        pdblReal = pIn->get(0)->getCoefReal();
        if (bComplex)
        {
            pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
            pdblImg = pIn->get(0)->getCoefImg();
            for (int j = 0; j < iRank; j++)
            {
                pdRData[j] = pdblReal[j];
                pdIData[j] = pdblImg[j];
            }
        }
        else
        {
            pSP = new types::SinglePoly(&pdRData, iRank);
            for (int j = 0; j < iRank; j++)
            {
                pdRData[j] = pdblReal[j];
            }
        }

        pOut->set(0, pSP);
        iLastRank = iRank;
        pdblLastReal = pdblReal;
        pdblLastImg = pdblImg;

        // compute next elements
        if (bComplex)
        {
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblReal = pIn->get(i)->getCoefReal();
                pdblImg  = pIn->get(i)->getCoefImg();
                iRank    = pIn->get(i)->getRank();

                iOutRank = Max(iRank, iLastRank);
                iMin     = Min(iRank, iLastRank);

                pSP = new types::SinglePoly(&pdRData, &pdIData, iOutRank);
                for (int j = 0; j < iMin; j++)
                {
                    pdRData[j] = pdblReal[j] + pdblLastReal[j];
                    pdIData[j] = pdblImg[j]  + pdblLastImg[j];
                }

                if (iOutRank == iRank)
                {
                    for (int j = iMin; j < iOutRank; j++)
                    {
                        pdRData[j] = pdblReal[j];
                        pdIData[j] = pdblImg[j];
                    }
                }
                else
                {
                    for (int j = iMin; j < iOutRank; j++)
                    {
                        pdRData[j] = pdblLastReal[j];
                        pdIData[j] = pdblLastImg[j];
                    }
                }

                pOut->set(i, pSP);
                pdblLastReal = pdRData;
                pdblLastImg = pdIData;
                iLastRank = iOutRank;
                delete pSP;
            }
        }
        else
        {
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblReal = pIn->get(i)->getCoefReal();
                iRank    = pIn->get(i)->getRank();

                iOutRank = Max(iRank, iLastRank);
                iMin     = Min(iRank, iLastRank);

                pSP = new types::SinglePoly(&pdRData, iOutRank);
                for (int j = 0; j < iMin; j++)
                {
                    pdRData[j] = pdblReal[j] + pdblLastReal[j];
                }

                if (iOutRank == iRank)
                {
                    for (int j = iMin; j < iOutRank; j++)
                    {
                        pdRData[j] = pdblReal[j];
                    }
                }
                else
                {
                    for (int j = iMin; j < iOutRank; j++)
                    {
                        pdRData[j] = pdblLastReal[j];
                    }
                }

                pOut->set(i, pSP);
                pdblLastReal = pdRData;
                iLastRank = iOutRank;
                delete pSP;
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

        if (bComplex)
        {
            for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblReal = pIn->get(i)->getCoefReal();
                    pdblImg  = pIn->get(i)->getCoefImg();
                    iRank    = pIn->get(i)->getRank();

                    pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);

                    for (int j = 0; j < iRank; j++)
                    {
                        pdRData[j] = pdblReal[j];
                        pdIData[j] = pdblImg[j];
                    }
                    pOut->set(i, pSP);
                    delete pSP;
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        iLastRank    = pOut->get(i - iIncrement)->getRank();
                        pdblLastReal = pOut->get(i - iIncrement)->getCoefReal();
                        pdblLastImg  = pOut->get(i - iIncrement)->getCoefImg();

                        iRank    = pIn->get(i)->getRank();
                        pdblReal = pIn->get(i)->getCoefReal();
                        pdblImg  = pIn->get(i)->getCoefImg();

                        iOutRank = Max(iRank, iLastRank);
                        iMin     = Min(iRank, iLastRank);

                        pSP = new types::SinglePoly(&pdRData, &pdIData, iOutRank);
                        for (int j = 0; j < iMin; j++)
                        {
                            pdRData[j] = pdblReal[j] + pdblLastReal[j];
                            pdIData[j] = pdblImg[j]  + pdblLastImg[j];
                        }

                        if (iOutRank == iRank)
                        {
                            for (int j = iMin; j < iOutRank; j++)
                            {
                                pdRData[j] = pdblReal[j];
                                pdIData[j] = pdblImg[j];
                            }
                        }
                        else
                        {
                            for (int j = iMin; j < iOutRank; j++)
                            {
                                pdRData[j] = pdblLastReal[j];
                                pdIData[j] = pdblLastImg[j];
                            }
                        }
                        pOut->set(i, pSP);
                        delete pSP;
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < pIn->getSize(); j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblReal = pIn->get(i)->getCoefReal();
                    iRank    = pIn->get(i)->getRank();

                    pSP = new types::SinglePoly(&pdRData, iRank);

                    for (int j = 0; j < iRank; j++)
                    {
                        pdRData[j] = pdblReal[j];
                    }
                    pOut->set(i, pSP);
                    delete pSP;
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumsum for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        iLastRank    = pOut->get(i - iIncrement)->getRank();
                        pdblLastReal = pOut->get(i - iIncrement)->getCoefReal();

                        iRank    = pIn->get(i)->getRank();
                        pdblReal = pIn->get(i)->getCoefReal();

                        iOutRank = Max(iRank, iLastRank);
                        iMin     = Min(iRank, iLastRank);

                        pSP = new types::SinglePoly(&pdRData, iOutRank);
                        for (int j = 0; j < iMin; j++)
                        {
                            pdRData[j] = pdblReal[j] + pdblLastReal[j];
                        }

                        if (iOutRank == iRank)
                        {
                            for (int j = iMin; j < iOutRank; j++)
                            {
                                pdRData[j] = pdblReal[j];
                            }
                        }
                        else
                        {
                            for (int j = iMin; j < iOutRank; j++)
                            {
                                pdRData[j] = pdblLastReal[j];
                            }
                        }
                        pOut->set(i, pSP);
                        delete pSP;
                    }
                }
            }
        }
    }

    return iErr;
}

