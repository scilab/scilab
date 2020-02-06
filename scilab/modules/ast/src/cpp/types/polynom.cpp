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
#include <algorithm>
#include "core_math.h"
#include "tostring_common.hxx"
#include "singlepoly.hxx"
#include "polynom.hxx"
#include "configvariable.hxx"

namespace types
{
Polynom::Polynom()
{
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Polynom::Polynom(const std::wstring& _szVarName, int _iRows, int _iCols)
{
    int piDims[2]   = {_iRows, _iCols};
    createPoly(_szVarName, 2, piDims, NULL);
}

Polynom::Polynom(const std::wstring& _szVarName, int _iRows, int _iCols, const int *_piRank)
{
    int piDims[2]   = {_iRows, _iCols};
    createPoly(_szVarName, 2, piDims, _piRank);
}

Polynom::Polynom(const std::wstring& _szVarName, int _iDims, const int* _piDims)
{
    createPoly(_szVarName, _iDims, _piDims, NULL);
}

Polynom::Polynom(const std::wstring& _szVarName, int _iDims, const int* _piDims, const int *_piRank)
{
    createPoly(_szVarName, _iDims, _piDims, _piRank);
}

Polynom::~Polynom()
{
    if (isDeletable() == true)
    {
        deleteAll();
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

bool Polynom::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    *_piSize = 0;
    for (int i = 0; i<getSize(); i++)
    {
        *_piSize += (get(i)->getRank()+1)*sizeof(double);
    }

    *_piSize = *_piSize * (isComplex() ? 2 : 1);
    *_piSizePlusType = *_piSize + getSize()*sizeof(SinglePoly *) + sizeof(*this);
    return true;
}


void Polynom::createPoly(const std::wstring& _szVarName, int _iDims, const int* _piDims, const int *_piRank)
{
    m_szVarName = _szVarName;
    SinglePoly** pPoly = NULL;
    create(_piDims, _iDims, &pPoly, NULL);

    if (_piRank)
    {
        for (int i = 0 ; i < getSize() ; i++)
        {
            double* pReal = NULL;
            m_pRealData[i] = new SinglePoly(&pReal, _piRank[i]);
        }
    }
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Polynom* Polynom::set(int _iPos, SinglePoly* _pS)
{
    if (m_pRealData == NULL || _iPos >= m_iSize)
    {
        return NULL;
    }

    typedef Polynom* (Polynom::*set_t)(int, SinglePoly*);
    Polynom* pIT = checkRef(this, (set_t)&Polynom::set, _iPos, _pS);
    if (pIT != this)
    {
        return pIT;
    }

    bool bComplex = isComplex();

    if (m_pRealData[_iPos])
    {
        delete m_pRealData[_iPos];
    }

    m_pRealData[_iPos] = copyValue(_pS);

    if (_pS->isComplex() && bComplex == false)
    {
        setComplex(true);
    }
    else if (_pS->isComplex() == false && bComplex)
    {
        m_pRealData[_iPos]->setComplex(true);
    }

    return this;
}

Polynom* Polynom::set(int _iRows, int _iCols, SinglePoly* _pS)
{
    return set(_iCols * getRows() + _iRows, _pS);
}

Polynom* Polynom::set(SinglePoly** _pS)
{
    typedef Polynom* (Polynom::*set_t)(SinglePoly**);
    Polynom* pIT = checkRef(this, (set_t)&Polynom::set, _pS);
    if (pIT != this)
    {
        return pIT;
    }

    for (int i = 0 ; i < m_iSize ; i++)
    {
        set(i, _pS[i]);
    }

    return this;
}

Polynom* Polynom::setCoef(int _iRows, int _iCols, Double *_pdblCoef)
{
    int piDims[] = {_iRows, _iCols};
    int iPos = getIndex(piDims);
    return setCoef(iPos, _pdblCoef);
}

Polynom* Polynom::setCoef(int _iIdx, Double *_pdblCoef)
{
    if (_iIdx > m_iSize)
    {
        return NULL;
    }

    typedef Polynom* (Polynom::*setCoef_t)(int, Double*);
    Polynom* pIT = checkRef(this, (setCoef_t)&Polynom::setCoef, _iIdx, _pdblCoef);
    if (pIT != this)
    {
        return pIT;
    }

    /*Get old SinglePoly*/
    m_pRealData[_iIdx]->setRank(_pdblCoef->getSize() - 1);
    m_pRealData[_iIdx]->setCoef(_pdblCoef);

    return this;
}

void Polynom::setZeros()
{
    for (int i = 0; i < m_iSize; i++)
    {
        m_pRealData[i]->setZeros();
    }
}

bool Polynom::getSizes(int *_piSizes)
{
    if (_piSizes == NULL || m_pRealData == NULL)
    {
        return false;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        _piSizes[i] = m_pRealData[i]->getSize();
    }

    return true;
}

bool Polynom::getRank(int *_piRank)
{
    if (_piRank == NULL || m_pRealData == NULL)
    {
        return false;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        _piRank[i] = m_pRealData[i]->getRank();
    }

    return true;
}

void Polynom::whoAmI(void)
{
    std::cout << "types::SinglePoly";
}

std::wstring& Polynom::getVariableName()
{
    return m_szVarName;
}

void Polynom::setVariableName(const std::wstring& _szVarName)
{
    m_szVarName = _szVarName;
}

bool Polynom::isComplex()
{
    if (m_iSize && m_pRealData[0])
    {
        return m_pRealData[0]->isComplex();
    }

    return false;
}

bool Polynom::isComplexElement(int i)
{
    if (i < getSize())
    {
        return get(i)->hasComplexCoef();
    }

    return isComplex();
}

Polynom* Polynom::setComplex(bool _bComplex)
{
    if (_bComplex == isComplex())
    {
        return this;
    }

    typedef Polynom* (Polynom::*setcplx_t)(bool);
    Polynom* pIT = checkRef(this, (setcplx_t)&Polynom::setComplex, _bComplex);
    if (pIT != this)
    {
        return pIT;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        get(i)->setComplex(_bComplex);
    }

    return this;
}

Polynom* Polynom::clone()
{
    Polynom* pMP = new Polynom(getVariableName(), getDims(), getDimsArray());
    for (int i = 0 ; i < getSize() ; i++)
    {
        pMP->set(i, m_pRealData[i]);
    }

    return pMP;
}

bool Polynom::transpose(InternalType *& out)
{
    if (isScalar())
    {
        out = clone();
        return true;
    }

    if (m_iDims == 2)
    {
        int piNewDims[2] = {m_piDims[1], m_piDims[0]};
        Polynom* pPoly = new Polynom(m_szVarName, m_iDims, piNewDims);
        Transposition::transpose_clone(getRows(), getCols(), m_pRealData, pPoly->get());
        out = pPoly;
        return true;
    }

    return false;

}

bool Polynom::adjoint(InternalType *& out)
{
    if (isComplex())
    {
        if (m_iDims == 2)
        {
            int piNewDims[2] = {m_piDims[1], m_piDims[0]};
            Polynom* pPoly = new Polynom(m_szVarName, m_iDims, piNewDims);
            Transposition::adjoint_clone(getRows(), getCols(), m_pRealData, pPoly->get());
            out = pPoly;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return transpose(out);
    }
}

Double* Polynom::evaluate(Double* _pdblValue)
{
    double *pR = _pdblValue->getReal();
    double *pI = _pdblValue->getImg();
    int iRows  = _pdblValue->getRows();
    int iCols  = _pdblValue->getCols();

    double *pReturnR = NULL;
    double *pReturnI = NULL;
    Double *pReturn  = new Double(getRows() * iRows, getCols() * iCols, &pReturnR, &pReturnI);
    pReturn->setComplex(_pdblValue->isComplex());

    int i = 0;
    //all lines of the matrix remplacement
    for (int iCol = 0 ; iCol < iCols ; iCol++)
    {
        for (int iPolyCol = 0 ; iPolyCol < getCols() ; iPolyCol++)
        {
            for (int iRow = 0 ; iRow < iRows ; iRow++)
            {
                for (int iPolyRow = 0 ; iPolyRow < getRows() ; iPolyRow++)
                {
                    double OutR = 0;
                    double OutI = 0;

                    SinglePoly *pPoly = get(iPolyRow, iPolyCol);
                    if (pReturn->isComplex())
                    {
                        pPoly->evaluate(pR[iCol * iRows + iRow], pI[iCol * iRows + iRow], &OutR, &OutI);
                        pReturnR[i] = OutR;
                        pReturnI[i] = OutI;
                    }
                    else
                    {
                        pPoly->evaluate(pR[iCol * iRows + iRow], 0, &OutR, &OutI);
                        pReturnR[i] = OutR;
                    }
                    i++;
                }
            }
        }
    }
    return pReturn;
}

void Polynom::updateRank(void)
{
    for (int i = 0 ; i < getSize() ; i++)
    {
        m_pRealData[i]->updateRank();
    }
}

int Polynom::getMaxRank(void)
{
    int *piRank = new int[getSize()];
    getRank(piRank);
    int iMaxRank = 0;
    for (int i = 0 ; i < getSize() ; i++)
    {
        iMaxRank = std::max(iMaxRank, piRank[i]);
    }
    delete[] piRank;
    return iMaxRank;
}

Double* Polynom::getCoef(void)
{
    int iMaxRank = getMaxRank();
    int iColsOut = getCols() * (iMaxRank + 1);

    Double *pCoef = new Double(getRows(), iColsOut, isComplex());
    pCoef->setZeros();
    double *pCoefR = pCoef->getReal();

    if (isComplex())
    {
        double *pCoefI = pCoef->getImg();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            int iSize = pPoly->getSize();
            double *pR = pPoly->get();
            double *pI = pPoly->getImg();

            for (int iRank = 0 ; iRank < iSize ; iRank++)
            {
                pCoefR[iRank * m_iSize + i] = pR[iRank];
                pCoefI[iRank * m_iSize + i] = pI[iRank];
            }
        }
    }
    else
    {
        for (int i = 0 ; i < m_iSize ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            int iSize = pPoly->getSize();
            double *pR = pPoly->get();
            for (int iRank = 0 ; iRank < iSize ; iRank++)
            {
                pCoefR[iRank * m_iSize + i] = pR[iRank];
            }
        }
    }

    return pCoef;
}

Polynom* Polynom::setCoef(Double *_pCoef)
{
    typedef Polynom* (Polynom::*setCoef_t)(Double*);
    Polynom* pIT = checkRef(this, (setCoef_t)&Polynom::setCoef, _pCoef);
    if (pIT != this)
    {
        return pIT;
    }

    setComplex(_pCoef->isComplex());
    double *pR = _pCoef->getReal();

    if (isComplex())
    {
        double *pI = _pCoef->getImg();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            int iSize = pPoly->getSize();
            double* pTempR = pPoly->get();
            double* pTempI = pPoly->getImg();

            for (int iRank = 0 ; iRank < iSize ; iRank++)
            {
                pTempR[iRank] = pR[iRank * m_iSize + i];
                pTempI[iRank] = pI[iRank * m_iSize + i];
            }
        }
    }
    else
    {
        for (int i = 0 ; i < m_iSize ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            int iSize = pPoly->getSize();
            double* pTempR = pPoly->get();

            for (int iRank = 0 ; iRank < iSize ; iRank++)
            {
                pTempR[iRank] = pR[iRank * m_iSize + i];
            }
        }
    }

    return this;
}

bool Polynom::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims)
{

    ostr << getMatrixString(_piDims, _iDims, false);

    return true;
}

std::wstring Polynom::getMatrixString(int* _piDims, int /*_iDims*/, bool _bComplex)
{
    int iLineLen = ConfigVariable::getConsoleWidth();

    std::wostringstream ostr;
    std::wostringstream osPoly;

    std::list<std::wstring> listWstPoly;

    int iLen        = 0;
    int iLastCol    = 0;
    bool bWordWarp  = false;

    int *piMaxLen = new int[abs(getCols())];
    memset(piMaxLen, 0x00, sizeof(int) * abs(getCols()));

    //find the largest row for each col
    for (int iCols1 = 0 ; iCols1 < abs(getCols()) ; iCols1++)
    {
        for (int iRows1 = 0 ; iRows1 < abs(getRows()) ; iRows1++)
        {
            int iLength = 0;
            _piDims[0] = iRows1;
            _piDims[1] = iCols1;
            int iPos = getIndex(_piDims);
            get(iPos)->toStringRealImg(getVariableName(), &listWstPoly, iLineLen);

            for (auto it : listWstPoly)
            {
                iLength += static_cast<int>(it.size());
            }
            piMaxLen[iCols1] = std::min(std::max(piMaxLen[iCols1], iLength), iLineLen);
            listWstPoly.clear();
        }

        //We know the length of the column
        if (static_cast<int>(iLen + piMaxLen[iCols1]) + 2 >= iLineLen && iLen != 0)
        {
            //if the max length exceeded
            std::wostringstream ostemp;
            bWordWarp = true;
            for (int iRows2 = 0 ; iRows2 < abs(getRows()) ; iRows2++)
            {
                bool bMultiLine = false;
                for (int iCols2 = iLastCol ; iCols2 < iCols1; iCols2++)
                {
                    _piDims[0] = iRows2;
                    _piDims[1] = iCols2;

                    int iPos = getIndex(_piDims);
                    get(iPos)->toStringRealImg(getVariableName(),  &listWstPoly, iLineLen);

                    if (listWstPoly.size() > 1)
                    {
                        for (auto it : listWstPoly)
                        {
                            osPoly << L"  " << it << std::endl;
                            bMultiLine = true;
                        }
                    }
                    else
                    {
                        osPoly << L"  " << listWstPoly.front();
                        addSpaces(&osPoly, piMaxLen[iCols2] - static_cast<int>(listWstPoly.front().size()));
                        bMultiLine = false;
                    }
                    listWstPoly.clear();
                }

                if (bMultiLine == false)
                {
                    osPoly << std::endl;
                }
                if (iRows2 < abs(getRows())-1 &&  ConfigVariable::isPrintCompact() == false)
                {
                    osPoly << std::endl;
                }

                ostemp << osPoly.str() ;
                osPoly.str(L"");

            }
            iLen = piMaxLen[iCols1];

            //write "column x to y"
            addColumnString(ostr, iLastCol + 1, iCols1);
            ostr << ostemp.str() << std::endl;

            iLastCol = iCols1;
        }
        else //if((int)(iLen + piMaxLen[iCols1]) <= iLineLen)
        {
            iLen += piMaxLen[iCols1];
        }
    }//for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)

    if (bWordWarp)
    {
        addColumnString(ostr, iLastCol + 1, getCols());
    }

    //print the end
    for (int iRows2 = 0 ; iRows2 < abs(getRows()) ; iRows2++)
    {
        bool bMultiLine = false;
        for (int iCols2 = iLastCol ; iCols2 < abs(getCols()) ; iCols2++)
        {
            _piDims[0] = iRows2;
            _piDims[1] = iCols2;

            int iPos = getIndex(_piDims);
            get(iPos)->toStringRealImg(getVariableName(), &listWstPoly, iLineLen);

            if (listWstPoly.size() > 1)
            {
                for (auto it : listWstPoly)
                {
                    osPoly << L"  " << it << std::endl;
                    bMultiLine = true;
                }
            }
            else
            {
                osPoly << L"  " << listWstPoly.front();
                addSpaces(&osPoly, piMaxLen[iCols2] - static_cast<int>(listWstPoly.front().size()));
            }
            listWstPoly.clear();
        }

        if (bMultiLine == false)
        {
            osPoly << std::endl;
        }
        if (iRows2 < abs(getRows())-1 &&  ConfigVariable::isPrintCompact() == false )
        {
            osPoly << std::endl;
        }

        if (isIdentity())
        {
            ostr << L"eye *" << std::endl << std::endl;
        }
        ostr << osPoly.str() ;
        osPoly.str(L"");
    }

    delete[] piMaxLen;
    return ostr.str();
}


Double* Polynom::extractCoef(int _iRank)
{
    Double *pdbl = new Double(getRows(), getCols(), isComplex());
    pdbl->setZeros();
    double *pReal = pdbl->getReal();

    if (isComplex())
    {
        double *pImg = pdbl->getImg();
        for (int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            if (pPoly->getRank() >= _iRank)
            {
                pReal[i] = pPoly->get()[_iRank];
                pImg[i]  = pPoly->getImg()[_iRank];
            }
        }
    }
    else
    {
        for (int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            if (pPoly->getRank() >= _iRank)
            {
                pReal[i] = pPoly->get()[_iRank];
            }
        }
    }

    return pdbl;
}

bool Polynom::insertCoef(int _iRank, Double* _pCoef)
{
    double *pReal = _pCoef->getReal();
    if (isComplex())
    {
        double *pImg  = _pCoef->getImg();
        for (int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            if (pPoly->getRank() <= _iRank)
            {
                return false;
            }

            pPoly->get()[_iRank] = pReal[i];
            pPoly->getImg()[_iRank] = pImg[i];
        }
    }
    else
    {
        for (int i = 0 ; i < getSize() ; i++)
        {
            SinglePoly *pPoly = m_pRealData[i];
            if (pPoly->getRank() <= _iRank)
            {
                return false;
            }

            pPoly->get()[_iRank] = pReal[i];
        }
    }

    return true;
}

bool Polynom::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isPoly() == false)
    {
        return false;
    }

    Polynom* pM = const_cast<InternalType &>(it).getAs<types::Polynom>();

    if (pM->getRows() != getRows() || pM->getCols() != getCols())
    {
        return false;
    }

    for (int i = 0 ; i < getSize() ; i++)
    {
        SinglePoly* p1 = get(i);
        SinglePoly* p2 = pM->get(i);

        if (*p1 != *p2)
        {
            return false;
        }
    }
    return true;
}

bool Polynom::operator!=(const InternalType& it)
{
    return !(*this == it);
}

SinglePoly* Polynom::getNullValue()
{
    return new SinglePoly();
}

Polynom* Polynom::createEmpty(int _iDims, int* _piDims, bool /*_bComplex*/)
{
    return new Polynom(getVariableName(), _iDims, _piDims, NULL);
}

SinglePoly* Polynom::copyValue(SinglePoly* _pData)
{
    if (_pData == NULL)
    {
        return NULL;
    }
    return _pData->clone();
}

void Polynom::deleteAll()
{
    for (int i = 0 ; i < m_iSizeMax ; i++)
    {
        m_pRealData[i]->killMe();
    }
    delete[] m_pRealData;
    m_pRealData = NULL;
    deleteImg();
}

void Polynom::deleteImg()
{

}

SinglePoly** Polynom::allocData(int _iSize)
{
    SinglePoly** pData = new SinglePoly*[_iSize];
    memset(pData, 0x00, _iSize * sizeof(SinglePoly*));
    return pData;
}

void Polynom::deleteData(SinglePoly* data)
{
    if (data)
    {
        data->killMe();
    }
}

//overload to check variable name and call arrayof<>::insert after
Polynom* Polynom::insert(typed_list* _pArgs, InternalType* _pSource)
{
    Polynom* p = _pSource->getAs<Polynom>();
    if (p->getVariableName() != getVariableName())
    {
        char szError[512];
        os_sprintf(szError, _("Input arguments should have the same formal variable name.\n"));
        wchar_t* pwstError = to_wide_string(szError);
        std::wstring wstError(pwstError);
        FREE(pwstError);
        throw ast::InternalError(wstError);
    }

    return ArrayOf<SinglePoly*>::insert(_pArgs, _pSource)->getAs<Polynom>();
}

Polynom* Polynom::Dollar()
{
    int iRank = 1;
    Polynom* pDollar = new Polynom(L"$", 1, 1, &iRank);
    double* pdblCoef = pDollar->get(0)->get();
    pdblCoef[0] = 0;
    pdblCoef[1] = 1;

    return pDollar;
}

bool Polynom::isDollar()
{
    if (m_szVarName != L"$" || getSize() != 1)
    {
        return false;
    }

    double* pCoef = get(0)->get();

    if (pCoef[0] != 0 && pCoef[1] != 1)
    {
        return false;
    }

    return true;
}

}

