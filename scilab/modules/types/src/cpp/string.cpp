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
			all_delete();
		}
	}

	String::String(const wchar_t *_pstData)
	{
		CreateString(1,1);
		string_set(0,0, _pstData);
	}

	String::String(const char *_pstData)
	{
		CreateString(1,1);
		string_set(0,0, to_wide_string(const_cast<char*>(_pstData)));
	}

	String::String(size_t _iRows, size_t _iCols)
	{
		CreateString(_iRows, _iCols);
	}

    String::String(size_t _iRows, size_t _iCols, wchar_t** _pstData)
    {

    }

	String *String::clone()
	{
	  String *pstClone = new String(rows_get(), cols_get());

	  pstClone->string_set(m_pstData);

	  return pstClone;
	}

	void String::CreateString(size_t _iRows, size_t _iCols)
	{
		m_iRows		= _iRows;
		m_iCols		= _iCols;
		m_iSize		= m_iRows * m_iCols;

		m_pstData	= new wchar_t*[m_iSize];
		for(size_t iIndex = 0 ; iIndex < m_iSize ; iIndex++)
		{
			m_pstData[iIndex] = os_wcsdup(L"");
		}
	}

	void String::whoAmI()
	{
		cout << "types::String";
	}

	wchar_t** String::string_get() const
	{
		return m_pstData;
	}

	wchar_t* String::string_get(size_t _iRows, size_t _iCols) const
	{
		if(_iRows >= m_iRows || _iCols >= m_iCols)
		{
			return NULL;
		}
		return string_get(_iCols * m_iRows + _iRows);
	}

	wchar_t* String::string_get(size_t _iPos) const
	{
		if(m_pstData != NULL && _iPos < m_iSize)
		{
			return m_pstData[_iPos];
		}
		else
		{
			return NULL;
		}
	}

	bool String::string_set(size_t _iRows, size_t _iCols, const wchar_t *_pstData)
	{
		return string_set(_iCols * m_iRows + _iRows, _pstData);
	}

	bool String::string_set(wchar_t **_pstData)
	{
		if(_pstData != NULL)
		{
			for(size_t iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				if(_pstData[iIndex] != 0)
				{
					bool bRet = string_set(iIndex, _pstData[iIndex]);
					if(bRet == false)
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	bool String::string_set(size_t _iPos, const wchar_t *_pstData)
	{
		if(m_pstData == NULL)
		{
			return false;
		}

		if(m_pstData[_iPos] != NULL)
		{
			delete[] m_pstData[_iPos];
		}

		m_pstData[_iPos] = new wchar_t[wcslen(_pstData) + 1];
		//memcpy(m_pstData[_iPos], _pcData, strlen(_pcData) * sizeof(wchar_t));
		//m_pstData[_iPos][strlen(_pcData)] = '\0';

#ifdef _MSC_VER
        wcscpy_s(m_pstData[_iPos], wcslen(_pstData) + 1, _pstData);
#else
        wcscpy(m_pstData[_iPos], _pstData);
#endif

		return true;
	}


	void String::string_delete(size_t _iPos)
	{
		if(m_pstData != NULL)
		{
			if(m_pstData[_iPos] != NULL)
			{
				delete[] m_pstData[_iPos];
				m_pstData[_iPos]	= NULL;
			}
		}
	}

	void String::all_delete()
	{
		for(size_t iIndex = 0 ; iIndex < m_iSize ; iIndex++)
		{
			string_delete(iIndex);
		}
		delete[] m_pstData;
		m_pstData = NULL;
	}


	String* String::getAsString(void)
	{
		return this;
	}

	GenericType::RealType String::getType()
	{
		return GenericType::RealString;
	}

	wstring String::toString(int _iPrecision, int _iLineLen)
	{
		wostringstream ostr;

		if(m_iRows == 1 && m_iCols == 1)
		{
			ostr << string_get(0,0) << endl;
		}
		else if(m_iCols == 1)
		{
			size_t iMaxLen = 0;
			for(size_t i = 0 ; i < m_iSize ; i++)
			{
				iMaxLen = Max(iMaxLen, wcslen(string_get(i,0)));
			}

			iMaxLen += 2;

			for(size_t i = 0 ; i < m_iSize ; i++)
			{
				ostr << L"!";
				Config_Stream(&ostr, iMaxLen, _iPrecision, ' ');
				ostr << left << string_get(i,0);
				ostr << L"!" << endl;
				if((i+1) < m_iSize)
				{
					ostr << L"!";
					Config_Stream(&ostr, iMaxLen, _iPrecision, ' ');
					ostr << left << L" ";
					ostr << L"!" << endl;
				}
			}
		}
		else if(m_iRows == 1)
		{
			wostringstream ostemp;
			size_t iLastVal = 0;
			for(size_t i = 0 ; i < m_iCols ; i++)
			{
				size_t iLen = 0;
				size_t iCurLen = wcslen(string_get(0, i));
				iLen = iCurLen + SIZE_BETWEEN_TWO_VALUES + ostemp.str().size();
				if(iLen > _iLineLen)
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

				Config_Stream(&ostemp, iCurLen + 2, _iPrecision, ' ');
				ostemp << left << string_get(0,i);
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
			size_t iLen = 0;
			size_t iLastCol = 0;

			//Array with the max printed size of each col
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			for(size_t iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(size_t iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					piSize[iCols1] = Max(piSize[iCols1], wcslen(string_get(iRows1,iCols1)));
				}

				if(iLen + piSize[iCols1] > _iLineLen)
				{//find the limit, print this part
					for(size_t iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						ostemp << L"!";
						for(size_t iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							Config_Stream(&ostemp, piSize[iCols2], _iPrecision, ' ');
							ostemp << left << string_get(iRows2, iCols2) << SPACE_BETWEEN_TWO_VALUES;
						}

						ostemp << L"!" << endl;
						if((iRows2 + 1) != m_iRows)
						{
							ostemp << L"!";
							Config_Stream(&ostemp, iLen, _iPrecision, ' ');
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

			for(size_t iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				iLen = 0;
				ostemp << L"!";
				for(size_t iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					Config_Stream(&ostemp, piSize[iCols2], _iPrecision, ' ');
					ostemp << left << string_get(iRows2, iCols2) << SPACE_BETWEEN_TWO_VALUES;
					iLen += piSize[iCols2] + SIZE_BETWEEN_TWO_VALUES;
				}
				ostemp << L"!" << endl;
				if((iRows2 + 1) != m_iRows)
				{
					ostemp << L"!";
					Config_Stream(&ostemp, iLen, _iPrecision, ' ');
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
		return ostr.str();
	}

	bool String::operator==(const InternalType& it)
	{
		if(const_cast<InternalType&>(it).getType() != RealString)
		{
			return false;
		}

		String* pS = const_cast<InternalType&>(it).getAsString();

		if(pS->rows_get() != rows_get() || pS->cols_get() != cols_get())
		{
			return false;
		}

		wchar_t **p1 = string_get();
		wchar_t **p2 = pS->string_get();

		for(size_t i = 0 ; i < size_get() ; i++)
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

	GenericType* String::get_col_value(size_t _iPos)
	{
		String *ps = NULL;
		if(_iPos < m_iCols)
		{
			ps = new String(m_iRows, 1);
			for(size_t i = 0 ; i < m_iRows ; i++)
			{
				ps->string_set(i, 0, string_get(i, _iPos));
			}
		}
		return ps;
	}

	bool String::insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector)
	{
		size_t iNewRows = rows_get();
		size_t iNewCols = cols_get();
		//check input size
		if(_bAsVector == false)
		{
			if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
			{//compute new dimensions
				iNewRows = Max(_piMaxDim[0], rows_get());
				iNewCols = Max(_piMaxDim[1], cols_get());
			}
		}
		else
		{
			if(size_get() < _piMaxDim[0])
			{
				if(rows_get() == 1 || size_get() == 0)
				{
					iNewRows = 1;
					iNewCols = _piMaxDim[0];
				}
				else if(cols_get() == 1)
				{
					iNewRows = _piMaxDim[0];
					iNewCols = 1;
				}
				else
				{
					return false;
				}
			}
		}

		//check if the size of _poSource is compatible with the size of the variable
		if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
		{
			return false;
		}
		else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->size_get()))
		{
			return false;
		}


		//check if the count of values is compatible with indexes
		if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
		{
			return false;
		}


		switch(_poSource->getType())
		{
		case InternalType::RealString :
			{
				String *pIn = _poSource->getAsString();

				//Only resize after all tests !
				if(resize(iNewRows, iNewCols) == false)
				{
					return false;
				}

				wchar_t** pstIn = pIn->string_get();

				//variable can receive new values.
				if(pIn->size_get() == 1)
				{//a(?) = x
					if(_bAsVector)
					{//a([]) = R
						for(size_t i = 0 ; i < _iSeqCount ; i++)
						{
							m_pstData[_piSeqCoord[i] - 1]	= os_wcsdup(pstIn[0]);
						}
					}
					else
					{//a([],[]) = R
						for(size_t i = 0 ; i < _iSeqCount ; i++)
						{
							size_t iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
							m_pstData[iPos]	= os_wcsdup(pstIn[0]);
						}
					}
				}
				else
				{//a(?) = [x]
					if(_bAsVector)
					{//a([]) = [R]
						for(size_t i = 0 ; i < _iSeqCount ; i++)
						{
							m_pstData[_piSeqCoord[i] - 1]	= os_wcsdup(pstIn[i]);
						}
					}
					else
					{//a([],[]) = [R]
						for(size_t i = 0 ; i < _iSeqCount ; i++)
						{
							size_t iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
							size_t iTempR = i / pIn->cols_get();
							size_t iTempC = i % pIn->cols_get();
							size_t iNew_i = iTempR + iTempC * pIn->rows_get();

							m_pstData[iPos]	= os_wcsdup(pstIn[iNew_i]);
						}
					}
				}
			break;
			}
		default :
			return false;
			break;
		}
		return true;
	}

	String*	String::insert_new(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, String* _poSource, bool _bAsVector)
	{
		String *pS	= NULL ;

		if(_bAsVector)
		{
			if(_poSource->cols_get() == 1)
			{
				pS = new String(_piMaxDim[0], 1);
			}
			else if(_poSource->rows_get() == 1)
			{
				pS = new String(1, _piMaxDim[0]);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			pS = new String(_piMaxDim[0], _piMaxDim[1]);
		}

		if(pS->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
		{
			delete pS;
			return NULL;
		}

		return pS;
	}

	bool String::resize(size_t _iNewRows, size_t _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		//alloc new data array
		wchar_t** pst = NULL;

		pst	= new wchar_t*[_iNewRows * _iNewCols];
		for(size_t i = 0 ; i < _iNewRows * _iNewCols ; i++)
		{
			pst[i] = os_wcsdup(L"");
		}

		//copy existing values
		for(size_t i = 0 ; i < rows_get() ; i++)
		{
			for(size_t j = 0 ; j < cols_get() ; j++)
			{
				if(pst[j * _iNewRows + i])
				{
					delete pst[j * _iNewRows + i];
				}
				pst[j * _iNewRows + i] = m_pstData[j * rows_get() + i];
			}
		}
		delete[] m_pstData;
		m_pstData	= pst;


		m_iRows = _iNewRows;
		m_iCols	= _iNewCols;
		m_iSize = m_iRows * m_iCols;
		return true;
	}

	String*	String::extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector)
	{
		String* pOut	= NULL;
		size_t iRowsOut	= 0;
		size_t iColsOut	= 0;

		//check input param

		if(	(_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
		{
			return NULL;
		}

		if(_bAsVector)
		{//a([])
			if(rows_get() == 1)
			{
				iRowsOut	= 1;
				iColsOut	= _piDimSize[0];
			}
			else
			{
				iRowsOut	= _piDimSize[0];
				iColsOut	= 1;
			}
		}
		else
		{//a([],[])
			iRowsOut	= _piDimSize[0];
			iColsOut	= _piDimSize[1];
		}

		pOut				= new String(iRowsOut, iColsOut);
		wchar_t** pst	= pOut->string_get();


		if(_bAsVector)
		{
			for(size_t i = 0 ; i < _iSeqCount ; i++)
			{
				pst[i] = os_wcsdup(m_pstData[_piSeqCoord[i] - 1]);
			}
		}
		else
		{
			for(size_t i = 0 ; i < _iSeqCount ; i++)
			{
				//convert vertical indexes to horizontal indexes
				size_t iCurIndex   = (i % iColsOut) * iRowsOut + (i / iColsOut);
				size_t iInIndex    = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
				pst[iCurIndex]  = os_wcsdup(m_pstData[iInIndex]);
			}
		}

		return pOut;
	}

    bool String::append(size_t _iRows, size_t _iCols, String *_poSource)
    {
        size_t iRows = _poSource->rows_get();
        size_t iCols = _poSource->cols_get();

        //insert without resize
        if(iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
        {
            return false;
        }

        for(size_t i = 0 ; i < iRows ; i++)
        {
            for(size_t j = 0 ; j < iCols ; j++)
            {
                string_set(_iRows + i, _iCols + j, _poSource->string_get(i,j));
            }
        }

        return true;
    }

}

