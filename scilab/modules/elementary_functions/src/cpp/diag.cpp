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

#include "diag.hxx"

types::Double* diag(types::Double* pIn,  int iStartPos)
{
    types::Double* pDblOut = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pDblOut = new types::Double(iSize, 1, pIn->isComplex());
        }
        else
        {
            return types::Double::Empty();
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pDblOut->set(i, pIn->get(iPos));

            if (pIn->isComplex())
            {
                pDblOut->setImg(i, pIn->getImg(iPos));
            }
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pDblOut = new types::Double(iSize, iSize, pIn->isComplex());
        memset(pDblOut->get(), 0x00, iSize * iSize * sizeof(double));

        if (pIn->isComplex())
        {
            memset(pDblOut->getImg(), 0x00, iSize * iSize * sizeof(double));
        }

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pDblOut->set(iPos, pIn->get(i));

            if (pIn->isComplex())
            {
                pDblOut->setImg(iPos, pIn->getImg(i));
            }
        }
    }

    return pDblOut;
}

types::Polynom* diag(types::Polynom* pIn,  int iStartPos)
{
    types::Polynom* pPolyOut    = NULL;
    types::SinglePoly* pSP      = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;
    int iRank       = 0;

    double* pdRData = 0;
    double* pdIData = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, 1);
            pPolyOut->setComplex(pIn->isComplex());
        }
        else
        {
            return NULL;
        }

        if (pIn->isComplex())
        {
            for (int i = 0; i < iSize; i++)
            {
                iRank = pIn->get(i)->getRank();
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                iPos = (i + iStartCol) * iRows + (i + iStartRow);

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(iPos)->getCoefReal()[j];
                    pdIData[j] = pIn->get(iPos)->getCoefImg()[j];
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                iRank = pIn->get(i)->getRank();
                pSP = new types::SinglePoly(&pdRData, iRank);
                iPos = (i + iStartCol) * iRows + (i + iStartRow);

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(iPos)->getCoefReal()[j];
                }

                pPolyOut->set(i, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pPolyOut = new types::Polynom(pIn->getVariableName(), iSize, iSize);
        pPolyOut->setComplex(pIn->isComplex());

        if (pIn->isComplex())
        {
            for (int i = 0; i < iSizeOfVector; i++)
            {
                iRank = pIn->get(i)->getRank();
                pSP = new types::SinglePoly(&pdRData, &pdIData, iRank);
                iPos = (i + iStartCol) * iSize + (i + iStartRow);

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(i)->getCoefReal()[j];
                    pdIData[j] = pIn->get(i)->getCoefImg()[j];
                }

                pPolyOut->set(iPos, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
        else
        {
            for (int i = 0; i < iSizeOfVector; i++)
            {
                iRank = pIn->get(i)->getRank();
                pSP = new types::SinglePoly(&pdRData, iRank);
                iPos = (i + iStartCol) * iSize + (i + iStartRow);

                for (int j = 0; j < iRank; j++)
                {
                    pdRData[j] = pIn->get(i)->getCoefReal()[j];
                }

                pPolyOut->set(iPos, pSP);
                delete pSP;
                pSP = NULL;
            }
        }
    }

    pPolyOut->updateRank();
    return pPolyOut;
}


types::String* diag(types::String* pIn,  int iStartPos)
{
    types::String* pStrOut = NULL;

    int iRows = pIn->getRows();
    int iCols = pIn->getCols();

    int iSize       = 0;
    int iStartRow   = 0;
    int iStartCol   = 0;
    int iPos        = 0;

    if (iRows != 1 && iCols != 1) // pIn is a matrix
    {
        if (iStartPos < 0)
        {
            iSize = Max(0, Min(iRows + iStartPos, iCols));
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = Max(0, Min(iRows, iCols - iStartPos));
            iStartCol = iStartPos;
        }

        if (iSize)
        {
            pStrOut = new types::String(iSize, 1);
        }
        else
        {
            return NULL;
        }

        for (int i = 0; i < iSize; i++)
        {
            iPos = (i + iStartCol) * iRows + (i + iStartRow);
            pStrOut->set(i, pIn->get(iPos));
        }
    }
    else // pIn is a vector
    {
        int iSizeOfVector = Max(iRows, iCols);
        if (iStartPos < 0)
        {
            iSize = iSizeOfVector - iStartPos;
            iStartRow = -iStartPos;
        }
        else
        {
            iSize = iSizeOfVector + iStartPos;
            iStartCol = iStartPos;
        }

        pStrOut = new types::String(iSize, iSize);

        for (int i = 0; i < iSize * iSize; i++)
        {
            pStrOut->set(i, L"");
        }

        for (int i = 0; i < iSizeOfVector; i++)
        {
            iPos = (i + iStartCol) * iSize + (i + iStartRow);
            pStrOut->set(iPos, pIn->get(i));
        }
    }

    return pStrOut;
}

