/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
#include <sstream>
#include <cmath>
#include "singlepoly.hxx"
#include "double.hxx"
#include "tostring_common.hxx"
#include "configvariable.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "log.h"
#include "exp.h"
#include "elem_common.h"
}

namespace types
{
SinglePoly::SinglePoly()
{
    double* pdblCoefR = NULL;
    int piDims[2] = {1, 1};
    create(piDims, 2, &pdblCoefR, NULL);
    pdblCoefR[0] = 0;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SinglePoly::SinglePoly(double** _pdblCoefR, int _iRank)
{
    int piDims[2] = {_iRank + 1, 1};
    create(piDims, 2, _pdblCoefR, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SinglePoly::SinglePoly(double** _pdblCoefR, double** _pdblCoefI, int _iRank)
{
    int piDims[2] = {_iRank + 1, 1};
    create(piDims, 2, _pdblCoefR, _pdblCoefI);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SinglePoly::~SinglePoly()
{
    deleteAll();
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

void SinglePoly::deleteAll()
{
    delete[] m_pRealData;
    m_pRealData = NULL;
    deleteImg();
}

void SinglePoly::deleteImg()
{
    if (m_pImgData != NULL)
    {
        delete[] m_pImgData;
        m_pImgData = NULL;
    }
}

double SinglePoly::getNullValue()
{
    return 0;
}

SinglePoly* SinglePoly::createEmpty(int /*_iDims*/, int* _piDims, bool _bComplex)
{
    double* pdblData = NULL;
    SinglePoly* pSP = new SinglePoly(&pdblData, _piDims[0] - 1);
    pSP->setComplex(_bComplex);

    return pSP;
}

double* SinglePoly::allocData(int _iSize)
{
    double* pDbl = NULL;
    try
    {
        if (_iSize < 0)
        {
            m_pRealData = NULL;
            m_pImgData = NULL;
            char message[bsiz];
            os_sprintf(message, _("Can not allocate negative size (%d).\n"),  _iSize);
            throw ast::InternalError(message);
        }
        else
        {
            pDbl = new double[_iSize];
        }
    }
    catch (std::bad_alloc &/*e*/)
    {
        char message[bsiz];
        os_sprintf(message, _("Can not allocate %.2f MB memory.\n"),  (double) (_iSize * sizeof(double)) / 1.e6);
        throw ast::InternalError(message);
    }

    return pDbl;
}

double SinglePoly::copyValue(double _dblData)
{
    return _dblData;
}

int SinglePoly::getRank()
{
    return m_iSize - 1;
}

bool SinglePoly::setRank(int _iRank, bool bSave)
{
    double *pR = NULL;
    double *pI = NULL;
    if (bSave == false)
    {
        if (getRank() != _iRank)
        {
            int piDims[2] = {_iRank + 1, 1};
            if (m_pImgData == NULL)
            {
                deleteAll();
                create(piDims, 2, &pR, NULL);
            }
            else
            {
                deleteAll();
                create(piDims, 2, &pR, &pI);
            }

            return true;
        }

        return true;
    }
    else
    {
        double* pdblOldReal = m_pRealData;
        double* pdblOldImg  = m_pImgData;
        int iMinSize = Min(m_iSize, _iRank + 1);
        int piDims[2] = {_iRank + 1, 1};

        if (m_pImgData == NULL)
        {
            create(piDims, 2, &pR, NULL);
        }
        else
        {
            create(piDims, 2, &pR, &pI);
            memcpy(m_pImgData, pdblOldImg, iMinSize * sizeof(double));
        }

        memcpy(m_pRealData, pdblOldReal, iMinSize * sizeof(double));

        if (pdblOldImg)
        {
            delete[] pdblOldImg;
            pdblOldImg = NULL;
        }

        delete[] pdblOldReal;
        pdblOldReal = NULL;

        return true;
    }

    return false;
}

bool SinglePoly::setZeros()
{
    if (m_pRealData != NULL)
    {
        memset(m_pRealData, 0x00, m_iSize * sizeof(double));
    }
    else
    {
        return false;
    }

    if (isComplex() == true)
    {
        if (m_pImgData != NULL)
        {
            memset(m_pImgData, 0x00, m_iSize * sizeof(double));
        }
        else
        {
            return false;
        }
    }

    return true;
}

bool SinglePoly::setCoef(Double* _pdblCoefR)
{
    if (m_pRealData == NULL || _pdblCoefR == NULL)
    {
        return false;
    }

    double *pInR    = _pdblCoefR->getReal();
    double *pInI    = _pdblCoefR->getImg();

    return setCoef(pInR, pInI);
}

bool SinglePoly::setCoef(const double* _pdblCoefR, const double* _pdblCoefI)
{
    if (_pdblCoefI != NULL && isComplex() == false)
    {
        setComplex(true);
    }

    if (_pdblCoefR != NULL)
    {
        memcpy(m_pRealData, _pdblCoefR, m_iSize * sizeof(double));
    }

    if (_pdblCoefI != NULL)
    {
        memcpy(m_pImgData, _pdblCoefI, m_iSize * sizeof(double));
    }

    return true;
}

void SinglePoly::whoAmI()
{
    std::cout << "types::SinglePoly";
}

bool SinglePoly::evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI)
{
    *_pdblOutR = 0;
    *_pdblOutI = 0;
    if (m_iSize == 0)
    {
        return true;
    }

    for (int i = 0 ; i < m_iSize ; i++)
    {
        //real part
        *_pdblOutR += m_pRealData[i] * std::pow(_dblInR, i);
        //only if variable is complex
        if (isComplex())
        {
            *_pdblOutR -= m_pImgData[i] * std::pow(_dblInI, i);
            //img part
            *_pdblOutI += m_pRealData[i] * std::pow(_dblInR, i);
        }
        *_pdblOutI += m_pRealData[i] * std::pow(_dblInI, i);
    }

    return true;
}

void SinglePoly::updateRank(void)
{
    int iNewRank = getRank();
    if (m_pImgData)
    {
        for (int i = getRank(); i > 0 ; i--)
        {
            if (std::fabs(m_pRealData[i]) == 0.0 && std::fabs(m_pImgData[i]) == 0.0)
            {
                iNewRank--;
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        for (int i = getRank(); i > 0 ; i--)
        {
            if (std::fabs(m_pRealData[i]) == 0.0)
            {
                iNewRank--;
            }
            else
            {
                break;
            }
        }
    }

    if (iNewRank < getRank())
    {
        setRank(iNewRank, true);
    }
}

bool SinglePoly::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : implement SinglePoly::toString" << std::endl;
    return true;
}

void SinglePoly::toStringReal(const std::wstring& _szVar, std::list<std::wstring>* _pListExp, std::list<std::wstring>* _pListCoef)
{
    toStringInternal(m_pRealData, _szVar, _pListExp, _pListCoef);
}

void SinglePoly::toStringImg(const std::wstring& _szVar, std::list<std::wstring>* _pListExp, std::list<std::wstring>* _pListCoef)
{
    if (isComplex() == false)
    {
        _pListExp->clear();
        _pListCoef->clear();
        return;
    }

    toStringInternal(m_pImgData, _szVar, _pListExp, _pListCoef);
}

bool SinglePoly::subMatrixToString(std::wostringstream& /*ostr*/, int* /*_piDims*/, int /*_iDims*/)
{
    return false;
}

void SinglePoly::toStringInternal(double *_pdblVal, const std::wstring& _szVar, std::list<std::wstring>* _pListExp, std::list<std::wstring>* _pListCoef)
{
    int iLineLen = ConfigVariable::getConsoleWidth();

    std::wostringstream ostemp;
    std::wostringstream ostemp2;

    ostemp << L" ";
    ostemp2 << L" ";

    int iLen = 0;
    int iLastFlush = 2;
    for (int i = 0 ; i < m_iSize ; i++)
    {
        if (_pdblVal[i] != 0)
        {
            DoubleFormat df;
            getDoubleFormat(_pdblVal[i], &df);

            if (iLen + df.iWidth + df.iSignLen >= iLineLen - 1)
            {
                iLastFlush = i;
                _pListExp->push_back(ostemp2.str());
                ostemp2.str(L""); //reset stream
                addSpaces(&ostemp2, 11); //take from scilab ... why not ...

                _pListCoef->push_back(ostemp.str());
                ostemp.str(L""); //reset stream
                addSpaces(&ostemp, 11); //take from scilab ... why not ...
            }

            bool bFirst = ostemp.str().size() == 1;

            // In scientific notation case bExp == true, so we have to print point (2.000D+10s)
            // In other case don't print point (2s)
            df.bPrintPoint = df.bExp;
            df.bPrintPlusSign = bFirst == false;
            df.bPrintOne = i == 0;
            addDoubleValue(&ostemp, _pdblVal[i], &df);

            if (i == 0)
            {
                iLen = static_cast<int>(ostemp.str().size());
            }
            else if (i == 1)
            {
                // add polynom name
                ostemp << _szVar;
                iLen = static_cast<int>(ostemp.str().size());
            }
            else
            {
                // add polynom name and exponent
                ostemp << _szVar;
                iLen = static_cast<int>(ostemp.str().size());
                addSpaces(&ostemp2, iLen - static_cast<int>(ostemp2.str().size()));
                ostemp2 << i;
                int iSize = static_cast<int>(ostemp2.str().size()) - iLen;
                addSpaces(&ostemp, iSize);
            }
        }
    }

    if (iLastFlush != 0)
    {
        if (ostemp.str() == L" ")
        {
            ostemp << L"  0";
            addSpaces(&ostemp2, static_cast<int>(ostemp.str().size()));
        }

        if (ostemp2.str() == L" ")
        {
            // -1 because ostemp2 have already a space
            addSpaces(&ostemp2, static_cast<int>(ostemp.str().size()) - 1);
        }

        _pListExp->push_back(ostemp2.str());
        _pListCoef->push_back(ostemp.str());
    }

    return;
}

bool SinglePoly::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isSinglePoly() == false)
    {
        return false;
    }

    SinglePoly* pP = const_cast<InternalType &>(it).getAs<SinglePoly>();

    if (getRank() != pP->getRank())
    {
        return false;
    }

    double *pdblReal = pP->get();
    for (int i = 0 ; i < getSize() ; i++)
    {
        if (m_pRealData[i] != pdblReal[i])
        {
            return false;
        }
    }

    //both complex
    if (isComplex() && pP->isComplex())
    {
        double *pdblImg = pP->getImg();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            if (m_pImgData[i] != pdblImg[i])
            {
                return false;
            }
        }
    }
    //pdbl complex check all img values == 0
    else if (pP->isComplex())
    {
        double *pdblImg = pP->getImg();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            if (pdblImg[i])
            {
                return false;
            }
        }
    }
    //complex check all img values == 0
    else if (isComplex())
    {
        for (int i = 0 ; i < m_iSize ; i++)
        {
            if (m_pImgData[i])
            {
                return false;
            }
        }
    }

    return true;
}

bool SinglePoly::operator!=(const InternalType& it)
{
    return !(*this == it);
}

SinglePoly* SinglePoly::clone()
{
    SinglePoly* pPoly = NULL;
    double *pR = NULL;
    if (isComplex())
    {
        double *pI = NULL;
        pPoly = new SinglePoly(&pR, &pI, getRank());
        pPoly->setCoef(m_pRealData, m_pImgData);
    }
    else
    {
        pPoly = new SinglePoly(&pR, getRank());
        pPoly->setCoef(m_pRealData, NULL);
    }
    return pPoly;
}

SinglePoly* SinglePoly::conjugate()
{
    if (isComplex())
    {
        double *pR = NULL;
        double *pI = NULL;
        SinglePoly * pPoly = new SinglePoly(&pR, &pI, getRank());

        Transposition::conjugate(m_iSize, m_pRealData, pR, m_pImgData, pI);

        return pPoly;
    }
    else
    {
        return clone();
    }
}

SinglePoly* operator*(const SinglePoly& _lhs, const SinglePoly& _rhs)
{
    SinglePoly& lhs = const_cast<SinglePoly &>(_lhs);
    SinglePoly& rhs = const_cast<SinglePoly &>(_rhs);
    SinglePoly* pOut = NULL;

    bool isComplexL = lhs.isComplex();
    bool isComplexR = rhs.isComplex();
    bool isComplexOut = isComplexL || isComplexR;

    int iRankL = lhs.getRank();
    int iRankR = rhs.getRank();
    int iRankOut = lhs.getRank() + rhs.getRank();

    double* pdblOutR = NULL;
    double* pdblOutI = NULL;
    double* pdblLR = lhs.get();
    double* pdblLI = lhs.getImg();
    double* pdblRR = rhs.get();
    double* pdblRI = rhs.getImg();

    if (isComplexOut)
    {
        pOut = new SinglePoly(&pdblOutR, &pdblOutI, iRankOut);
        memset(pdblOutR, 0x00, sizeof(double) * (iRankOut + 1));
        memset(pdblOutI, 0x00, sizeof(double) * (iRankOut + 1));
    }
    else
    {
        pOut = new SinglePoly(&pdblOutR, iRankOut);
        memset(pdblOutR, 0x00, sizeof(double) * (iRankOut + 1));
    }

    if (isComplexL)
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iRankL + 1; ++i)
            {
                for (int j = 0 ; j < iRankR + 1; ++j)
                {
                    pdblOutR[i + j]  += pdblLR[i] * pdblRR[j] - pdblLI[i] * pdblRI[j];
                    pdblOutI[i + j]  += pdblLI[i] * pdblRR[j] + pdblLR[i] * pdblRI[j];
                }
            }
        }
        else
        {
            for (int i = 0 ; i < iRankL + 1 ; ++i)
            {
                for (int j = 0 ; j < iRankR + 1 ; ++j)
                {
                    pdblOutR[i + j]  += pdblLR[i] * pdblRR[j];
                    pdblOutI[i + j]  += pdblLI[i] * pdblRR[j];
                }
            }
        }
    }
    else
    {
        if (isComplexR)
        {
            for (int i = 0 ; i < iRankL + 1 ; ++i)
            {
                for (int j = 0 ; j < iRankR + 1 ; ++j)
                {
                    pdblOutR[i + j]  += pdblLR[i] * pdblRR[j];
                    pdblOutI[i + j]  += pdblLR[i] * pdblRI[j];
                }
            }
        }
        else
        {
            for (int i = 0 ; i < iRankL + 1 ; ++i)
            {
                for (int j = 0 ; j < iRankR + 1 ; ++j)
                {
                    pdblOutR[i + j]  += pdblLR[i] * pdblRR[j];
                }
            }
        }
    }

    return pOut;
}
}



