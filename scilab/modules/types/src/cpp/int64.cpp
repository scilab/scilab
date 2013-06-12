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
#include "int64.hxx"
#include "tostring_common.hxx"
#include "formatmode.h"

extern "C"
{
#include "core_math.h"
}

namespace types
{
Int64::~Int64()
{
    if (isDeletable() == true)
    {
        deleteAll();
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

Int64::Int64(int _iRows, int _iCols)
{
    int piDims[2]   = {_iRows, _iCols};
    long long* pllVal    = NULL;
    create(piDims, 2, &pllVal, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Int64::Int64(long long _llVal)
{
    int piDims[2]   = {1, 1};
    long long* pllVal    = NULL;
    create(piDims, 2, &pllVal, NULL);
    pllVal[0] = _llVal;
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Int64::Int64(int _iRows, int _iCols, long long** _pllVal)
{
    int piDims[2]   = {_iRows, _iCols};
    create(piDims, 2, _pllVal, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Int64::Int64(int _iDims, int* _piDims)
{
    long long* pllVal    = NULL;
    create(_piDims, _iDims, &pllVal, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

InternalType* Int64::clone()
{
    Int64 *pbClone =  new Int64(getDims(), getDimsArray());
    pbClone->set(get());
    return pbClone;
}

void Int64::whoAmI()
{
    std::cout << "types::Int64";
}

bool Int64::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims)
{
    int iCurrentLine = 0;
    int iLineLen = getConsoleWidth();
    int iMaxLines = getConsoleLines();

    if (isScalar())
    {
        //scalar
        int iWidth  = 0;
        _piDims[0]  = 0;
        _piDims[1]  = 0;
        int iPos    = getIndex(_piDims);

        getSignedIntFormat(get(iPos), &iWidth);
        addSignedIntValue(&ostr, get(iPos), iWidth);
        ostr << std::endl;
    }
    else if (getCols() == 1)
    {
        //column vector
        int iWidthMax = 0;

        //Array with the max printed size of each col
        for (int i = 0 ; i < getRows() ; i++)
        {
            _piDims[1] = 0;
            _piDims[0] = i;
            int iWidth = 0;
            int iPos = getIndex(_piDims);
            getSignedIntFormat(get(iPos), &iWidth);
            iWidthMax = Max(iWidthMax, iWidth);
        }

        for (int i = m_iRows1PrintState ; i < getRows() ; i++)
        {
            iCurrentLine++;
            if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
            {
                m_iRows1PrintState = i;
                return false;
            }

            _piDims[1]  = 0;
            _piDims[0]  = i;
            int iPos    = getIndex(_piDims);

            addSignedIntValue(&ostr, get(iPos), iWidthMax);
            ostr << std::endl;
        }
    }
    else if (getRows() == 1)
    {
        //row vector
        std::wostringstream ostemp;
        int iLastVal = m_iCols1PrintState;

        for (int i = m_iCols1PrintState ; i < getCols() ; i++)
        {
            int iWidth  = 0;
            int iLen    = 0;
            _piDims[0]  = 0;
            _piDims[1]  = i;
            int iPos    = getIndex(_piDims);

            getSignedIntFormat(get(iPos), &iWidth);
            iLen = iWidth + static_cast<int>(ostemp.str().size());
            if (iLen > iLineLen)
            {
                //Max length, new line
                iCurrentLine += 4; //"column x to Y" + empty line + value + empty line
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    m_iCols1PrintState = iLastVal;
                    return false;
                }

                ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << i << std::endl << std::endl;
                ostr << ostemp.str() << std::endl;
                ostemp.str(L"");
                iLastVal = i;
            }

            addSignedIntValue(&ostemp, get(iPos), iWidth);
        }

        if (iLastVal != 0)
        {
            ostr << std::endl << L"       column " << iLastVal + 1 << L" to " << getCols() << std::endl << std::endl;
        }
        ostemp << std::endl;
        ostr << ostemp.str();
    }
    else // matrix
    {
        std::wostringstream ostemp;
        int iLen = 0;
        int iLastCol = m_iCols1PrintState;

        //Array with the max printed size of each col
        int *piSize = new int[getCols()];
        memset(piSize, 0x00, getCols() * sizeof(int));

        //compute the row size for padding for each printed bloc.
        for (int iCols1 = m_iCols1PrintState ; iCols1 < getCols() ; iCols1++)
        {
            for (int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
            {
                int iWidth  = 0;
                _piDims[0]  = iRows1;
                _piDims[1]  = iCols1;
                int iPos    = getIndex(_piDims);

                getSignedIntFormat(get(iPos), &iWidth);
                piSize[iCols1] = Max(piSize[iCols1], iWidth);
            }

            if (iLen + piSize[iCols1] > iLineLen)
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
                            ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << std::endl << std::endl;
                        }
                        ostr << ostemp.str();
                        m_iRows2PrintState = iRows2;
                        m_iCols1PrintState = iLastCol;
                        return false;
                    }

                    for (int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
                    {
                        int iWidth  = 0;
                        _piDims[0]  = iRows2;
                        _piDims[1]  = iCols2;
                        int iPos    = getIndex(_piDims);

                        addSignedIntValue(&ostemp, get(iPos), piSize[iCols2]);
                    }
                    ostemp << std::endl;
                }
                iLen = 0;

                iCurrentLine++;
                if (m_iRows2PrintState == 0)
                {
                    iCurrentLine += 3;
                    ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << std::endl << std::endl;
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
                    ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << getCols() << std::endl << std::endl;
                }

                ostr << ostemp.str();
                m_iRows2PrintState = iRows2;
                m_iCols1PrintState = iLastCol;
                return false;
            }

            for (int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
            {
                int iWidth  = 0;
                _piDims[0]  = iRows2;
                _piDims[1]  = iCols2;
                int iPos    = getIndex(_piDims);

                addSignedIntValue(&ostemp, get(iPos), piSize[iCols2]);
            }
            ostemp << std::endl;
        }

        if (m_iRows2PrintState == 0 && iLastCol != 0)
        {
            ostr << std::endl << L"       column " << iLastCol + 1 << L" to " << getCols() << std::endl << std::endl;
        }
        ostr << ostemp.str();
    }

    return true;
}

bool Int64::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isInt64() == false)
    {
        return false;
    }

    Int64* pb = const_cast<InternalType &>(it).getAs<types::Int64>();

    if (pb->getDims() != getDims())
    {
        return false;
    }

    for (int i = 0 ; i < getDims() ; i++)
    {
        if (pb->getDimsArray()[i] != getDimsArray()[i])
        {
            return false;
        }
    }

    if (memcmp(get(), pb->get(), getSize() * sizeof(int)) != 0)
    {
        return false;
    }
    return true;
}

bool Int64::operator!=(const InternalType& it)
{
    return !(*this == it);
}

long long Int64::getNullValue()
{
    return 0;
}

Int64* Int64::createEmpty(int _iDims, int* _piDims, bool _bComplex)
{
    return new Int64(_iDims, _piDims);
}

long long Int64::copyValue(long long _llVal)
{
    return _llVal;
}

void Int64::deleteAll()
{
    delete[] m_pRealData;
    m_pRealData = NULL;
    deleteImg();
}

void Int64::deleteImg()
{
}

long long* Int64::allocData(int _iSize)
{
    return new long long[_iSize];
}
}

