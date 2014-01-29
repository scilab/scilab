/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <sstream>
#include <math.h>
#include "singlepoly.hxx"
#include "double.hxx"
#include "tostring_common.hxx"
#include "formatmode.h"

extern "C"
{
#include "log.h"
#include "exp.h"
#include "elem_common.h"
}

using namespace std;

namespace types
{
SinglePoly::SinglePoly()
{
    int iRank = 1;
    double* pdblCoefR = NULL;
    createPoly(&pdblCoefR, NULL, iRank);
    pdblCoefR[0] = 0;
}

SinglePoly::SinglePoly(double** _pdblCoefR, int _iRank)
{
    createPoly(_pdblCoefR, NULL, _iRank);
}

SinglePoly::SinglePoly(double** _pdblCoefR, double** _pdblCoefI, int _iRank)
{
    createPoly(_pdblCoefR, _pdblCoefI, _iRank);
}

SinglePoly::SinglePoly(Double** _poCoefR, int _iRank)
{
    double *pR	= NULL;
    double *pI	= NULL;
    createPoly(&pR, &pI, _iRank);
    *_poCoefR = m_pdblCoef;
}


SinglePoly::~SinglePoly()
{
    if (m_pdblCoef)
    {
        delete m_pdblCoef;
    }
}

/*Real constructor, private only*/
void SinglePoly::createPoly(double** _pdblCoefR, double** _pdblCoefI, int _iRank)
{
    double *pR	= NULL;
    double *pI	= NULL;
    m_bComplex	= false;
    m_iRank     = _iRank;

    if (m_iRank == 0)
    {
        m_pdblCoef = NULL;
        return;
    }

    if (_pdblCoefI != NULL)
    {
        m_pdblCoef = new Double(1, _iRank, &pR, &pI);
    }
    else
    {
        m_pdblCoef = new Double(1, _iRank, &pR);
    }

    m_pdblCoef->IncreaseRef();
    m_pdblCoef->setZeros();
    if (_pdblCoefR != NULL)
    {
        *_pdblCoefR = pR;
    }

    if (_pdblCoefI != NULL)
    {
        m_bComplex = true;
        *_pdblCoefI = pI;
    }
}

int SinglePoly::getRank()
{
    return m_iRank;
}

int SinglePoly::getRealRank()
{
    return m_iRank - 1;
}

bool SinglePoly::setRank(int _iRank, bool bSave)
{
    double *pR	= NULL;
    double *pI	= NULL;
    if (bSave == false)
    {
        if (m_iRank != _iRank)
        {
            delete m_pdblCoef;

            if (m_bComplex == false)
            {
                createPoly(&pR, NULL, _iRank);
            }
            else
            {
                createPoly(&pR, &pI, _iRank);
            }
            return true;
        }
        return true;
    }
    else
    {
        Double *pCoef = NULL;
        if (_iRank != 0)
        {
            pCoef = new Double(1, _iRank, &pR, &pI);
            pCoef->set(m_pdblCoef->getReal());
            pCoef->setComplex(m_pdblCoef->isComplex());
            if (m_pdblCoef->isComplex())
            {
                pCoef->setImg(m_pdblCoef->getImg());
            }
            m_iRank = _iRank;
        }
        else
        {
            m_iRank = 1;
            pCoef = new Double(1, 1, &pR, &pI);
            pCoef->setComplex(m_pdblCoef->isComplex());
            pCoef->set(0, 0, 0);
        }

        m_pdblCoef->DecreaseRef();
        if (m_pdblCoef->isDeletable())
        {
            delete m_pdblCoef;
        }

        m_pdblCoef = pCoef;
        m_pdblCoef->IncreaseRef();
        return true;
    }
    return false;
}

Double* SinglePoly::getCoef()
{
    return m_pdblCoef;
}

double* SinglePoly::getCoefReal()
{
    return m_pdblCoef->getReal();
}

double* SinglePoly::getCoefImg()
{
    return m_pdblCoef->getImg();
}

bool SinglePoly::setCoef(Double* _pdblCoefR)
{
    if (m_pdblCoef == NULL || _pdblCoefR == NULL)
    {
        return false;
    }

    double *pInR	= _pdblCoefR->getReal();
    double *pInI	= _pdblCoefR->getImg();

    return setCoef(pInR, pInI);
}

void SinglePoly::setComplex(bool _bComplex)
{
    m_pdblCoef->setComplex(_bComplex);
    m_bComplex = _bComplex;
}

bool SinglePoly::setCoef(double* _pdblCoefR, double* _pdblCoefI)
{
    if (m_pdblCoef == NULL)
    {
        return false;
    }

    if (_pdblCoefI != NULL && isComplex() == false)
    {
        m_pdblCoef->setComplex(true);
        m_bComplex = true;
    }

    double *pR = m_pdblCoef->getReal();
    double *pI = m_pdblCoef->getImg();

    if (_pdblCoefR != NULL && pR != NULL)
    {
        memcpy(pR, _pdblCoefR, m_iRank * sizeof(double));
    }

    if (_pdblCoefI != NULL && pI != NULL)
    {
        memcpy(pI, _pdblCoefI, m_iRank * sizeof(double));
    }

    return true;
}

void SinglePoly::whoAmI()
{
    std::cout << "types::SinglePoly";
}

bool SinglePoly::isComplex()
{
    return m_bComplex;
}

GenericType* SinglePoly::getColumnValues(int _iPos)
{
    return NULL;
}

GenericType::RealType SinglePoly::getType(void)
{
    return RealSinglePoly;
}

bool SinglePoly::evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI)
{
    double *pCoefR = m_pdblCoef->getReal();
    double *pCoefI = m_pdblCoef->getImg();

    *_pdblOutR = 0;
    *_pdblOutI = 0;
    if (m_iRank == 0)
    {
        return true;
    }

    for (int i = 0 ; i < m_iRank ; i++)
    {
        //real part
        *_pdblOutR += pCoefR[i] * pow(_dblInR, i);
        //only if variable is complex
        if (m_pdblCoef->isComplex())
        {
            *_pdblOutR -= pCoefI[i] * pow(_dblInI, i);
            //img part
            *_pdblOutI += pCoefI[i] * pow(_dblInR, i);
        }
        *_pdblOutI += pCoefR[i] * pow(_dblInI, i);
    }

    //old version, does not work
    //for(int i = m_iRank - 1 ; i >= 0 ; i--)
    //{
    //	//(a1 + ib1)(a2 + ib2)**n = (a1 + ib1) * exp(n * log(a2 + ib2))
    //	double dblLogR = 0;
    //	double dblLogI = 0;
    //	double dblExpR = 0;
    //	double dblExpI = 0;

    //	//log(a2 + ib2)
    //	if(_dblInI != 0)
    //	{
    //		wlog(_dblInR, _dblInI, &dblLogR, &dblLogI);
    //	}
    //	else
    //	{
    //		dblLogR = dlogs(_dblInR);
    //	}

    //	//n * log(a2 + ib2)
    //	dblLogR *= i;
    //	dblLogI *= i;

    //	//exp(n * log(a2 + ib2))
    //	if(dblLogI != 0)
    //	{
    //		zexps(dblLogR, dblLogI, &dblExpR, &dblExpI);
    //	}
    //	else
    //	{
    //		dblExpR = dexps(dblLogR);
    //	}

    //	//(a1 + ib1) * exp(n * log(a2 + ib2))
    //	if(m_pdblCoef->isComplex())
    //	{
    //		*_pdblOutR += (dblExpR * pCoefR[i] - dblExpI * pCoefI[i]);
    //		*_pdblOutI += (dblExpR * pCoefI[i] + dblExpI * pCoefR[i]);
    //	}
    //	else
    //	{
    //		*_pdblOutR += (dblExpR * pCoefR[i]);
    //		*_pdblOutI += (dblExpI * pCoefR[i]);
    //	}
    //}
    return true;
}

void SinglePoly::updateRank(void)
{
    double dblEps = getRelativeMachinePrecision();
    int iNewRank = m_iRank;
    double *pCoefR = getCoef()->getReal();
    double *pCoefI = getCoef()->getImg();
    for (int i = m_iRank - 1; i > 0 ; i--)
    {
        if (fabs(pCoefR[i]) <= dblEps && (pCoefI != NULL ? fabs(pCoefI[i]) : 0) <= dblEps)
        {
            iNewRank--;
        }
        else
        {
            break;
        }
    }
    if (iNewRank < m_iRank)
    {
        setRank(iNewRank, true);
    }
}

bool SinglePoly::toString(std::wostringstream& ostr)
{
    ostr << L"FIXME : implement SinglePoly::toString" << std::endl;
    return true;
}

void SinglePoly::toStringReal(wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
{
    toStringInternal(m_pdblCoef->getReal(), _szVar, _pListExp, _pListCoef);
}

void SinglePoly::toStringImg(wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
{
    if (isComplex() == false)
    {
        _pListExp->clear();
        _pListCoef->clear();
        return;
    }

    toStringInternal(m_pdblCoef->getImg(), _szVar, _pListExp, _pListCoef);
}

void SinglePoly::toStringInternal(double *_pdblVal, wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
{
    int iPrecision = getFormatSize();
    int iLineLen = getConsoleWidth();

    wostringstream ostemp;
    wostringstream ostemp2;

    ostemp << L"  ";

    //to add exponant value a the good place
    int *piIndexExp = new int[m_iRank];

    int iLen				= 0;
    int iLastFlush	= 2;
    for (int i = 0 ; i < m_iRank ; i++)
    {
        piIndexExp[i] = 0;
        if (isRealZero(_pdblVal[i]) == false)
        {
            DoubleFormat df;
            getDoubleFormat(_pdblVal[i], &df);

            if (iLen + df.iWidth + 2 >= iLineLen)
            {
                //flush
                for (int j = iLastFlush ; j < i ; j++)
                {
                    if (piIndexExp[j] == 0)
                    {
                        continue;
                    }

                    addSpaces(&ostemp2, piIndexExp[j] - static_cast<int>(ostemp2.str().size()));
                    if (isRealZero(_pdblVal[j]) == false)
                    {
                        ostemp2 << j;
                    }
                }
                iLastFlush = i;
                _pListExp->push_back(ostemp2.str());
                ostemp2.str(L""); //reset stream
                addSpaces(&ostemp2, 12); //take from scilab ... why not ...

                _pListCoef->push_back(ostemp.str());
                ostemp.str(L""); //reset stream
                addSpaces(&ostemp, 12); //take from scilab ... why not ...
            }

            bool bFirst = ostemp.str().size() == 2;

            df.bPrintPoint = false;
            df.bPrintPlusSign = ostemp.str().size() != 2;
            df.bPrintOne = i == 0;
            addDoubleValue(&ostemp, _pdblVal[i], &df);

            if (i != 0)
            {
                ostemp << _szVar;
                piIndexExp[i] = static_cast<int>(ostemp.str().size());
            }
            ostemp << L" ";
            iLen = static_cast<int>(ostemp.str().size());
        }
    }

    if (iLastFlush != 0)
    {
        for (int j = iLastFlush ; j < m_iRank ; j++)
        {
            if (piIndexExp[j] == 0)
            {
                continue;
            }

            addSpaces(&ostemp2, piIndexExp[j] - static_cast<int>(ostemp2.str().size()));
            if (isRealZero(_pdblVal[j]) == false)
            {
                ostemp2 << j;
            }
        }

        if (ostemp.str() == L"  ")
        {
            ostemp << L"  0";
            addSpaces(&ostemp2, static_cast<int>(ostemp.str().size()));
        }

        _pListExp->push_back(ostemp2.str());
        _pListCoef->push_back(ostemp.str());
    }

    delete[] piIndexExp;
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

    return *(getCoef()) == *(pP->getCoef());
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
        pPoly->setCoef(getCoefReal(), getCoefImg());
    }
    else
    {
        pPoly = new SinglePoly(&pR, getRank());
        pPoly->setCoef(getCoefReal(), NULL);
    }
    return pPoly;
}
}


