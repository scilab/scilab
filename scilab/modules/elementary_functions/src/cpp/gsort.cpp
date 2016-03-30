/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2012 - DIGITEO - Cedric Delamarre
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
#include <algorithm>
#include <vector>
#include <vector>
#include <functional>
#include "gsort.hxx"

extern "C"
{
#include "abs.h" // complex case
#include "matrix_division.h"
}

/*------------------------ comparison function ------------------------------*/
// Basic comparison.
template <typename T>
bool increasing (T left, T right)
{
    if (ISNAN((double)left))
    {
        return false;
    }

    if (ISNAN((double)right))
    {
        return true;
    }

    return (left < right);
}

template <typename T>
bool descendent (T left, T right)
{
    if (ISNAN((double)left))
    {
        return true;
    }

    if (ISNAN((double)right))
    {
        return false;
    }

    return (left > right);
}

// std::pair<int,int> use to save the position.
template <typename T>
bool increasing (std::pair<std::pair<int, int>, T> left, std::pair<std::pair<int, int>, T> right)
{
    if (ISNAN((double)left.second))
    {
        return false;
    }

    if (ISNAN((double)right.second))
    {
        return true;
    }

    // keep position order (position order is always increasing)
    if (left.second == right.second)
    {
        return (left.first.second < right.first.second);
    }

    return (left.second < right.second);
}

template <typename T>
bool descendent (std::pair<std::pair<int, int>, T> left, std::pair<std::pair<int, int>, T> right)
{
    if (ISNAN((double)left.second))
    {
        return true;
    }

    if (ISNAN((double)right.second))
    {
        return false;
    }

    // keep position order (position order is always increasing)
    if (left.second == right.second)
    {
        return (left.first.second < right.first.second);
    }

    return (left.second > right.second);
}

// compare two vector.
template <typename T>
bool increasing (std::pair<int, std::pair<T*, T*> > left, std::pair<int, std::pair<T*, T*> > right)
{
    bool (*pMyComp)(T left, T right);
    pMyComp = &increasing;
    return std::lexicographical_compare(left.second.first, left.second.second, right.second.first, right.second.second, *pMyComp);
}

template <typename T>
bool descendent (std::pair<int, std::pair<T*, T*> > left, std::pair<int, std::pair<T*, T*> > right)
{
    bool (*pMyComp)(T left, T right);
    pMyComp = &descendent;
    return std::lexicographical_compare(left.second.first, left.second.second, right.second.first, right.second.second, *pMyComp);
}

// string case
bool increasingCompareStr (wchar_t* left, wchar_t* right)
{
    return wcscmp(left, right) < 0;
}

bool descendentCompareStr (wchar_t* left, wchar_t* right)
{
    return wcscmp(left, right) > 0;
}

bool descendentStr (std::pair<int, std::pair<wchar_t**, wchar_t**> > left, std::pair<int, std::pair<wchar_t**, wchar_t**> > right)
{
    return std::lexicographical_compare(right.second.first, right.second.second, left.second.first, left.second.second, increasingCompareStr);
}

bool increasingStr (std::pair<int, std::pair<wchar_t**, wchar_t**> > left, std::pair<int, std::pair<wchar_t**, wchar_t**> > right)
{
    return std::lexicographical_compare(left.second.first, left.second.second, right.second.first, right.second.second, increasingCompareStr);
}

bool descendentStr (std::pair<int, wchar_t*> left, std::pair<int, wchar_t*> right)
{
    int comp = wcscmp(left.second, right.second);
    // keep position order (position order is always increasing)
    if (comp == 0)
    {
        return (left.first < right.first);
    }

    return comp > 0;
}

bool increasingStr (std::pair<int, wchar_t*> left, std::pair<int, wchar_t*> right)
{
    int comp = wcscmp(left.second, right.second);
    // keep position order (position order is always increasing)
    if (comp == 0)
    {
        return (left.first < right.first);
    }

    return comp < 0;
}

/*--------------------------- Double ---------------------------------------*/
types::Double* gsort(types::Double* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::Double* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    double* pdblInR = pIn->get();
    double* pdblInI = pIn->getImg();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWayBasic)(double left,
                          double right);

    // std::pair<std::pair<index global, index on row/col> value>
    bool (*pFuncWay)(std::pair<std::pair<int, int>, double> left,
                     std::pair<std::pair<int, int>, double> right);

    // std::pair<index on row/col std::pair<start vector, end vector> >
    bool (*pFuncWayL)(std::pair<int, std::pair<double*, double*> > left,
                      std::pair<int, std::pair<double*, double*> > right);

    // sort direction
    if (wstrWay == L"i")
    {
        pFuncWayBasic   = &(increasing);
        pFuncWay        = &(increasing);
        pFuncWayL       = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWayBasic   = &(descendent);
        pFuncWay        = &(descendent);
        pFuncWayL       = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (pIn->isComplex())
    {
        pOut = new types::Double(pIn->getDims(), pIn->getDimsArray(), true);
        double* pdblOutR = pOut->get();
        double* pdblOutI = pOut->getImg();

        if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
        {
            std::vector<std::pair<std::pair<int, int>, double> > V;
            std::vector<std::pair<std::pair<int, int>, double> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, double>(std::pair<int, int>(iPos, j), dabsz(pdblInR[iPos], pdblInI[iPos])));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    pdblOutR[iPosSet] = pdblInR[(*it).first.first];
                    pdblOutI[iPosSet] = pdblInI[(*it).first.first];
                    if (pDblInd)
                    {
                        pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                    }
                }
                V.clear();
            }
        }
        else// wstrProcess == L"lr" and wstrProcess == L"lc"
        {
            std::vector<std::pair<int, std::pair<double*, double*> > > V;
            std::vector<std::pair<int, std::pair<double*, double*> > >::iterator it;

            types::Double* pTemp = new types::Double(pIn->getRows(), pIn->getCols());
            double* pdblTemp = pTemp->get();

            if (wstrProcess == L"lr")
            {
                // transpose matrix pIn
                types::Double* pTemp = pIn;
                pIn = new types::Double(pTemp->getCols(), pTemp->getRows(), pTemp->isComplex());
                pdblInR = pIn->get();
                pdblInI = pIn->getImg();
                vTransposeComplexMatrix(pTemp->get(), pTemp->getImg(), pTemp->getRows(), pTemp->getCols(), pIn->get(), pIn->getImg(), 0);
            }

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                pdblTemp[i] = dabsz(pdblInR[i], pdblInI[i]);
            }

            for (int i = 0; i < iTimes; i++)
            {
                V.push_back(std::pair<int, std::pair<double*, double*> >(i, std::pair<double*, double*>(pdblTemp + (i * iItem), pdblTemp + (i + 1) * iItem)));
            }

            std::sort(V.begin(), V.end(), (*pFuncWayL));

            int j = 0;
            for (it = V.begin(); it != V.end(); it++, j++)
            {
                for (int i = 0; i < iItem; i++)
                {
                    iPosSet = i * iOffset1 + j * iOffset2;
                    iPosGet = (*it).first * iItem + i;
                    pdblOutR[iPosSet] = pdblInR[iPosGet];
                    pdblOutI[iPosSet] = pdblInI[iPosGet];
                }

                if (pDblInd)
                {
                    pdblInd[j] = static_cast<double>((*it).first + 1);
                }
            }

            V.clear();
            delete pTemp;
            pTemp = NULL;

            if (wstrProcess == L"lr")
            {
                delete pIn;
            }
        }
    }
    else
    {
        if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
        {
            if (pDblInd)
            {
                pOut = new types::Double(pIn->getDims(), pIn->getDimsArray());
                double* pdblOutR = pOut->get();

                std::vector<std::pair<std::pair<int, int>, double> > V;
                std::vector<std::pair<std::pair<int, int>, double> >::iterator it;

                for (int i = 0; i < iTimes; i++)
                {
                    for (int j = 0; j < iItem; j++)
                    {
                        iPos = i * iOffset2 + j * iOffset1;
                        V.push_back(std::pair<std::pair<int, int>, double>(std::pair<int, int>(iPos, j), pdblInR[iPos]));
                    }

                    std::sort(V.begin(), V.end(), (*pFuncWay));

                    int j = 0;
                    for (it = V.begin(); it != V.end(); it++, j++)
                    {
                        iPosSet = i * iOffset2 + j * iOffset1;
                        pdblOutR[iPosSet] = (*it).second;
                        pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                    }
                    V.clear();
                }
            }
            else
            {
                if (wstrProcess == L"g")
                {
                    pOut = pIn->clone()->getAs<types::Double>();
                    double* pdblOutR = pOut->get();
                    std::sort(pdblOutR, pdblOutR + pOut->getSize(), (*pFuncWayBasic));
                }
                else if (wstrProcess == L"c")
                {
                    pOut = new types::Double(pIn->getCols(), pIn->getRows());
                    double* pdblOutR = pOut->get();

                    // transpose input in output
                    vTransposeRealMatrix(pdblInR, pIn->getRows(), pIn->getCols(), pdblOutR);

                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pdblOutR + (iItem * i), pdblOutR + (iItem * (i + 1)), (*pFuncWayBasic));
                    }

                    // transpose output
                    types::Double* pTemp = pOut;
                    pOut = new types::Double(pTemp->getCols(), pTemp->getRows());
                    pdblOutR = pOut->get();
                    vTransposeRealMatrix(pTemp->get(), pTemp->getRows(), pTemp->getCols(), pdblOutR);
                    delete pTemp;
                }
                else //wstrProcess == L"r"
                {
                    pOut = pIn->clone()->getAs<types::Double>();
                    double* pdblOutR = pOut->get();

                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pdblOutR + (iItem * i), pdblOutR + (iItem * (i + 1)), (*pFuncWayBasic));
                    }
                }
            }
        }
        else// wstrProcess == L"lr" and wstrProcess == L"lc"
        {
            pOut = new types::Double(pIn->getDims(), pIn->getDimsArray());
            double* pdblOutR = pOut->get();

            std::vector<std::pair<int, std::pair<double*, double*> > > V;
            std::vector<std::pair<int, std::pair<double*, double*> > >::iterator it;

            if (wstrProcess == L"lr")
            {
                // transpose matrix pIn
                types::Double* pTemp = pIn;
                pIn = new types::Double(pTemp->getCols(), pTemp->getRows());
                pdblInR = pIn->get();
                vTransposeRealMatrix(pTemp->get(), pTemp->getRows(), pTemp->getCols(), pdblInR);
            }

            for (int i = 0; i < iTimes; i++)
            {
                V.push_back(std::pair<int, std::pair<double*, double*> >(i, std::pair<double*, double*>(pdblInR + i * iItem, pdblInR + (i + 1) * iItem)));
            }

            std::sort(V.begin(), V.end(), (*pFuncWayL));

            int j = 0;
            for (it = V.begin(); it != V.end(); it++, j++)
            {
                for (int i = 0; i < iItem; i++)
                {
                    iPosSet = i * iOffset1 + j * iOffset2;
                    pdblOutR[iPosSet] = ((*it).second.first)[i];
                }

                if (pDblInd)
                {
                    pdblInd[j] = static_cast<double>((*it).first + 1);
                }
            }

            V.clear();
            if (wstrProcess == L"lr")
            {
                delete pIn;
            }
        }
    }

    return pOut;
}

/*--------------------------- String ---------------------------------------*/
types::String* gsort(types::String* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::String* pOut = nullptr;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    bool (*pFuncWayStr)(std::pair<int, wchar_t*> left,
                        std::pair<int, wchar_t*> right);

    bool (*pFuncWayStrL)(std::pair<int, std::pair<wchar_t**, wchar_t**> > left,
                         std::pair<int, std::pair<wchar_t**, wchar_t**> > right);

    if (wstrWay == L"i")
    {
        pFuncWayStr = &(increasingStr);
        pFuncWayStrL = &(increasingStr);
    }
    else // strWay == L"d"
    {
        pFuncWayStr = &(descendentStr);
        pFuncWayStrL = &(descendentStr);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }
    else if (wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }
    else if (wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::String(pIn->getDims(), pIn->getDimsArray());

            std::vector<std::pair<int, wchar_t*> > V;
            std::vector<std::pair<int, wchar_t*> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    wchar_t* pWcsData = pIn->get(iPos);
                    V.push_back(std::pair<int, wchar_t*>(j, pWcsData));
                }

                std::sort(V.begin(), V.end(), (*pFuncWayStr));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    pOut->set(iPosSet, (*it).second);
                    if (pDblInd)
                    {
                        pDblInd->set(iPosSet, static_cast<double>((*it).first + 1));
                    }
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::String>();
                if (wstrWay == L"i")
                {
                    std::sort(pOut->get(), pOut->get() + pOut->getSize(), increasingCompareStr);
                }
                else // strWay == L"d"
                {
                    std::sort(pOut->get(), pOut->get() + pOut->getSize(), descendentCompareStr);
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::String(pIn->getRows(), pIn->getCols());

                // transpose matrix pIn
                wchar_t** pwstOut = new wchar_t*[pIn->getSize()];
                wchar_t** pwstIn = pIn->get();
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();

                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    pwstOut[iPos] = pwstIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pwstOut + (iItem * i), pwstOut + (iItem * (i + 1)), increasingCompareStr);
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pwstOut + (iItem * i), pwstOut + (iItem * (i + 1)), descendentCompareStr);
                    }
                }

                // transpose output
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iCols) * iRows + (i / iCols);
                    pOut->set(iPos, pwstOut[i]);
                }

                delete[] pwstOut;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::String>();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pOut->get() + (iItem * i), pOut->get() + (iItem * (i + 1)), increasingCompareStr);
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(pOut->get() + (iItem * i), pOut->get() + (iItem * (i + 1)), descendentCompareStr);
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::String(pIn->getDims(), pIn->getDimsArray());
        std::vector<std::pair<int, std::pair<wchar_t**, wchar_t**> > > V;
        std::vector<std::pair<int, std::pair<wchar_t**, wchar_t**> > >::iterator it;

        wchar_t** pwstIn = NULL;
        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::String* pTemp = pIn;

            pwstIn = new wchar_t*[pTemp->getSize()];
            wchar_t** pwstTemp = pTemp->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                pwstIn[iPos] = pwstTemp[i];
            }
        }
        else
        {
            pwstIn = pIn->get();
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<wchar_t**, wchar_t**> >(i, std::pair<wchar_t**, wchar_t**>(pwstIn + (i * iItem), pwstIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayStrL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                pOut->set(iPosSet, ((*it).second.first)[i]);
            }

            if (pDblInd)
            {
                pDblInd->set(j, static_cast<double>((*it).first + 1));
            }
        }
        V.clear();

        if (wstrProcess == L"lr")
        {
            delete[] pwstIn;
        }
    }

    return pOut;
}

/*--------------------------- Int8 -----------------------------------------*/
types::Int8* gsort(types::Int8* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::Int8* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    char* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, long> left,
                     std::pair<std::pair<int, int>, long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<char*, char*> > left,
                      std::pair<int, std::pair<char*, char*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::Int8(pIn->getDims(), pIn->getDimsArray());
            char* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, long> > V;
            std::vector<std::pair<std::pair<int, int>, long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, long>(std::pair<int, int>(iPos, j), (long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (char)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::Int8>();
                char* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<char>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::Int8(pIn->getCols(), pIn->getRows());
                char* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<char>());
                    }
                }

                // transpose output
                types::Int8* pTemp = pOut;
                char* piTemp = pTemp->get();
                pOut = new types::Int8(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::Int8>();
                char* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<char>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::Int8(pIn->getDims(), pIn->getDimsArray());
        char* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<char*, char*> > > V;
        std::vector<std::pair<int, std::pair<char*, char*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::Int8* pTemp = pIn;
            char* piTemp = pTemp->get();
            pIn = new types::Int8(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<char*, char*> >(i, std::pair<char*, char*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- Int16 -----------------------------------------*/
types::Int16* gsort(types::Int16* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::Int16* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    short* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, long> left,
                     std::pair<std::pair<int, int>, long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<short*, short*> > left,
                      std::pair<int, std::pair<short*, short*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::Int16(pIn->getDims(), pIn->getDimsArray());
            short* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, long> > V;
            std::vector<std::pair<std::pair<int, int>, long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, long>(std::pair<int, int>(iPos, j), (long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (short)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::Int16>();
                short* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<short>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::Int16(pIn->getCols(), pIn->getRows());
                short* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<short>());
                    }
                }

                // transpose output
                types::Int16* pTemp = pOut;
                short* piTemp = pTemp->get();
                pOut = new types::Int16(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::Int16>();
                short* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<short>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::Int16(pIn->getDims(), pIn->getDimsArray());
        short* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<short*, short*> > > V;
        std::vector<std::pair<int, std::pair<short*, short*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::Int16* pTemp = pIn;
            short* piTemp = pTemp->get();
            pIn = new types::Int16(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<short*, short*> >(i, std::pair<short*, short*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- Int32 -----------------------------------------*/
types::Int32* gsort(types::Int32* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::Int32* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    int* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, long> left,
                     std::pair<std::pair<int, int>, long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<int*, int*> > left,
                      std::pair<int, std::pair<int*, int*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::Int32(pIn->getDims(), pIn->getDimsArray());
            int* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, long> > V;
            std::vector<std::pair<std::pair<int, int>, long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, long>(std::pair<int, int>(iPos, j), (long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (int)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::Int32>();
                int* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<int>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::Int32(pIn->getCols(), pIn->getRows());
                int* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<int>());
                    }
                }

                // transpose output
                types::Int32* pTemp = pOut;
                int* piTemp = pTemp->get();
                pOut = new types::Int32(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::Int32>();
                int* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<int>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::Int32(pIn->getDims(), pIn->getDimsArray());
        int* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<int*, int*> > > V;
        std::vector<std::pair<int, std::pair<int*, int*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::Int32* pTemp = pIn;
            int* piTemp = pTemp->get();
            pIn = new types::Int32(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<int*, int*> >(i, std::pair<int*, int*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- Int64 -----------------------------------------*/
types::Int64* gsort(types::Int64* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::Int64* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    long long* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, long long> left,
                     std::pair<std::pair<int, int>, long long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<long long*, long long*> > left,
                      std::pair<int, std::pair<long long*, long long*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::Int64(pIn->getDims(), pIn->getDimsArray());
            long long* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, long long> > V;
            std::vector<std::pair<std::pair<int, int>, long long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, long long>(std::pair<int, int>(iPos, j), piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::Int64>();
                long long* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<long long>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::Int64(pIn->getCols(), pIn->getRows());
                long long* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<long long>());
                    }
                }

                // transpose output
                types::Int64* pTemp = pOut;
                long long* piTemp = pTemp->get();
                pOut = new types::Int64(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::Int64>();
                long long* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<long long>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::Int64(pIn->getDims(), pIn->getDimsArray());
        long long* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<long long*, long long*> > > V;
        std::vector<std::pair<int, std::pair<long long*, long long*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::Int64* pTemp = pIn;
            long long* piTemp = pTemp->get();
            pIn = new types::Int64(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<long long*, long long*> >(i, std::pair<long long*, long long*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- UInt8 -----------------------------------------*/
types::UInt8* gsort(types::UInt8* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::UInt8* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    unsigned char* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, unsigned long> left,
                     std::pair<std::pair<int, int>, unsigned long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<unsigned char*, unsigned char*> > left,
                      std::pair<int, std::pair<unsigned char*, unsigned char*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::UInt8(pIn->getDims(), pIn->getDimsArray());
            unsigned char* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, unsigned long> > V;
            std::vector<std::pair<std::pair<int, int>, unsigned long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, unsigned long>(std::pair<int, int>(iPos, j), (unsigned long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (unsigned char)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::UInt8>();
                unsigned char* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<unsigned char>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::UInt8(pIn->getCols(), pIn->getRows());
                unsigned char* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned char>());
                    }
                }

                // transpose output
                types::UInt8* pTemp = pOut;
                unsigned char* piTemp = pTemp->get();
                pOut = new types::UInt8(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::UInt8>();
                unsigned char* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned char>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::UInt8(pIn->getDims(), pIn->getDimsArray());
        unsigned char* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<unsigned char*, unsigned char*> > > V;
        std::vector<std::pair<int, std::pair<unsigned char*, unsigned char*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::UInt8* pTemp = pIn;
            unsigned char* piTemp = pTemp->get();
            pIn = new types::UInt8(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<unsigned char*, unsigned char*> >(i, std::pair<unsigned char*, unsigned char*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- UInt16 -----------------------------------------*/
types::UInt16* gsort(types::UInt16* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::UInt16* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    unsigned short* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, unsigned long> left,
                     std::pair<std::pair<int, int>, unsigned long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<unsigned short*, unsigned short*> > left,
                      std::pair<int, std::pair<unsigned short*, unsigned short*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::UInt16(pIn->getDims(), pIn->getDimsArray());
            unsigned short* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, unsigned long> > V;
            std::vector<std::pair<std::pair<int, int>, unsigned long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, unsigned long>(std::pair<int, int>(iPos, j), (unsigned long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (unsigned short)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::UInt16>();
                unsigned short* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<unsigned short>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::UInt16(pIn->getCols(), pIn->getRows());
                unsigned short* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned short>());
                    }
                }

                // transpose output
                types::UInt16* pTemp = pOut;
                unsigned short* piTemp = pTemp->get();
                pOut = new types::UInt16(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::UInt16>();
                unsigned short* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned short>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::UInt16(pIn->getDims(), pIn->getDimsArray());
        unsigned short* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<unsigned short*, unsigned short*> > > V;
        std::vector<std::pair<int, std::pair<unsigned short*, unsigned short*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::UInt16* pTemp = pIn;
            unsigned short* piTemp = pTemp->get();
            pIn = new types::UInt16(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<unsigned short*, unsigned short*> >(i, std::pair<unsigned short*, unsigned short*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- UInt32 -----------------------------------------*/
types::UInt32* gsort(types::UInt32* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::UInt32* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    unsigned int* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, unsigned long> left,
                     std::pair<std::pair<int, int>, unsigned long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<unsigned int*, unsigned int*> > left,
                      std::pair<int, std::pair<unsigned int*, unsigned int*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::UInt32(pIn->getDims(), pIn->getDimsArray());
            unsigned int* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, unsigned long> > V;
            std::vector<std::pair<std::pair<int, int>, unsigned long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, unsigned long>(std::pair<int, int>(iPos, j), (unsigned long)piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (unsigned int)(*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::UInt32>();
                unsigned int* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<unsigned int>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::UInt32(pIn->getCols(), pIn->getRows());
                unsigned int* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned int>());
                    }
                }

                // transpose output
                types::UInt32* pTemp = pOut;
                unsigned int* piTemp = pTemp->get();
                pOut = new types::UInt32(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::UInt32>();
                unsigned int* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned int>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::UInt32(pIn->getDims(), pIn->getDimsArray());
        unsigned int* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<unsigned int*, unsigned int*> > > V;
        std::vector<std::pair<int, std::pair<unsigned int*, unsigned int*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::UInt32* pTemp = pIn;
            unsigned int* piTemp = pTemp->get();
            pIn = new types::UInt32(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<unsigned int*, unsigned int*> >(i, std::pair<unsigned int*, unsigned int*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

/*--------------------------- UInt64 -----------------------------------------*/
types::UInt64* gsort(types::UInt64* pIn, types::Double* pDblInd, const std::wstring& wstrWay, const std::wstring& wstrProcess)
{
    types::UInt64* pOut = NULL;

    int iTimes      = 1;
    int iItem       = 0;
    int iPos        = 0;
    int iPosSet     = 0;
    int iPosGet     = 0;
    int iOffset1    = 1;
    int iOffset2    = 1;

    unsigned long long* piIn = pIn->get();

    double* pdblInd = pDblInd ? pDblInd->get() : NULL;

    bool (*pFuncWay)(std::pair<std::pair<int, int>, unsigned long long> left,
                     std::pair<std::pair<int, int>, unsigned long long> right);

    bool (*pFuncWayL)(std::pair<int, std::pair<unsigned long long*, unsigned long long*> > left,
                      std::pair<int, std::pair<unsigned long long*, unsigned long long*> > right);

    if (wstrWay == L"i")
    {
        pFuncWay  = &(increasing);
        pFuncWayL = &(increasing);
    }
    else // strWay == L"d"
    {
        pFuncWay  = &(descendent);
        pFuncWayL = &(descendent);
    }

    if (wstrProcess == L"g")
    {
        iItem = pIn->getSize();
    }
    else if (wstrProcess == L"c" || wstrProcess == L"lr")
    {
        iTimes   = pIn->getRows();
        iItem    = pIn->getCols();
        iOffset1 = pIn->getRows();
    }
    else if (wstrProcess == L"r" || wstrProcess == L"lc")
    {
        iTimes   = pIn->getCols();
        iItem    = pIn->getRows();
        iOffset2 = pIn->getRows();
    }

    if (wstrProcess == L"r" || wstrProcess == L"c" || wstrProcess == L"g")
    {
        if (pDblInd)
        {
            pOut = new types::UInt64(pIn->getDims(), pIn->getDimsArray());
            unsigned long long* piOut = pOut->get();

            std::vector<std::pair<std::pair<int, int>, unsigned long long> > V;
            std::vector<std::pair<std::pair<int, int>, unsigned long long> >::iterator it;

            for (int i = 0; i < iTimes; i++)
            {
                for (int j = 0; j < iItem; j++)
                {
                    iPos = i * iOffset2 + j * iOffset1;
                    V.push_back(std::pair<std::pair<int, int>, unsigned long long>(std::pair<int, int>(iPos, j), piIn[iPos]));
                }

                std::sort(V.begin(), V.end(), (*pFuncWay));

                int j = 0;
                for (it = V.begin(); it != V.end(); it++, j++)
                {
                    iPosSet = i * iOffset2 + j * iOffset1;
                    piOut[iPosSet] = (*it).second;
                    pdblInd[iPosSet] = static_cast<double>((*it).first.second + 1);
                }
                V.clear();
            }
        }
        else
        {
            if (wstrProcess == L"g")
            {
                pOut = pIn->clone()->getAs<types::UInt64>();
                unsigned long long* piOut = pOut->get();
                if (wstrWay == L"i")
                {
                    std::sort(piOut, piOut + pOut->getSize());
                }
                else // strWay == L"d"
                {
                    std::sort(piOut, piOut + pOut->getSize(), std::greater<unsigned long long>());
                }
            }
            else if (wstrProcess == L"c")
            {
                pOut = new types::UInt64(pIn->getCols(), pIn->getRows());
                unsigned long long* piOut = pOut->get();

                // transpose input in output
                int iRows = pIn->getRows();
                int iCols = pIn->getCols();
                for (int i = 0; i < pIn->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piIn[i];
                }

                // sort output
                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned long long>());
                    }
                }

                // transpose output
                types::UInt64* pTemp = pOut;
                unsigned long long* piTemp = pTemp->get();
                pOut = new types::UInt64(pTemp->getCols(), pTemp->getRows());
                piOut = pOut->get();
                iRows = pTemp->getRows();
                iCols = pTemp->getCols();

                for (int i = 0; i < pTemp->getSize(); i++)
                {
                    iPos = (i % iRows) * iCols + (i / iRows);
                    piOut[iPos] = piTemp[i];
                }

                delete pTemp;
            }
            else //wstrProcess == L"r"
            {
                pOut = pIn->clone()->getAs<types::UInt64>();
                unsigned long long* piOut = pOut->get();

                if (wstrWay == L"i")
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)));
                    }
                }
                else // strWay == L"d"
                {
                    for (int i = 0; i < iTimes; i++)
                    {
                        std::sort(piOut + (iItem * i), piOut + (iItem * (i + 1)), std::greater<unsigned long long>());
                    }
                }
            }
        }
    }
    else// wstrProcess == L"lr" and wstrProcess == L"lc"
    {
        pOut = new types::UInt64(pIn->getDims(), pIn->getDimsArray());
        unsigned long long* piOut = pOut->get();

        std::vector<std::pair<int, std::pair<unsigned long long*, unsigned long long*> > > V;
        std::vector<std::pair<int, std::pair<unsigned long long*, unsigned long long*> > >::iterator it;

        if (wstrProcess == L"lr")
        {
            // transpose matrix pIn
            types::UInt64* pTemp = pIn;
            unsigned long long* piTemp = pTemp->get();
            pIn = new types::UInt64(pTemp->getCols(), pTemp->getRows());
            piIn = pIn->get();
            int iRows = pTemp->getRows();
            int iCols = pTemp->getCols();

            for (int i = 0; i < pTemp->getSize(); i++)
            {
                iPos = (i % iRows) * iCols + (i / iRows);
                piIn[iPos] = piTemp[i];
            }
        }

        for (int i = 0; i < iTimes; i++)
        {
            V.push_back(std::pair<int, std::pair<unsigned long long*, unsigned long long*> >(i, std::pair<unsigned long long*, unsigned long long*>(piIn + i * iItem, piIn + (i + 1) * iItem)));
        }

        std::sort(V.begin(), V.end(), (*pFuncWayL));

        int j = 0;
        for (it = V.begin(); it != V.end(); it++, j++)
        {
            for (int i = 0; i < iItem; i++)
            {
                iPosSet = i * iOffset1 + j * iOffset2;
                piOut[iPosSet] = ((*it).second.first)[i];
            }

            if (pDblInd)
            {
                pdblInd[j] = static_cast<double>((*it).first + 1);
            }
        }

        V.clear();
        if (wstrProcess == L"lr")
        {
            delete pIn;
        }
    }

    return pOut;
}

