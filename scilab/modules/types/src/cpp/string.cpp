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
#include "tostring_common.hxx"
#include "formatmode.h"

extern "C"
{
#include "charEncoding.h"
#include "os_wcsdup.h"
}

using namespace std;

#define SIZE_BETWEEN_TWO_VALUES			2
#define SPACE_BETWEEN_TWO_VALUES		L"  "

namespace types
{
	String::~String()
	{
		if(isDeletable() == true)
		{
			deleteAll();
		}
#ifndef NDEBUG
        Inspector::removeItem(this);
#endif
	}

    String::String(int _iDims, int* _piDims)
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
        int piDims[] = {1,1};
		create(piDims, 2, &pwsData, NULL);
		set(0,0, _pwstData);
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
	}

	String::String(const char *_pstData)
	{
        wchar_t** pwsData = NULL;
        int piDims[] = {1,1};
		create(piDims, 2, &pwsData, NULL);
		set(0,0, to_wide_string(const_cast<char*>(_pstData)));
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

    String::String(int _iRows, int _iCols, wchar_t** _pstData)
    {
        wchar_t** pwsData = NULL;
        int piDims[] = {_iRows, _iCols};
		create(piDims, 2, &pwsData, NULL);
        for(int i = 0 ; i < m_iSize ; i++)
        {
            set(i, os_wcsdup(_pstData[i]));
        }
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

	InternalType* String::clone()
	{
	  String *pstClone = new String(getDims(), getDimsArray());
	  pstClone->set(m_pRealData);
	  return pstClone;
	}

	void String::whoAmI()
	{
		cout << "types::String";
	}

	void String::deleteString(int _iPos)
	{
		if(m_pRealData != NULL)
		{
			if(m_pRealData[_iPos] != NULL)
			{
				delete[] m_pRealData[_iPos];
				m_pRealData[_iPos] = NULL;
			}
		}
	}

	void String::deleteAll()
	{
		for(int i = 0 ; i < getSize() ; i++)
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

	GenericType::RealType String::getType()
	{
		return GenericType::RealString;
	}

    void String::subMatrixToString(wostringstream& ostr, int* _piDims, int _iDims)
	{
        int iPrecision = getFormatSize();
        int iLineLen = getConsoleWidth();

		if(isScalar())
		{
            _piDims[0] = 0;
            _piDims[1] = 0;
            int iPos = getIndex(_piDims);
			ostr << L" " << get(iPos) << endl;
		}
		else if(getCols() == 1)
		{
			int iMaxLen = 0;
			for(int i = 0 ; i < getRows() ; i++)
			{
                _piDims[1] = 0;
                _piDims[0] = i;
                int iPos = getIndex(_piDims);
				iMaxLen = Max(iMaxLen, static_cast<int>(wcslen(get(iPos))));
			}

			iMaxLen += 2;

			for(int i = 0 ; i < getRows() ; i++)
			{
                _piDims[1] = 0;
                _piDims[0] = i;
                int iPos = getIndex(_piDims);

                ostr << L"!";
				configureStream(&ostr, iMaxLen, iPrecision, ' ');
				ostr << left << get(iPos);
				ostr << L"!" << endl;
				if((i+1) < m_iSize)
				{
					ostr << L"!";
					configureStream(&ostr, iMaxLen, iPrecision, ' ');
					ostr << left << L" ";
					ostr << L"!" << endl;
				}
			}
		}
		else if(getRows() == 1)
		{
			wostringstream ostemp;
			int iLastVal = 0;
			for(int i = 0 ; i < getCols() ; i++)
			{
                _piDims[0] = 0;
                _piDims[1] = i;
                int iPos = getIndex(_piDims);

                int iLen = 0;
				int iCurLen = static_cast<int>(wcslen(get(iPos)));
				iLen = iCurLen + SIZE_BETWEEN_TWO_VALUES + static_cast<int>(ostemp.str().size());
				if(iLen > iLineLen)
				{//Max length, new line
					if(iLastVal + 1 == i)
					{
						ostr << endl << L"       column " << iLastVal + 1 << endl << endl;
					}
					else
					{
						ostr << endl << L"       column " << iLastVal + 1 << L" to " << i << endl << endl;
					}
					ostr << ostemp.str() << L"!" << endl;
					ostemp.str(L"");
					iLastVal = i;
				}

				configureStream(&ostemp, iCurLen + 2, iPrecision, ' ');
				ostemp << left << get(0,i);
			}

			if(iLastVal != 0)
			{
				if(iLastVal + 1 == m_iCols)
				{
					ostr << endl << L"       column " << iLastVal + 1 << endl << endl;
				}
				else
				{
					ostr << endl << L"       column " << iLastVal + 1 << L" to " << m_iCols << endl << endl;
				}
			}
			ostr << L"!" << ostemp.str() << L"!" << endl;
		}
		else //Matrix
		{
			wostringstream ostemp;
			int iLen = 0;
			int iLastCol = 0;

			//Array with the max printed size of each col
			int *piSize = new int[getCols()];
			memset(piSize, 0x00, getCols() * sizeof(int));

			for(int iCols1 = 0 ; iCols1 < getCols() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < getRows() ; iRows1++)
				{
                    _piDims[1] = iCols1;
                    _piDims[0] = iRows1;
                    int iPos = getIndex(_piDims);
					piSize[iCols1] = Max(piSize[iCols1], static_cast<int>(wcslen(get(iPos))));
				}

				if(iLen + piSize[iCols1] > iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
					{
						ostemp << L"!";
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
                            _piDims[0] = iRows2;
                            _piDims[1] = iCols2;
                            int iPos = getIndex(_piDims);
							configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
							ostemp << left << get(iPos) << SPACE_BETWEEN_TWO_VALUES;
						}

						ostemp << L"!" << endl;
						if((iRows2 + 1) != m_iRows)
						{
							ostemp << L"!";
							configureStream(&ostemp, iLen, iPrecision, ' ');
							ostemp << left << L" ";
							ostemp << L"!" << endl;
						}
					}

					iLen = 0;
					if(iCols1 + 1 == m_iCols)
					{
						ostr << endl << L"       column " << iCols1 << endl << endl;
					}
					else
					{
						ostr << endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;
					}
					ostr << ostemp.str();
					ostemp.str(L"");
					iLastCol = iCols1;
				}
				iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_VALUES;
			}

			for(int iRows2 = 0 ; iRows2 < getRows() ; iRows2++)
			{
				iLen = 0;
				ostemp << L"!";
				for(int iCols2 = iLastCol ; iCols2 < getCols() ; iCols2++)
				{
                    _piDims[0] = iRows2;
                    _piDims[1] = iCols2;
                    int iPos = getIndex(_piDims);

                    configureStream(&ostemp, piSize[iCols2], iPrecision, ' ');
					ostemp << left << get(iPos) << SPACE_BETWEEN_TWO_VALUES;
					iLen += piSize[iCols2] + SIZE_BETWEEN_TWO_VALUES;
				}
				ostemp << L"!" << endl;
				if((iRows2 + 1) != m_iRows)
				{
					ostemp << L"!";
					configureStream(&ostemp, iLen, iPrecision, ' ');
					ostemp << left << L" ";
					ostemp << L"!" << endl;
				}
			}

			if(iLastCol != 0)
			{
				if(iLastCol + 1 == m_iCols)
				{
					ostr << endl << L"       column " << m_iCols << endl << endl;
				}
				else
				{
					ostr << endl << L"       column " << iLastCol + 1 << L" to " << m_iCols << endl << endl;
				}
			}
			ostr << ostemp.str();
		}
	}

	bool String::operator==(const InternalType& it)
	{
		if(const_cast<InternalType&>(it).isString() == false)
		{
			return false;
		}

		String* pS = const_cast<InternalType&>(it).getAs<types::String>();

		if(pS->getRows() != getRows() || pS->getCols() != getCols())
		{
			return false;
		}

		wchar_t **p1 = get();
		wchar_t **p2 = pS->get();

		for(int i = 0 ; i < getSize() ; i++)
		{
			if(wcscmp(p1[i], p2[i]) != 0)
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

    String* String::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new String(_iDims, _piDims);
    }

    wchar_t* String::copyValue(wchar_t* _pwstData)
    {
        return os_wcsdup(_pwstData);
    }

    wchar_t* String::copyValue(const wchar_t* _pwstData)
    {
        return os_wcsdup(_pwstData);
    }

    bool String::set(int _iPos, wchar_t* _pwstData)
    {
        if(m_pRealData == NULL || _iPos >= m_iSize)
        {
            return false;
        }
        m_pRealData[_iPos] = copyValue(_pwstData);
        return true;
    }

    bool String::set(int _iPos, const wchar_t* _pwstData)
    {
        if(m_pRealData == NULL || _iPos >= m_iSize)
        {
            return false;
        }
        m_pRealData[_iPos] = copyValue(_pwstData);
        return true;
    }

    bool String::set(int _iRows, int _iCols, const wchar_t* _pwstData)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return set(getIndex(piIndexes), _pwstData);
    }

    bool String::set(int _iRows, int _iCols, wchar_t* _pwstData)
    {
        int piIndexes[2] = {_iRows, _iCols};
        return set(getIndex(piIndexes), _pwstData);
    }

    bool String::set(wchar_t** _pwstData)
    {
        if(m_pRealData == NULL)
        {
            return false;
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(set(i, _pwstData[i]) == false)
            {
                return false;
            }
        }
        return true;
    }

    wchar_t** String::allocData(int _iSize)
    {
        wchar_t** pStr = new wchar_t*[_iSize];
        memset(pStr, 0x00, _iSize * sizeof(wchar_t*));
        return pStr;
    }
}

