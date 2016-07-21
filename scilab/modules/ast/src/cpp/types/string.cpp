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
#include <iomanip>
#include "core_math.h"
#include "string.hxx"
#include "stringexp.hxx"
#include "tostring_common.hxx"
#include "configvariable.hxx"
#include "type_traits.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_string.h"
#include "sci_malloc.h"
}

#define SIZE_BETWEEN_TWO_STRING_VALUES  2
#define SPACE_BETWEEN_TWO_STRING_VALUES L"  "

namespace types
{
String::~String()
{
    if (isDeletable() == true)
    {
        deleteAll();
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

String::String(int _iDims, const int* _piDims)
{
    wchar_t** pwsData = NULL;
    create(_piDims, _iDims, &pwsData, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(const wchar_t* _pwstData)
{
    wchar_t** pwsData = NULL;
    int piDims[] = {1, 1};
    create(piDims, 2, &pwsData, NULL);
    set(0, 0, _pwstData);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(const char *_pstData)
{
    wchar_t** pwsData = NULL;
    int piDims[] = {1, 1};
    create(piDims, 2, &pwsData, NULL);
    wchar_t* data = to_wide_string(const_cast<char*>(_pstData));
    set(0, 0, data);
    FREE(data);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(int _iRows, int _iCols)
{
    wchar_t** pwsData = NULL;
    int piDims[] = {_iRows, _iCols};
    create(piDims, 2, &pwsData, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(int _iRows, int _iCols, wchar_t const* const* _pstData)
{
    wchar_t** pwsData = NULL;
    int piDims[] = {_iRows, _iCols};
    create(piDims, 2, &pwsData, NULL);
    for (int i = 0 ; i < m_iSize ; i++)
    {
        set(i, _pstData[i]);
    }
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String* String::clone()
{
    String *pstClone = new String(getDims(), getDimsArray());
    pstClone->set(m_pRealData);
    return pstClone;
}

void String::whoAmI()
{
    std::cout << "types::String";
}

void String::deleteString(int _iPos)
{
    if (m_pRealData != NULL)
    {
        if (m_pRealData[_iPos] != NULL)
        {
            FREE(m_pRealData[_iPos]);
            m_pRealData[_iPos] = NULL;
        }
    }
}

void String::deleteAll()
{
    for (int i = 0 ; i < m_iSizeMax ; i++)
    {
        deleteString(i);
    }
    delete[] m_pRealData;
    m_pRealData = NULL;
}

void String::deleteImg()
{
    return;
}

bool String::subMatrixToString(std::wostringstream& ostr, int* _piDims, int /*_iDims*/)
{
    int iPrecision = ConfigVariable::getFormatSize();
    int iLineLen = ConfigVariable::getConsoleWidth();
    int iMaxLines = ConfigVariable::getConsoleLines();
    int iCurrentLine = 0;

    // less the two "!" (or two " " in scalar case)
    // for iLineLen = 50 we will write "!48char!"
    int iStrMaxSize = iLineLen - 2;

    if (isScalar())
    {
        int iMaxLen = 0;
        _piDims[0]  = 0;
        _piDims[1]  = 0;

        int iPos = getIndex(_piDims);
        wchar_t* wcsStr = get(iPos);
        int iCurLen = static_cast<int>(wcslen(wcsStr));
        iMaxLen = std::max(iMaxLen, iCurLen);
        iMaxLen = std::min(iMaxLen, iStrMaxSize);

        if (iCurLen > iMaxLen)
        {
            int iStrPos = 0;
            while (iCurLen > iStrMaxSize)
            {
                ostr << L" ";
                ostr.write(wcsStr + iStrPos, iStrMaxSize);
                ostr << L" " << std::endl;
                iCurLen -= iStrMaxSize;
                iStrPos += iStrMaxSize;
            }

            ostr << L" ";
            configureStream(&ostr, iStrMaxSize, iPrecision, ' ');
            ostr << std::left << wcsStr + iStrPos << std::resetiosflags(std::ios::adjustfield);
        }
        else
        {
            ostr << L" " << wcsStr << std::endl;
        }
    }
    else if (getCols() == 1)
    {
        std::wstring spaces(L"");

        // compte max string size
        int iMaxLen = 0;
        for (int i = 0 ; i < getRows() ; i++)
        {
            _piDims[1] = 0;
            _piDims[0] = i;
            int iPos = getIndex(_piDims);
            iMaxLen = std::max(iMaxLen, static_cast<int>(wcslen(get(iPos))));
            iMaxLen = std::min(iMaxLen, iStrMaxSize);
        }

        int iEmptyLineSize = iMaxLen;
        if (iMaxLen != iStrMaxSize)
        {
            // count SPACE_BETWEEN_TWO_STRING_VALUES size in padding of empty line
            // only if all lines have not a size greater than max size of a line.
            iEmptyLineSize += SIZE_BETWEEN_TWO_STRING_VALUES;
            spaces = SPACE_BETWEEN_TWO_STRING_VALUES;
        }

        for (int i = m_iRows1PrintState ; i < getRows() ; i++)
        {
            iCurrentLine += 2;
            if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
            {
                m_iRows1PrintState = i;
                return false;
            }

            _piDims[1] = 0;
            _piDims[0] = i;
            int iPos = getIndex(_piDims);
            wchar_t* wcsStr = get(iPos);
            int iCurLen = static_cast<int>(wcslen(wcsStr));

            ostr << L"!";
            if (iCurLen > iMaxLen)
            {
                int iStrPos = 0;
                while (iCurLen > iStrMaxSize)
                {
                    ostr.write(wcsStr + iStrPos, iStrMaxSize);
                    ostr << L"!" << std::endl << L"!";
                    iCurLen -= iStrMaxSize;
                    iStrPos += iStrMaxSize;
                }

                configureStream(&ostr, iStrMaxSize, iPrecision, ' ');
                ostr << std::left << wcsStr + iStrPos;
            }
            else
            {
                configureStream(&ostr, iMaxLen, iPrecision, ' ');
                ostr << std::left << wcsStr << spaces;
            }

            ostr << L"!" << std::endl;

            if ((i + 1) < m_iSize)
            {
                //for all but last one
                ostr << L"!";
                configureStream(&ostr, iEmptyLineSize, iPrecision, ' ');
                ostr << std::left << L" ";
                ostr << L"!" << std::endl;
            }
        }
        ostr << std::resetiosflags(std::ios::adjustfield);
    }
    else if (getRows() == 1)
    {
        std::wostringstream ostemp;
        int iLastVal = m_iCols1PrintState;

        for (int i = m_iCols1PrintState ; i < getCols() ; i++)
        {
            _piDims[0] = 0;
            _piDims[1] = i;
            int iPos = getIndex(_piDims);

            int iLen = 0;
            int iCurLen = static_cast<int>(wcslen(get(iPos)));
            iLen = iCurLen + SIZE_BETWEEN_TWO_STRING_VALUES + static_cast<int>(ostemp.str().size());
            if (iLen > iLineLen && iLastVal != i)
            {
                //Max length, new line
                iCurrentLine += 4; //"column x to Y" + empty line + value + empty line
                if ((iMaxLines == 0 && iCurrentLine >= MAX_LINES) || (iMaxLines != 0 && iCurrentLine >= iMaxLines))
                {
                    m_iCols1PrintState = iLastVal;
                    return false;
                }

                addColumnString(ostr, iLastVal + 1, i);
                ostr << L"!" << ostemp.str() << L"!" << std::endl;
                ostemp.str(L"");
                iLastVal = i;
            }

            // Manage case where string length is greater than max line size.
            if (iStrMaxSize < iCurLen)
            {
                wchar_t* wcsStr = get(iPos);
                int iStrPos = 0;
                while (iCurLen > iStrMaxSize) // -2 because of two "!"
                {
                    ostemp.write(wcsStr + iStrPos, iStrMaxSize);
                    ostemp << L"!" << std::endl << L"!";
                    iCurLen -= iStrMaxSize;
                    iStrPos += iStrMaxSize;
                }

                configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                ostemp << std::left << wcsStr + iStrPos;
            }
            else
            {
                configureStream(&ostemp, iCurLen + 2, iPrecision, ' ');
                ostemp << std::left << get(iPos);
            }
        }

        if (iLastVal != 0)
        {
            addColumnString(ostr, iLastVal + 1, getCols());
        }

        ostr << L"!" << ostemp.str() << L"!" << std::endl << std::resetiosflags(std::ios::adjustfield);
    }
    else //Matrix
    {
        std::wostringstream ostemp;
        int iLen = 0;
        int iLastCol = m_iCols1PrintState;

        //Array with the max printed size of each col
        int *piSize = new int[getCols()];
        memset(piSize, 0x00, getCols() * sizeof(int));

        for (int iCols1 = m_iCols1PrintState ; iCols1 < getCols() ; iCols1++)
        {
            std::wstring spaces(L"");
            for (int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
            {
                _piDims[1] = iCols1;
                _piDims[0] = iRows1;
                int iPos = getIndex(_piDims);
                piSize[iCols1] = std::max(piSize[iCols1], static_cast<int>(wcslen(get(iPos))));
                piSize[iCols1] = std::min(piSize[iCols1], iStrMaxSize);
            }

            int iEmptyLineSize = piSize[iLastCol];
            if (piSize[iLastCol] != iStrMaxSize)
            {
                // count SPACE_BETWEEN_TWO_STRING_VALUES size in padding of empty line
                // only if all lines have not a size greater than max size of a line.
                iEmptyLineSize += SIZE_BETWEEN_TWO_STRING_VALUES;
                spaces = SPACE_BETWEEN_TWO_STRING_VALUES;
            }

            if (iLen + piSize[iCols1] > iLineLen && iLastCol != iCols1)
            {
                //find the limit, print this part
                for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
                {
                    iCurrentLine += 2;
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

                    ostemp << L"!";
                    for (int iCols2 = iLastCol; iCols2 < iCols1; iCols2++)
                    {
                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;
                        int iPos = getIndex(_piDims);
                        wchar_t* wcsStr = get(iPos);
                        int iLenStr = static_cast<int>(wcslen(wcsStr));

                        // Manage case where string length is greater than max line size.
                        if (iLenStr > iStrMaxSize)
                        {
                            int iStrPos = 0;
                            while (iLenStr > iStrMaxSize)
                            {
                                ostemp.write(wcsStr + iStrPos, iStrMaxSize);
                                ostemp << L"!" << std::endl << L"!";
                                iLenStr -= iStrMaxSize;
                                iStrPos += iStrMaxSize;
                            }

                            configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                            ostemp << std::left << wcsStr + iStrPos;
                        }
                        else
                        {
                            configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
                            ostemp << std::left << get(iPos) << spaces;
                        }
                    }
                    ostemp << L"!" << std::endl;

                    if ((iRows2 + 1) != m_iRows)
                    {
                        ostemp << L"!";
                        // -2 because of two "!"
                        configureStream(&ostemp, iEmptyLineSize, iPrecision, ' ');
                        ostemp << std::left << L" ";
                        ostemp << L"!" << std::endl;
                    }
                }

                iLen = 0;
                iCurrentLine += 2;
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
            iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_STRING_VALUES;
        }

        for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
        {
            std::wstring spaces(L"");
            iCurrentLine += 2;
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

            int iEmptyLineSize = piSize[iLastCol];
            if (piSize[iLastCol] != iStrMaxSize)
            {
                // count SPACE_BETWEEN_TWO_STRING_VALUES size in padding of empty line
                // only if all lines have not a size greater than max size of a line.
                iEmptyLineSize += SIZE_BETWEEN_TWO_STRING_VALUES;
                spaces = SPACE_BETWEEN_TWO_STRING_VALUES;
            }


            ostemp << L"!";
            iLen = 0;
            for (int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
            {
                _piDims[0] = iRows2;
                _piDims[1] = iCols2;
                int iPos = getIndex(_piDims);
                wchar_t* wcsStr = get(iPos);
                int iLenStr = static_cast<int>(wcslen(wcsStr));

                // Manage case where string length is greater than max line size.
                if (iStrMaxSize < iLenStr)
                {
                    int iStrPos = 0;
                    while (iLenStr > iStrMaxSize)
                    {
                        ostemp.write(wcsStr + iStrPos, iStrMaxSize);
                        ostemp << L"!" << std::endl << L"!";
                        iLenStr -= iStrMaxSize;
                        iStrPos += iStrMaxSize;
                    }

                    configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                    ostemp << wcsStr + iStrPos << std::left;
                    iLen = iStrMaxSize;
                }
                else
                {
                    configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
                    ostemp << std::left << get(iPos) << spaces;
                    iLen += piSize[iCols2] + static_cast<int>(spaces.size());
                }
            }
            ostemp << L"!" << std::endl;

            if ((iRows2 + 1) != m_iRows)
            {
                ostemp << L"!";
                configureStream(&ostemp, iLen, iPrecision, ' ');
                ostemp << std::left << L" ";
                ostemp << L"!" << std::endl;
            }
        }

        if (m_iRows2PrintState == 0 && iLastCol != 0)
        {
            addColumnString(ostr, iLastCol + 1, getCols());
        }
        ostr << ostemp.str() << std::resetiosflags(std::ios::adjustfield);
        delete[] piSize;
    }

    return true;
}

bool String::operator==(const InternalType& it)
{
    if (const_cast<InternalType&>(it).isString() == false)
    {
        return false;
    }

    String* pS = const_cast<InternalType&>(it).getAs<types::String>();

    if (pS->getRows() != getRows() || pS->getCols() != getCols())
    {
        return false;
    }

    wchar_t **p1 = get();
    wchar_t **p2 = pS->get();

    for (int i = 0 ; i < getSize() ; i++)
    {
        if (wcscmp(p1[i], p2[i]) != 0)
        {
            return false;
        }
    }
    return true;
}

bool String::operator!=(const InternalType& it)
{
    return !(*this == it);
}

wchar_t* String::getNullValue()
{
    return os_wcsdup(L"");
}

String* String::createEmpty(int _iDims, int* _piDims, bool /*_bComplex*/)
{
    return new String(_iDims, _piDims);
}

wchar_t* String::copyValue(wchar_t* _pwstData)
{
    try
    {
        return os_wcsdup(_pwstData);
    }
    catch (std::bad_alloc & /*e*/)
    {
        char message[bsiz];
        os_sprintf(message, _("Can not allocate data.\n"));
        throw ast::InternalError(message);
    }

    return NULL;
}

wchar_t* String::copyValue(const wchar_t* _pwstData)
{
    return os_wcsdup(_pwstData);
}

void String::deleteData(wchar_t* data)
{
    if (data)
    {
        // data are always allocated using C-like malloc API
        FREE(data);
    }
}

String* String::set(int _iPos, const wchar_t* _pwstData)
{
    if (m_pRealData == NULL || _iPos >= m_iSize)
    {
        return NULL;
    }

    typedef String* (String::*set_t)(int, const wchar_t*);
    String* pIT = checkRef(this, (set_t)&String::set, _iPos, _pwstData);
    if (pIT != this)
    {
        return pIT;
    }

    deleteString(_iPos);
    m_pRealData[_iPos] = copyValue(_pwstData);
    return this;
}

String* String::set(int _iRows, int _iCols, const wchar_t* _pwstData)
{
    int piIndexes[2] = {_iRows, _iCols};
    return set(getIndex(piIndexes), _pwstData);
}

String* String::set(const wchar_t* const* _pwstData)
{
    typedef String* (String::*set_t)(const wchar_t * const*);
    String* pIT = checkRef(this, (set_t)&String::set, _pwstData);
    if (pIT != this)
    {
        return pIT;
    }

    for (int i = 0; i < m_iSize; i++)
    {
        if (m_pRealData == NULL || i >= m_iSize)
        {
            return NULL;
        }

        deleteString(i);
        m_pRealData[i] = copyValue(_pwstData[i]);
    }
    return this;
}

String* String::set(int _iPos, const char* _pcData)
{
    wchar_t* w = to_wide_string(_pcData);
    String* ret = set(_iPos, w);
    FREE(w);
    return ret;
}

String* String::set(int _iRows, int _iCols, const char* _pcData)
{
    int piIndexes[2] = {_iRows, _iCols};
    return set(getIndex(piIndexes), _pcData);
}

String* String::set(const char* const* _pstrData)
{
    typedef String* (String::*set_t)(const char * const*);
    String* pIT = checkRef(this, (set_t)&String::set, _pstrData);
    if (pIT != this)
    {
        return pIT;
    }

    for (int i = 0; i < m_iSize; i++)
    {
        if (set(i, _pstrData[i]) == NULL)
        {
            return NULL;
        }
    }
    return this;
}

wchar_t** String::allocData(int _iSize)
{
    wchar_t** pStr = nullptr;
    try
    {
        pStr = new wchar_t*[_iSize];
        memset(pStr, 0x00, _iSize * sizeof(wchar_t*));
    }
    catch (std::bad_alloc & /*e*/)
    {
        char message[bsiz];
        os_sprintf(message, _("Can not allocate %.2f MB memory.\n"), (double)(_iSize * sizeof(char*)) / 1.e6);
        throw ast::InternalError(message);
    }
    return pStr;
}

ast::Exp* String::getExp(const Location& loc)
{
    return new ast::StringExp(loc, this);
}

bool String::transpose(InternalType *& out)
{
    return type_traits::transpose(*this, out);
}

}

