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
#include <math.h>
#include "double.hxx"
#include "doubleexp.hxx"
#include "tostring_common.hxx"
#include "configvariable.hxx"
#include "type_traits.hxx"
#include "types_tools.hxx"

extern "C"
{
#include <stdio.h>
#include "elem_common.h"
#include "localization.h"
#include "charEncoding.h"
#include "os_string.h"
}

namespace types
{
Double* Double::Empty()
{
    return new Double(0, 0);
}

Double* Double::Identity(int _iRows, int _iCols)
{
    double* pdbl = NULL;
    Double* pI = new Double(_iRows, _iCols, &pdbl);
    pI->setZeros();
    for (int i = 0 ; i < std::min(_iRows, _iCols) ; i++)
    {
        pI->set(i, i, 1);
    }

    if (_iRows == -1 && _iCols == -1)
    {
        pdbl[0] = 1;
    }
    return pI;
}

Double* Double::Identity(int _iDims, const int* _piDims)
{
    Double* pI = new Double(_iDims, _piDims);
    pI->setZeros();
    int iMinDim = _piDims[0];
    for (int i = 1 ; i < _iDims ; i++)
    {
        if (_piDims[i] < iMinDim)
        {
            iMinDim = _piDims[i];
        }
    }

    int* piIndex = new int[_iDims];
    for (int i = 0; i < iMinDim; i++)
    {
        for (int j = 0 ; j < _iDims ; j++)
        {
            piIndex[j] = i;
        }

        int index = getIndexWithDims(piIndex, _piDims, _iDims);
        pI->set(index, 1);
    }

    delete[] piIndex;
    return pI;
}

Double* Double::Identity(int _iDims, const int* _piDims, double _dblReal)
{
    Double* pI = new Double(_iDims, _piDims);
    pI->setZeros();
    int iMinDim = _piDims[0];
    for (int i = 1; i < _iDims; i++)
    {
        if (_piDims[i] < iMinDim)
        {
            iMinDim = _piDims[i];
        }
    }

    int* piIndex = new int[_iDims];
    for (int i = 0; i < iMinDim; i++)
    {
        for (int j = 0; j < _iDims; j++)
        {
            piIndex[j] = i;
        }

        int index = getIndexWithDims(piIndex, _piDims, _iDims);
        pI->set(index, _dblReal);
    }

    delete[] piIndex;
    return pI;
}

Double* Double::Identity(int _iDims, const int* _piDims, double _dblReal, double _dblImg)
{
    Double* pI = new Double(_iDims, _piDims, true);
    pI->setZeros();
    int iMinDim = _piDims[0];
    for (int i = 1; i < _iDims; i++)
    {
        if (_piDims[i] < iMinDim)
        {
            iMinDim = _piDims[i];
        }
    }

    for (int i = 0; i < iMinDim; i++)
    {
        int* piIndex = new int[_iDims];
        for (int j = 0; j < _iDims; j++)
        {
            piIndex[j] = i;
        }

        int index = getIndexWithDims(piIndex, _piDims, _iDims);
        pI->set(index, _dblReal);
        pI->setImg(index, _dblImg);
        delete[] piIndex;
    }
    return pI;
}

bool Double::isEmpty()
{
    if (getDims() == 2 && getRows() == 0 && getCols() == 0)
    {
        return true;
    }
    return false;
}

Double::~Double()
{
    if (isDeletable() == true)
    {
        deleteAll();
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

Double::Double(int _iRows, int _iCols, bool _bComplex, bool _bZComplex)
{
    int piDims[2]   = {_iRows, _iCols};
    double *pReal   = NULL;
    double *pImg    = NULL;
    setViewAsZComplex(_bZComplex);
    if (_bComplex == false || _bZComplex)
    {
        create(piDims, 2, &pReal, NULL);
    }
    else
    {
        create(piDims, 2, &pReal, &pImg);
    }

    setViewAsInteger(false);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Double::Double(double _dblReal)
{
    m_piDims[0] = 1;
    m_piDims[1] = 1;
    m_iDims = 2;
    m_iRows = 1;
    m_iCols = 1;
    m_iSize = 1;
    m_iSizeMax = m_iSize;
    m_pRealData = new double[1];
    setViewAsInteger(false);
    setViewAsZComplex(false);
    m_pRealData[0] = _dblReal;

    //      int piDims[2] = {1, 1};
    //double *pdblVal;
    //create(piDims, 2, &pdblVal, NULL);
    //pdblVal[0] = _dblReal;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Double::Double(double _dblReal, double _dblImg)
{
    int piDims[2] = {1, 1};
    double *pdblR;
    double *pdblI;
    setViewAsInteger(false);
    setViewAsZComplex(false);
    create(piDims, 2, &pdblR, &pdblI);

    pdblR[0] = _dblReal;
    pdblI[0] = _dblImg;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Double::Double(int _iRows, int _iCols, double **_pdblReal)
{
    int piDims[2] = {_iRows, _iCols};
    setViewAsInteger(false);
    setViewAsZComplex(false);
    create(piDims, 2, _pdblReal, NULL);

#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Double::Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg)
{
    int piDims[2] = {_iRows, _iCols};
    setViewAsInteger(false);
    setViewAsZComplex(false);
    create(piDims, 2, _pdblReal, _pdblImg);

#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Double::Double(int _iDims, const int* _piDims, bool _bComplex, bool _bZComplex)
{
    double *pReal   = NULL;
    double *pImg    = NULL;
    setViewAsZComplex(_bZComplex);
    setViewAsInteger(false);

    if (_bComplex == false || _bZComplex)
    {
        create(_piDims, _iDims, &pReal, NULL);
    }
    else
    {
        create(_piDims, _iDims, &pReal, &pImg);
    }

#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

double*    Double::getReal() const
{
    return get();
}

double Double::getReal(int _iRows, int _iCols)
{
    return get(_iRows, _iCols);
}

bool Double::setInt(int* _piReal)
{
    bool ret = true;
    for (int i = 0 ; i < m_iSize ; i++)
    {
        ret = set(i, static_cast<double>(_piReal[i]));
        if (ret == false)
        {
            return false;
        }
    }
    return true;
}

void Double::whoAmI()
{
    std::cout << "types::Double";
}

bool Double::setZeros()
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

bool Double::setOnes()
{
    if (m_pRealData != NULL)
    {
        std::fill(m_pRealData, m_pRealData + m_iSize, 1);
    }
    else
    {
        return false;
    }

    if (isComplex() == true)
    {
        if (m_pImgData != NULL)
        {
            std::fill(m_pImgData, m_pImgData + m_iSize, 1);
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool Double::subMatrixToString(std::wostringstream& ostr, int* _piDims, int /*_iDims*/)
{
    int iCurrentLine = 0;
    int iLineLen = ConfigVariable::getConsoleWidth();
    int iMaxLines = ConfigVariable::getConsoleLines();

    if (isIdentity())
    {
        ostr << L"eye *" << std::endl << std::endl;
        if (isComplex() == false)
        {
            printDoubleValue(ostr, m_pRealData[0]);
            ostr << std::endl;
        }
        else
        {
            //complex value
            printComplexValue(ostr, m_pRealData[0], m_pImgData[0]);
            ostr << std::endl;
        }
        ostr << std::endl;
    }
    else if (isEmpty())
    {
        printEmptyString(ostr);
        ostr << std::endl;
    }
    else if (isScalar())
    {
        //scalar
        _piDims[0] = 0;
        _piDims[1] = 0;
        int iPos = getIndex(_piDims);

        if (isComplex() == false)
        {
            printDoubleValue(ostr, m_pRealData[iPos]);
            ostr << std::endl;
        }
        else
        {
            //complex value
            printComplexValue(ostr, m_pRealData[iPos], m_pImgData[iPos]);
            ostr << std::endl;
        }
    }
    else if (getCols() == 1)
    {
        //column vector
        if (isComplex() == false)
        {
            for (int i = m_iRows1PrintState ; i < getRows() ; i++)
            {
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    // Number of lines to print exceeds the max lines allowed
                    // Record what line we were at
                    m_iRows1PrintState = i;
                    return false;
                }

                _piDims[1] = 0;
                _piDims[0] = i;
                int iPos = getIndex(_piDims);

                printDoubleValue(ostr, m_pRealData[iPos]);
                ostr << std::endl;
            }
        }
        else
        {
            for (int i = m_iRows1PrintState ; i < getRows() ; i++)
            {
                //complex value
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    m_iRows1PrintState = i;
                    return false;
                }

                _piDims[1] = 0;
                _piDims[0] = i;
                int iPos = getIndex(_piDims);

                printComplexValue(ostr, m_pRealData[iPos], m_pImgData[iPos]);
                ostr << std::endl;
            }
        }
    }
    else if (getRows() == 1)
    {
        //row vector
        std::wostringstream ostemp;
        int iLastVal = m_iCols1PrintState;

        if (isComplex() == false)
        {
            int iLen = 0;
            for (int i = m_iCols1PrintState ; i < getCols() ; i++)
            {
                _piDims[0] = 0;
                _piDims[1] = i;
                int iPos = getIndex(_piDims);

                DoubleFormat df;
                // Get the size of the column to print
                getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), &df);
                iLen = static_cast<int>(ostemp.str().size()) + SIZE_BETWEEN_TWO_VALUES + df.iSignLen + df.iWidth;
                if (iLen >= iLineLen - 1 && iLastVal != i)
                {
                    //Max length, new line
                    iCurrentLine += 4; //"column x to Y" + empty line + value + empty line
                    if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                    {
                        // Number of lines to print exceeds the max lines allowed
                        // Record what column we were at
                        m_iCols1PrintState = iLastVal;
                        return false;
                    }

                    addColumnString(ostr, iLastVal + 1, i);
                    ostr << ostemp.str() << std::endl;
                    ostemp.str(L"");
                    iLastVal = i;
                }

                ostemp << SPACE_BETWEEN_TWO_VALUES;
                addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), &df);
            }

            if (iLastVal != 0)
            {
                addColumnString(ostr, iLastVal + 1, getCols());
            }
            ostemp << std::endl;
            ostr << ostemp.str();
        }
        else //complex case
        {
            int iTotalLen = 0;
            int iLen = 0;

            for (int i = m_iCols1PrintState ; i < getCols() ; i++)
            {
                _piDims[0] = 0;
                _piDims[1] = i;
                int iPos = getIndex(_piDims);

                DoubleFormat dfR, dfI;
                getComplexFormat(ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), &iTotalLen, &dfR, &dfI);
                iLen = static_cast<int>(ostemp.str().size()) + SIZE_BETWEEN_TWO_VALUES + iTotalLen;
                if (iLen >= iLineLen - 1 && iLastVal != i)
                {
                    //Max length, new line
                    iCurrentLine += 4; //"column x to Y" + empty line + value + empty line
                    if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                    {
                        // Number of lines to print exceeds the max lines allowed
                        // Record what column we were at
                        m_iCols1PrintState = iLastVal;
                        return false;
                    }

                    addColumnString(ostr, iLastVal + 1, i);
                    ostr << ostemp.str() << std::endl;
                    ostemp.str(L"");
                    iLastVal = i;
                }

                ostemp << SPACE_BETWEEN_TWO_VALUES;
                addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), iTotalLen, &dfR, &dfI);
            }

            if (iLastVal != 0)
            {
                addColumnString(ostr, iLastVal + 1, getCols());
            }
            ostemp << std::endl;
            ostr << ostemp.str();
        }
    }
    else // matrix
    {
        std::wostringstream ostemp;
        int iLen = 0;
        int iLastCol = m_iCols1PrintState;

        //Array with the max printed size of each col
        int *piSize = new int[getCols()];
        memset(piSize, 0x00, getCols() * sizeof(int));

        if (isComplex() == false)
        {
            //compute the row size for padding for the full matrix
            for (int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
            {
                for (int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
                {
                    _piDims[0] = iRows1;
                    _piDims[1] = iCols1;
                    int iPos = getIndex(_piDims);

                    DoubleFormat df;
                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), &df);
                    piSize[iCols1] = std::max(piSize[iCols1], df.iWidth);
                }
            }

            for (int iCols1 = m_iCols1PrintState ; iCols1 < getCols() ; iCols1++)
            {
                if (iLen + piSize[iCols1] > iLineLen && iCols1 != iLastCol)
                {
                    //find the limit, print this part
                    for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
                    {
                        iCurrentLine++;
                        if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) ||
                                ( (iMaxLines != 0 && iCurrentLine + 3 >= iMaxLines && iRows2 == m_iRows2PrintState) ||
                                  (iMaxLines != 0 && iCurrentLine + 1 >= iMaxLines && iRows2 != m_iRows2PrintState)))
                        {
                            if (m_iRows2PrintState == 0 && iRows2 != 0)
                            {
                                //add header
                                addColumnString(ostr, iLastCol + 1, iCols1);
                            }
                            ostr << ostemp.str();
                            m_iRows2PrintState = iRows2;
                            m_iCols1PrintState = iLastCol;
                            delete[] piSize;
                            return false;
                        }

                        for (int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                        {
                            _piDims[0] = iRows2;
                            _piDims[1] = iCols2;
                            int iPos = getIndex(_piDims);

                            DoubleFormat df;
                            getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), &df);

                            ostemp << SPACE_BETWEEN_TWO_VALUES;

                            df.iWidth = piSize[iCols2];
                            addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), &df);
                        }
                        ostemp << std::endl;
                    }

                    iLen = 0;

                    iCurrentLine++;
                    if (m_iRows2PrintState == 0)
                    {
                        iCurrentLine += 3;
                        addColumnString(ostr, iLastCol + 1, iCols1);
                    }
                    ostr << ostemp.str();
                    ostemp.str(L"");
                    iLastCol = iCols1;
                    m_iRows2PrintState = 0;
                    m_iCols1PrintState = 0;
                }

                iLen += piSize[iCols1] + SIGN_LENGTH + SIZE_BETWEEN_TWO_VALUES;
            }

            for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
            {
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    if (m_iRows2PrintState == 0 && iLastCol != 0)
                    {
                        //add header
                        addColumnString(ostr, iLastCol + 1, getCols());
                    }

                    ostr << ostemp.str();
                    m_iRows2PrintState = iRows2;
                    m_iCols1PrintState = iLastCol;
                    delete[] piSize;
                    return false;
                }

                for (int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                {
                    _piDims[0] = iRows2;
                    _piDims[1] = iCols2;
                    int iPos = getIndex(_piDims);

                    DoubleFormat df;
                    getDoubleFormat(ZeroIsZero(m_pRealData[iPos]), &df);

                    ostemp << SPACE_BETWEEN_TWO_VALUES;
                    df.iWidth = piSize[iCols2];
                    addDoubleValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), &df);
                }
                ostemp << std::endl;
            }

            if (m_iRows2PrintState == 0 && iLastCol != 0)
            {
                addColumnString(ostr, iLastCol + 1, getCols());
            }
            ostr << ostemp.str();
        }
        else //Complex case
        {
            //compute the row size for padding for the full matrix.
            for (int iCols1 = 0; iCols1 < getCols() ; iCols1++)
            {
                for (int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
                {
                    int iTotalWidth = 0;
                    _piDims[0] = iRows1;
                    _piDims[1] = iCols1;
                    int iPos = getIndex(_piDims);

                    DoubleFormat dfR, dfI;
                    getComplexFormat(ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), &iTotalWidth, &dfR, &dfI);

                    iTotalWidth += (dfR.iWidth == 0 ? 0 : SIGN_LENGTH) + (dfI.iWidth == 0 ? 0 : SIGN_LENGTH + 1);
                    if (iTotalWidth > piSize[iCols1])
                    {
                        piSize[iCols1] = iTotalWidth;
                    }

                }
            }

            for (int iCols1 = m_iCols1PrintState ; iCols1 < getCols() ; iCols1++)
            {
                if (iLen + piSize[iCols1] > iLineLen && iCols1 != iLastCol)
                {
                    //find the limit, print this part
                    for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
                    {
                        iCurrentLine++;
                        if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) ||
                                ( (iMaxLines != 0 && iCurrentLine + 3 >= iMaxLines && iRows2 == m_iRows2PrintState) ||
                                  (iMaxLines != 0 && iCurrentLine + 1 >= iMaxLines && iRows2 != m_iRows2PrintState)))
                        {
                            if (m_iRows2PrintState == 0 && iRows2 != 0)
                            {
                                //add header
                                addColumnString(ostr, iLastCol + 1, iCols1);
                            }
                            ostr << ostemp.str();
                            m_iRows2PrintState = iRows2;
                            m_iCols1PrintState = iLastCol;
                            delete[] piSize;
                            return false;
                        }

                        for (int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                        {
                            int iTotalWidth = 0;
                            _piDims[0] = iRows2;
                            _piDims[1] = iCols2;
                            int iPos = getIndex(_piDims);

                            DoubleFormat dfR, dfI;
                            getComplexFormat(ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), &iTotalWidth, &dfR, &dfI);

                            ostemp << SPACE_BETWEEN_TWO_VALUES;
                            addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), piSize[iCols2], &dfR, &dfI);
                        }
                        ostemp << std::endl;
                    }

                    iLen = 0;

                    iCurrentLine++;
                    if (m_iRows2PrintState == 0)
                    {
                        iCurrentLine += 3;
                        addColumnString(ostr, iLastCol + 1, iCols1);
                    }
                    ostr << ostemp.str();
                    ostemp.str(L"");
                    iLastCol = iCols1;
                    m_iRows2PrintState = 0;
                    m_iCols1PrintState = 0;
                }

                iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_VALUES;
            }

            for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
            {
                iCurrentLine++;
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    if (m_iRows2PrintState == 0 && iLastCol != 0)
                    {
                        //add header
                        addColumnString(ostr, iLastCol + 1, getCols());
                    }

                    ostr << ostemp.str();
                    m_iRows2PrintState = iRows2;
                    m_iCols1PrintState = iLastCol;
                    delete[] piSize;
                    return false;
                }

                for (int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
                {
                    int iTotalWidth = 0;
                    _piDims[0] = iRows2;
                    _piDims[1] = iCols2;
                    int iPos = getIndex(_piDims);

                    DoubleFormat dfR, dfI;
                    getComplexFormat(ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), &iTotalWidth, &dfR, &dfI);

                    ostemp << SPACE_BETWEEN_TWO_VALUES;
                    addDoubleComplexValue(&ostemp, ZeroIsZero(m_pRealData[iPos]), ZeroIsZero(m_pImgData[iPos]), piSize[iCols2], &dfR, &dfI);
                }
                ostemp << std::endl;
            }

            if (m_iRows2PrintState == 0 && iLastCol != 0)
            {
                addColumnString(ostr, iLastCol + 1, getCols());
            }
            ostr << ostemp.str();
        }

        delete[] piSize;
    }

    return true;
}

Double* Double::clone()
{
    int iOne = 1;
    Double *pReturn = new Double(m_iDims, m_piDims, isComplex());
    //memcpy(pReturn->getReal(), m_pRealData, m_iSize * sizeof(double));
    dcopy_(&m_iSize, m_pRealData, &iOne, pReturn->getReal(), &iOne);

    if (isComplex())
    {
        pReturn->setComplex(true);
        //memcpy(pReturn->getImg(), m_pImgData, m_iSize * sizeof(double));
        dcopy_(&m_iSize, m_pImgData, &iOne, pReturn->getImg(), &iOne);
    }
    return pReturn;
}

bool Double::fillFromCol(int _iCols, Double *_poSource)
{
    //blas
    int iDestOffset     = _iCols * m_iRows;
    int iSize           = _poSource->getSize();
    double* pdblDest    = m_pRealData + iDestOffset;
    int iOne            = 1;
    dcopy_(&iSize, _poSource->getReal(), &iOne, pdblDest, &iOne);

    if (isComplex())
    {
        pdblDest    = m_pImgData + iDestOffset;
        dcopy_(&iSize, _poSource->getImg(), &iOne, pdblDest, &iOne);
    }
    return true;
}

bool Double::fillFromRow(int _iRows, Double *_poSource)
{
    int iCols = _poSource->getCols();

    if (isComplex())
    {
    }
    else
    {
        for (int i = 0 ; i < iCols ; i++)
        {
            int iDestOffset     = i * m_iRows + _iRows;
            int iOrigOffset     = i * _poSource->getRows();
            int iSize           = _poSource->getRows();
            double* pdblDest    = m_pRealData + iDestOffset;
            double* pdblSource  = _poSource->getReal() + iOrigOffset;
            int iOne            = 1;

            dcopy_(&iSize, pdblSource, &iOne, pdblDest, &iOne);
        }
    }
    return true;
}

bool Double::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isDouble() == false)
    {
        return false;
    }

    Double* pdbl = const_cast<InternalType &>(it).getAs<Double>();

    if (pdbl->getDims() != getDims())
    {
        return false;
    }

    for (int i = 0 ; i < getDims() ; i++)
    {
        if (pdbl->getDimsArray()[i] != getDimsArray()[i])
        {
            return false;
        }
    }

    double *pdblReal = pdbl->getReal();
    for (int i = 0 ; i < getSize() ; i++)
    {
        if (m_pRealData[i] != pdblReal[i])
        {
            return false;
        }
    }

    //both complex
    if (isComplex() && pdbl->isComplex())
    {
        double *pdblImg = pdbl->getImg();
        for (int i = 0 ; i < m_iSize ; i++)
        {
            if (m_pImgData[i] != pdblImg[i])
            {
                return false;
            }
        }
    }
    //pdbl complex check all img values == 0
    else if (pdbl->isComplex())
    {
        double *pdblImg = pdbl->getImg();
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

bool Double::operator!=(const InternalType& it)
{
    return !(*this == it);
}

double Double::getNullValue()
{
    return 0;
}

Double* Double::createEmpty(int _iDims, int* _piDims, bool _bComplex)
{
    return new Double(_iDims, _piDims, _bComplex);
}

double Double::copyValue(double _dblData)
{
    return _dblData;
}

void Double::deleteAll()
{
    delete[] m_pRealData;
    m_pRealData = NULL;
    deleteImg();
}

void Double::deleteImg()
{
    if (isComplex() && m_pImgData)
    {
        delete[] m_pImgData;
        m_pImgData = NULL;
    }
}

double* Double::allocData(int _iSize)
{
    if (isViewAsZComplex())
    {
        return (double*)new doublecomplex[_iSize];
    }
    else
    {
        return new double[_iSize];
    }
}

Double* Double::append(int _iRows, int _iCols, InternalType* _poSource)
{
    Double* pIT = checkRef(this, &Double::append, _iRows, _iCols, _poSource);
    if (pIT != this)
    {
        return pIT;
    }

    Double* pD = _poSource->getAs<Double>();
    int iRows = pD->getRows();
    int iCols = pD->getCols();
    int iSize = iRows * iCols;

    //insert without resize
    if (iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
    {
        return NULL;
    }

    //Update complexity if necessary
    setComplex(isComplex() || pD->isComplex());

    int iInc = 1;
    int iOne = 1;

    //two cases :
    //  - append to 0,0
    //  - real append in x,y

    if (_iRows == 0 && _iCols == 0)
    {
        //append to 0,0
        //std::cout << "append 0,0" << std::endl;
        if (iRows == 1 || iRows == getRows())
        {
            //std::cout << "full Rows or one row" << std::endl;
            if (iRows == 1)
            {
                iInc = getRows();
                //std::cout << "iInc : " << iInc << std::endl;
            }

            if (isComplex())
            {
                C2F(dcopy)(&iSize, pD->get(), &iOne, get(), &iInc);
                if (pD->isComplex())
                {
                    C2F(dcopy)(&iSize, pD->getImg(), &iOne, getImg(), &iInc);
                }
                else
                {
                    memset(getImg(), 0x00, iSize * sizeof(double));
                }
            }
            else
            {
                C2F(dcopy)(&iSize, pD->get(), &iOne, get(), &iInc);
            }
        }
        else
        {
            //std::cout << "part of row" << std::endl;
            if (isComplex())
            {
                for (int i = 0 ; i < iCols ; i++)
                {
                    int iOffset = i * getRows();
                    C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + iOffset, &iOne);
                    if (pD->isComplex())
                    {
                        C2F(dcopy)(&iRows, pD->getImg() + i * iRows, &iOne, getImg() + iOffset, &iOne);
                    }
                    else
                    {
                        memset(getImg() + iOffset, 0x00, iRows * sizeof(double));
                    }
                }
            }
            else
            {
                for (int i = 0 ; i < iCols ; i++)
                {
                    C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + i * getRows(), &iOne);
                }
            }
        }
    }
    else if (_iRows == 0 || (_iCols == 0 && (iCols == 1 || iRows == 1)))
    {
        //real append in x,y
        //std::cout << "real append in x,y" << std::endl;
        if (iRows == 1)
        {
            iInc = getRows();
            //std::cout << "iInc : " << iInc << std::endl;
        }

        int iOffset =  _iCols * getRows() + _iRows;
        C2F(dcopy)(&iSize, pD->get(), &iOne, get() + iOffset, &iInc);

        if (isComplex())
        {
            int iOffset =  _iCols * getRows() + _iRows;
            C2F(dcopy)(&iSize, pD->get(), &iOne, get() + iOffset, &iInc);
            if (pD->isComplex())
            {
                C2F(dcopy)(&iSize, pD->getImg(), &iOne, getImg() + iOffset, &iInc);
            }
            else
            {
                int iZero = 0;
                double dblZero = 0;
                C2F(dcopy)(&iSize, &dblZero, &iZero, getImg() + iOffset, &iInc);
            }
        }
    }
    else
    {
        //std::cout << "no optimisation" << std::endl;
        if (isComplex())
        {
            for (int i = 0 ; i < iCols ; i++)
            {
                int iOffset = (_iCols + i) * getRows() + _iRows;
                C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + iOffset, &iOne);
                if (pD->isComplex())
                {
                    C2F(dcopy)(&iRows, pD->getImg() + i * iRows, &iOne, getImg() + iOffset, &iOne);
                }
                else
                {
                    memset(getImg() + iOffset, 0x00, iRows * sizeof(double));
                }
            }
        }
        else
        {
            for (int i = 0 ; i < iCols ; i++)
            {
                C2F(dcopy)(&iRows, pD->get() + i * iRows, &iOne, get() + (_iCols + i) * getRows() + _iRows, &iOne);
            }
        }
    }
    return this;
}

void Double::convertToInteger()
{
    if (isViewAsInteger())
    {
        //already done
        return;
    }
    //convert in place double to integer
    int* piR = (int*)get();
    double *pdblR = get();

    if (isComplex())
    {
        int* piI = (int*)getImg();
        double *pdblI = getImg();

        //normal way to prevent overlap
        for (int i = 0 ; i < getSize() ; i++)
        {
            piR[i] = (int)pdblR[i];
            piI[i] = (int)pdblI[i];
        }
    }
    else
    {
        //normal way to prevent overlap
        for (int i = 0 ; i < getSize() ; i++)
        {
            piR[i] = (int)pdblR[i];
        }
    }

    setViewAsInteger(true);
}

void Double::convertFromInteger()
{
    if (isViewAsInteger() == false)
    {
        //no need change
        return;
    }

    int* piR = (int*)get();
    double *pdblR = get();
    //convert in place integer to double

    if (isComplex())
    {
        int* piI = (int*)getImg();
        double *pdblI = getImg();

        //reverse way to prevent overlap
        for (int i = getSize() - 1 ; i >= 0 ; i--)
        {
            pdblR[i] = (double)piR[i];
            pdblI[i] = (double)piI[i];
        }
    }
    else
    {
        //reverse way to prevent overlap
        for (int i = getSize() - 1 ; i >= 0 ; i--)
        {
            pdblR[i] = (double)piR[i];
        }
    }

    setViewAsInteger(false);
}

void Double::convertFromZComplex()
{
    if (isViewAsZComplex() == false)
    {
        //no need change
        return;
    }

    doublecomplex* pdblZ = (doublecomplex*)get();
    m_pRealData = new double[getSize()];

    if (m_pImgData)
    {
        delete[] m_pImgData;
    }

    m_pImgData = new double[getSize()];

    vGetPointerFromDoubleComplex(pdblZ, getSize(), m_pRealData, m_pImgData);
    vFreeDoubleComplexFromPointer(pdblZ);
    setViewAsZComplex(false);
}

void Double::convertToZComplex()
{
    if (isViewAsZComplex())
    {
        //already done
        return;
    }

    doublecomplex* pdblZ = NULL;

    if (isComplex())
    {
        pdblZ = oGetDoubleComplexFromPointer(getReal(), getImg() , getSize());
        delete[] m_pImgData;
        m_pImgData = NULL;
    }
    else
    {
        pdblZ = oGetDoubleComplexFromPointer(getReal(), NULL, getSize());
    }

    delete[] m_pRealData;
    m_pRealData = (double*)pdblZ;
    setViewAsZComplex(true);
}

ast::Exp* Double::getExp(const Location& loc)
{
    return new ast::DoubleExp(loc, this);
}

bool Double::isTrue()
{
    if (isEmpty() || isComplex())
    {
        return false;
    }

    return type_traits::isTrue<double>(m_iSize, m_pRealData);
}
}
