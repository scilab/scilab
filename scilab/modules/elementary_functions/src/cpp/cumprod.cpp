/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
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

#include "cumprod.hxx"
extern "C"
{
#include "matrix_multiplication.h"
}

int cumprod(types::Double* pIn, int iOrientation, types::Double* pOut)
{
    double* pdblInReal  = pIn->getReal();
    double* pdblOutReal = pOut->getReal();
    double* pdblInImg   = pIn->getImg();
    double* pdblOutImg  = pOut->getImg();

    int size = pIn->getSize();
    if (iOrientation == 0) // all
    {
        pdblOutReal[0] = pdblInReal[0];
        if (pIn->isComplex())
        {
            pdblOutImg[0] = pdblInImg[0];
            for (int i = 1; i < size; i++)
            {
                pdblOutReal[i] = pdblInReal[i] * pdblOutReal[i - 1];
                pdblOutReal[i] -= pdblInImg[i] * pdblOutImg[i - 1];
                pdblOutImg[i] = (pdblInReal[i] * pdblOutImg[i - 1]) + (pdblInImg[i] * pdblOutReal[i - 1]);
            }
        }
        else
        {
            for (int i = 1; i < size; i++)
            {
                pdblOutReal[i] = pdblInReal[i] * pdblOutReal[i - 1];
            }
        }
    }
    else // cumprod on one dimension
    {
        int iSizeOfDimN = pIn->getDimsArray()[iOrientation - 1];
        int iIncrement = 1;

        for (int i = 0; i < iOrientation - 1; i++)
        {
            iIncrement *= pIn->getDimsArray()[i];
        }

        if (pIn->isComplex())
        {
            for (int j = 0; j < size; j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblOutReal[i] = pdblInReal[i];
                    pdblOutImg[i] = pdblInImg[i];
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        pdblOutReal[i] = pdblInReal[i] * pdblOutReal[i - iIncrement];
                        pdblOutReal[i] -= pdblInImg[i] * pdblOutImg[i - iIncrement];
                        pdblOutImg[i] = (pdblInReal[i] * pdblOutImg[i - iIncrement]) + (pdblInImg[i] * pdblOutReal[i - iIncrement]);
                    }
                }
            }
        }
        else
        {
            for (int j = 0; j < size; j += (iIncrement * iSizeOfDimN))
            {
                for (int i = j; i < iIncrement + j; i++) // set the first values in out
                {
                    pdblOutReal[i] = pdblInReal[i];
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        pdblOutReal[i] = pdblInReal[i] * pdblOutReal[i - iIncrement];
                    }
                }
            }
        }
    }

    return 0;
}

// polynom
int cumprod(types::Polynom* pIn, int iOrientation, types::Polynom* pOut)
{
    int iErr        = 0;
    int iSize       = 0;
    int iOutRank    = 0;
    int iLastSize   = 0;

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
        pOut->set(0, pIn->get(0));

        iLastSize    = pOut->get(0)->getSize();
        pdblLastReal = pOut->get(0)->get();
        pdblLastImg  = pOut->get(0)->getImg();

        // compute next elements
        if (bComplex)
        {
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblReal = pIn->get(i)->get();
                pdblImg = pIn->get(i)->getImg();
                iSize = pIn->get(i)->getSize();

                iOutRank = iLastSize - 1 + iSize - 1;

                pSP = new types::SinglePoly(&pdRData, &pdIData, iOutRank);
                pSP->setZeros();

                iMultiComplexPolyByComplexPoly( pdblReal, pdblImg, iSize,
                                                pdblLastReal, pdblLastImg, iLastSize,
                                                pdRData, pdIData, iOutRank + 1);

                pOut->set(i, pSP);
                pdblLastReal = pOut->get(i)->get();
                pdblLastImg = pOut->get(i)->getImg();
                iLastSize = iOutRank + 1;
                delete pSP;
            }
        }
        else
        {
            for (int i = 1; i < pIn->getSize(); i++)
            {
                pdblReal = pIn->get(i)->get();
                iSize = pIn->get(i)->getSize();

                iOutRank = iLastSize - 1 + iSize - 1;

                pSP = new types::SinglePoly(&pdRData, iOutRank);
                pSP->setZeros();
                iMultiScilabPolynomByScilabPolynom(pdblReal, iSize, pdblLastReal, iLastSize, pdRData, iOutRank + 1);

                pOut->set(i, pSP);
                pdblLastReal = pOut->get(i)->get();
                iLastSize = iOutRank + 1;
                delete pSP;
            }
        }
    }
    else // cumprod on one dimension
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
                    pOut->set(i, pIn->get(i));
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        iLastSize = pOut->get(i - iIncrement)->getSize();
                        pdblLastReal = pOut->get(i - iIncrement)->get();
                        pdblLastImg = pOut->get(i - iIncrement)->getImg();
                        iSize = pIn->get(i)->getSize();
                        pdblReal = pIn->get(i)->get();
                        pdblImg = pIn->get(i)->getImg();

                        iOutRank = iLastSize - 1 + iSize - 1;
                        pSP = new types::SinglePoly(&pdRData, &pdIData, iOutRank);

                        iMultiComplexPolyByComplexPoly(pdblReal, pdblImg, iSize,
                                                       pdblLastReal, pdblLastImg, iLastSize,
                                                       pdRData, pdIData, iOutRank + 1);

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
                    pOut->set(i, pIn->get(i));
                }

                for (int k = 1; k < iSizeOfDimN; k++) // make the cumprod for the next values
                {
                    for (int i = (iIncrement * k) + j; i < (iIncrement * (k + 1)) + j; i++)
                    {
                        iLastSize = pOut->get(i - iIncrement)->getSize();
                        pdblLastReal = pOut->get(i - iIncrement)->get();
                        iSize = pIn->get(i)->getSize();
                        pdblReal = pIn->get(i)->get();

                        iOutRank = iLastSize - 1 + iSize - 1;
                        pSP = new types::SinglePoly(&pdRData, iOutRank);

                        iMultiScilabPolynomByScilabPolynom(pdblReal, iSize, pdblLastReal, iLastSize, pdRData, iOutRank + 1);

                        pOut->set(i, pSP);
                        delete pSP;
                    }
                }
            }
        }
    }

    pOut->updateRank();
    return iErr;
}

