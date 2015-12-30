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
#define SPACE_BETWEEN_TWO_STRING_VALUES "  "

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
    char** data = NULL;
    create(_piDims, _iDims, &data, NULL);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(const char *_pstData)
{
    char** data = NULL;
    int piDims[] = {1, 1};
    create(piDims, 2, &data, NULL);
    set(0, _pstData);
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

String::String(int _iRows, int _iCols)
{
    char** data = NULL;
    int piDims[] = {_iRows, _iCols};
    create(piDims, 2, &data, NULL);
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

bool String::subMatrixToString(std::ostringstream& ostr, int* _piDims, int /*_iDims*/)
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
        char* cStr = get(iPos);
        int iCurLen = static_cast<int>(strlen(cStr));
        iMaxLen = std::max(iMaxLen, iCurLen);
        iMaxLen = std::min(iMaxLen, iStrMaxSize);

        if (iCurLen > iMaxLen)
        {
            int iStrPos = 0;
            while (iCurLen > iStrMaxSize)
            {
                ostr << " ";
                ostr.write(cStr + iStrPos, iStrMaxSize);
                ostr << " " << std::endl;
                iCurLen -= iStrMaxSize;
                iStrPos += iStrMaxSize;
            }

            ostr << " ";
            configureStream(&ostr, iStrMaxSize, iPrecision, ' ');
            ostr << std::left << cStr + iStrPos;
        }
        else
        {
            ostr << " " << cStr << std::endl;
        }
    }
    else if (getCols() == 1)
    {
        std::string spaces("");

        // compte max string size
        int iMaxLen = 0;
        for (int i = 0 ; i < getRows() ; i++)
        {
            _piDims[1] = 0;
            _piDims[0] = i;
            int iPos = getIndex(_piDims);
            iMaxLen = std::max(iMaxLen, static_cast<int>(strlen(get(iPos))));
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
            char* cStr = get(iPos);
            int iCurLen = static_cast<int>(strlen(cStr));

            ostr << "!";
            if (iCurLen > iMaxLen)
            {
                int iStrPos = 0;
                while (iCurLen > iStrMaxSize)
                {
                    ostr.write(cStr + iStrPos, iStrMaxSize);
                    ostr << "!" << std::endl << "!";
                    iCurLen -= iStrMaxSize;
                    iStrPos += iStrMaxSize;
                }

                configureStream(&ostr, iStrMaxSize, iPrecision, ' ');
                ostr << std::left << cStr + iStrPos;
            }
            else
            {
                configureStream(&ostr, iMaxLen, iPrecision, ' ');
                ostr << std::left << cStr << spaces;
            }

            ostr << "!" << std::endl;

            if ((i + 1) < m_iSize)
            {
                //for all but last one
                ostr << "!";
                configureStream(&ostr, iEmptyLineSize, iPrecision, ' ');
                ostr << std::left << " ";
                ostr << "!" << std::endl;
            }
        }
    }
    else if (getRows() == 1)
    {
        std::ostringstream ostemp;
        int iLastVal = m_iCols1PrintState;

        for (int i = m_iCols1PrintState ; i < getCols() ; i++)
        {
            _piDims[0] = 0;
            _piDims[1] = i;
            int iPos = getIndex(_piDims);

            int iLen = 0;
            int iCurLen = static_cast<int>(strlen(get(iPos)));
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
                ostr << "!" << ostemp.str() << "!" << std::endl;
                ostemp.str("");
                iLastVal = i;
            }

            // Manage case where string length is greater than max line size.
            if (iStrMaxSize < iCurLen)
            {
                char* cStr = get(iPos);
                int iStrPos = 0;
                while (iCurLen > iStrMaxSize) // -2 because of two "!"
                {
                    ostemp.write(cStr + iStrPos, iStrMaxSize);
                    ostemp << "!" << std::endl << "!";
                    iCurLen -= iStrMaxSize;
                    iStrPos += iStrMaxSize;
                }

                configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                ostemp << std::left << cStr + iStrPos;
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

        ostr << "!" << ostemp.str() << "!" << std::endl;
    }
    else //Matrix
    {
        std::ostringstream ostemp;
        int iLen = 0;
        int iLastCol = m_iCols1PrintState;

        //Array with the max printed size of each col
        int *piSize = new int[getCols()];
        memset(piSize, 0x00, getCols() * sizeof(int));

        for (int iCols1 = m_iCols1PrintState ; iCols1 < getCols() ; iCols1++)
        {
            std::string spaces("");
            for (int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
            {
                _piDims[1] = iCols1;
                _piDims[0] = iRows1;
                int iPos = getIndex(_piDims);
                piSize[iCols1] = std::max(piSize[iCols1], static_cast<int>(strlen(get(iPos))));
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
                        return false;
                    }

                    ostemp << "!";
                    for (int iCols2 = iLastCol; iCols2 < iCols1; iCols2++)
                    {
                        _piDims[0] = iRows2;
                        _piDims[1] = iCols2;
                        int iPos = getIndex(_piDims);
                        char* cStr = get(iPos);
                        int iLenStr = static_cast<int>(strlen(cStr));

                        // Manage case where string length is greater than max line size.
                        if (iLenStr > iStrMaxSize)
                        {
                            int iStrPos = 0;
                            while (iLenStr > iStrMaxSize)
                            {
                                ostemp.write(cStr + iStrPos, iStrMaxSize);
                                ostemp << "!" << std::endl << "!";
                                iLenStr -= iStrMaxSize;
                                iStrPos += iStrMaxSize;
                            }

                            configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                            ostemp << std::left << cStr + iStrPos;
                        }
                        else
                        {
                            configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
                            ostemp << std::left << get(iPos) << spaces;
                        }
                    }
                    ostemp << "!" << std::endl;

                    if ((iRows2 + 1) != m_iRows)
                    {
                        ostemp << "!";
                        // -2 because of two "!"
                        configureStream(&ostemp, iEmptyLineSize, iPrecision, ' ');
                        ostemp << std::left << " ";
                        ostemp << "!" << std::endl;
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
                ostemp.str("");
                iLastCol = iCols1;
                m_iRows2PrintState = 0;
                m_iCols1PrintState = 0;
            }
            iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_STRING_VALUES;
        }

        for (int iRows2 = m_iRows2PrintState ; iRows2 < getRows() ; iRows2++)
        {
            std::string spaces("");
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


            ostemp << "!";
            iLen = 0;
            for (int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
            {
                _piDims[0] = iRows2;
                _piDims[1] = iCols2;
                int iPos = getIndex(_piDims);
                char* cStr = get(iPos);
                int iLenStr = static_cast<int>(strlen(cStr));

                // Manage case where string length is greater than max line size.
                if (iStrMaxSize < iLenStr)
                {
                    int iStrPos = 0;
                    while (iLenStr > iStrMaxSize)
                    {
                        ostemp.write(cStr + iStrPos, iStrMaxSize);
                        ostemp << "!" << std::endl << "!";
                        iLenStr -= iStrMaxSize;
                        iStrPos += iStrMaxSize;
                    }

                    configureStream(&ostemp, iStrMaxSize, iPrecision, ' ');
                    ostemp << cStr + iStrPos << std::left;
                    iLen = iStrMaxSize;
                }
                else
                {
                    configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
                    ostemp << std::left << get(iPos) << spaces;
                    iLen += piSize[iCols2] + static_cast<int>(spaces.size());
                }
            }
            ostemp << "!" << std::endl;

            if ((iRows2 + 1) != m_iRows)
            {
                ostemp << "!";
                configureStream(&ostemp, iLen, iPrecision, ' ');
                ostemp << std::left << " ";
                ostemp << "!" << std::endl;
            }
        }

        if (m_iRows2PrintState == 0 && iLastCol != 0)
        {
            addColumnString(ostr, iLastCol + 1, getCols());
        }
        ostr << ostemp.str();
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

    char **p1 = get();
    char **p2 = pS->get();

    for (int i = 0 ; i < getSize() ; i++)
    {
        if (strcmp(p1[i], p2[i]) != 0)
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

char* String::getNullValue()
{
    return os_strdup("");
}

String* String::createEmpty(int _iDims, int* _piDims, bool /*_bComplex*/)
{
    return new String(_iDims, _piDims);
}

char* String::copyValue(char* _pstData)
{
    try
    {
        return os_strdup(_pstData);
    }
    catch (std::bad_alloc & /*e*/)
    {
        char message[bsiz];
        os_sprintf(message, _("Can not allocate data.\n"));
        throw ast::InternalError(message);
    }

    return NULL;
}

char* String::copyValue(const char* _pstData)
{
    return os_strdup(_pstData);
}

void String::deleteData(char* data)
{
    if (data)
    {
        // data are always allocated using C-like malloc API
        FREE(data);
    }
}

String* String::set(int _iPos, const char* _pstData)
{
    if (m_pRealData == NULL || _iPos >= m_iSize)
    {
        return NULL;
    }

    typedef String* (String::*set_t)(int, const char*);
    String* pIT = checkRef(this, (set_t)&String::set, _iPos, _pstData);
    if (pIT != this)
    {
        return pIT;
    }

    deleteString(_iPos);
    m_pRealData[_iPos] = copyValue(_pstData);
    return this;
}

String* String::set(int _iRows, int _iCols, const char* _pstData)
{
    int piIndexes[2] = {_iRows, _iCols};
    return set(getIndex(piIndexes), _pstData);
}

String* String::set(const char* const* _pstData)
{
    typedef String* (String::*set_t)(const char * const*);
    String* pIT = checkRef(this, (set_t)&String::set, _pstData);
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
        m_pRealData[i] = copyValue(_pstData[i]);
    }
    return this;
}

char** String::allocData(int _iSize)
{
    char** pStr = nullptr;
    try
    {
        pStr = new char*[_iSize];
        memset(pStr, 0x00, _iSize * sizeof(char*));
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

