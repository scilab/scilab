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

#ifdef _MSC_VER
	#define strdup _strdup
#endif

using namespace std;

#define SIZE_BETWEEN_TWO_VALUES			2
#define SPACE_BETWEEN_TWO_VALUES		"  "

void Config_Stream(ostringstream *_postr, int _iWidth, int _iPrec, char _cFill);


namespace types
{
	String::~String()
	{
		if(isDeletable() == true)
		{
			all_delete();
		}
	}

	String::String(const char *_pcData)
	{
		CreateString(1,1);
		string_set(0,0, _pcData);
	}

	String::String(int _iRows, int _iCols)
	{
		CreateString(_iRows, _iCols);
	}

	String *String::clone()
	{
	  String *pstClone = new String(rows_get(), cols_get());
	  
	  pstClone->string_set(m_pstData);

	  return pstClone;
	}

	void String::CreateString(int _iRows, int _iCols)
	{
		m_iRows		= _iRows;
		m_iCols		= _iCols;
		m_iSize		= m_iRows * m_iCols;

		m_pstData	= new char*[m_iSize];
		for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
		{
			m_pstData[iIndex] = NULL;
		}
	}

	void String::whoAmI()
	{
		cout << "types::String";
	}

	char** String::string_get() const
	{
		return m_pstData;
	}

	char* String::string_get(int _iRows, int _iCols) const
	{
		if(_iRows >= m_iRows || _iCols >= m_iCols)
		{
			return NULL;
		}
		return string_get(_iCols * m_iRows + _iRows);
	}

	char* String::string_get(int _iPos) const
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

	bool String::string_set(int _iRows, int _iCols, const char *_pcData)
	{
		return string_set(_iCols * m_iRows + _iRows, _pcData);
	}

	bool String::string_set(char **_pcData)
	{
		if(_pcData != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				if(_pcData[iIndex] != 0)
				{
					bool bRet = string_set(iIndex, _pcData[iIndex]);
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

	bool String::string_set(int _iPos, const char *_pcData)
	{
		if(m_pstData == NULL)
		{
			return false;
		}

		if(m_pstData[_iPos] != NULL)
		{
			delete m_pstData[_iPos];
		}

		m_pstData[_iPos] = new char[strlen(_pcData) + 1];
		memcpy(m_pstData[_iPos], _pcData, strlen(_pcData) * sizeof(char));
		m_pstData[_iPos][strlen(_pcData)] = '\0';
		//strncpy(m_pstData[_iCols * m_iRows + _iRows], _pcData, strlen(_pcData));

		return true;
	}


	void String::string_delete(int _iPos)
	{
		if(m_pstData != NULL)
		{
			if(m_pstData[_iPos] != NULL)
			{
				delete m_pstData[_iPos];
				m_pstData[_iPos]	= NULL;
			}
		}
	}

	void String::all_delete()
	{
		for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
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

	string String::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;

		if(m_iRows == 1 && m_iCols == 1)
		{
			ostr << string_get(0,0) << endl;
		}
		else if(m_iCols == 1)
		{
			int iMaxLen = 0;
			for(int i = 0 ; i < m_iSize ; i++)
			{
				iMaxLen = Max(iMaxLen, (int)strlen(string_get(i,0)));
			}

			iMaxLen += 2;

			for(int i = 0 ; i < m_iSize ; i++)
			{
				ostr << "!";
				Config_Stream(&ostr, iMaxLen, _iPrecision, ' ');
				ostr << left << string_get(i,0);
				ostr << "!" << endl;
				if((i+1) < m_iSize)
				{
					ostr << "!";
					Config_Stream(&ostr, iMaxLen, _iPrecision, ' ');
					ostr << left << " ";
					ostr << "!" << endl;
				}
			}
		}
		else if(m_iRows == 1)
		{
			ostringstream ostemp;
			int iLastVal = 0;
			ostemp << "!";
			for(int i = 0 ; i < m_iCols ; i++)
			{
				int iLen = 0;
				int iCurLen = (int)strlen(string_get(0, i));
				iLen = iCurLen + SIZE_BETWEEN_TWO_VALUES + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					if(iLastVal + 1 == i)
					{
						cout << endl << "       column " << iLastVal + 1 << endl << endl;
					}
					else
					{
						cout << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					}
					cout << ostemp.str() << "!" << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				Config_Stream(&ostemp, iCurLen + 2, _iPrecision, ' ');
				ostemp << left << string_get(0,i);
			}

			if(iLastVal != 0)
			{
				if(iLastVal + 1 == m_iCols)
				{
					cout << endl << "       column " << iLastVal + 1 << endl << endl;
				}
				else
				{
					ostr << endl << "       column " << iLastVal + 1 << " to " << m_iCols << endl << endl;
				}
			}
			ostr << "!" << ostemp.str() << "!" << endl;
		}
		else //Matrix
		{
			ostringstream ostemp;
			int iLastVal = 0;
			int iLen = 0;
			int iLastCol = 0;

			//Array with the max printed size of each col
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					piSize[iCols1] = Max(piSize[iCols1], (int)strlen(string_get(iRows1,iCols1)));
				}

				if(iLen + piSize[iCols1] > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						ostemp << "!";
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							Config_Stream(&ostemp, piSize[iCols2], _iPrecision, ' ');
							ostemp << left << string_get(iRows2, iCols2) << SPACE_BETWEEN_TWO_VALUES;
						}

						ostemp << "!" << endl;
						if((iRows2 + 1) != m_iRows)
						{
							ostemp << "!";
							Config_Stream(&ostemp, iLen, _iPrecision, ' ');
							ostemp << left << " ";
							ostemp << "!" << endl;
						}
					}

					iLen = 0;
					if(iCols1 + 1 == m_iCols)
					{
						ostr << endl << "       column " << iCols1 << endl << endl;
					}
					else
					{
						ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;
					}
					ostr << ostemp.str();
					ostemp.str("");
					iLastCol = iCols1;
				}
				iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_VALUES;
			}

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				iLen = 0;
				ostemp << "!";
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					Config_Stream(&ostemp, piSize[iCols2], _iPrecision, ' ');
					ostemp << left << string_get(iRows2, iCols2) << SPACE_BETWEEN_TWO_VALUES;
					iLen += piSize[iCols2] + SIZE_BETWEEN_TWO_VALUES;
				}
				ostemp << "!" << endl;
				if((iRows2 + 1) != m_iRows)
				{
					ostemp << "!";
					Config_Stream(&ostemp, iLen, _iPrecision, ' ');
					ostemp << left << " ";
					ostemp << "!" << endl;
				}
			}

			if(iLastCol != 0)
			{
				if(iLastCol + 1 == m_iCols)
				{
					ostr << endl << "       column " << m_iCols << endl << endl;
				}
				else
				{
					ostr << endl << "       column " << iLastCol + 1 << " to " << m_iCols << endl << endl;
				}
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	bool String::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealString)
		{
			return false;
		}

		String* pS = pIT->getAsString();

		if(pS->rows_get() != rows_get() || pS->cols_get() != cols_get())
		{
			return false;
		}

		char **p1 = string_get();
		char **p2 = pS->string_get();

		for(int i = 0 ; i < size_get() ; i++)
		{
			if(strcmp(p1[i], p2[i]) != 0)
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

	GenericType* String::get(int _iPos)
	{
		String *ps = NULL;
		if(_iPos < m_iCols)
		{
			ps = new String(m_iRows, 1);
			for(int i = 0 ; i < m_iRows ; i++)
			{
				ps->string_set(i, 0, string_get(i, _iPos));
			}
		}
		return ps;
	}

	bool String::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
	{
		int iNewRows = rows_get();
		int iNewCols = cols_get();
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

				char** pstIn = pIn->string_get();

				//variable can receive new values.
				if(pIn->size_get() == 1)
				{//a(?) = x
					if(_bAsVector)
					{//a([]) = R
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							m_pstData[_piSeqCoord[i] - 1]	= strdup(pstIn[0]);
						}
					}
					else
					{//a([],[]) = R
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
							m_pstData[iPos]	= strdup(pstIn[0]);
						}
					}
				}
				else
				{//a(?) = [x]
					if(_bAsVector)
					{//a([]) = [R]
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							m_pstData[_piSeqCoord[i] - 1]	= strdup(pstIn[i]);
						}
					}
					else
					{//a([],[]) = [R]
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
							int iTempR = i / pIn->cols_get();
							int iTempC = i % pIn->cols_get();
							int iNew_i = iTempR + iTempC * pIn->rows_get();

							m_pstData[iPos]	= strdup(pstIn[iNew_i]);
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

	String*	String::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, String* _poSource, bool _bAsVector)
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

	bool String::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		//alloc new data array
		char** pst = NULL;

		pst	= new char*[_iNewRows * _iNewCols];
		for(int i = 0 ; i < _iNewRows * _iNewCols ; i++)
		{
			pst[i] = strdup("");
		}

		//copy existing values
		for(int i = 0 ; i < rows_get() ; i++)
		{
			for(int j = 0 ; j < cols_get() ; j++)
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

	String*	String::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
	{
		String* pOut	= NULL;
		int iRowsOut	= 0;
		int iColsOut	= 0;

		//check input param

		if(	_bAsVector && _piMaxDim[0] > size_get() ||
				_bAsVector == false && _piMaxDim[0] > rows_get() ||
				_bAsVector == false && _piMaxDim[1] > cols_get())
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
		char** pst	= pOut->string_get();


		if(_bAsVector)
		{
			for(int i = 0 ; i < _iSeqCount ; i++)
			{
				pst[i] = strdup(m_pstData[_piSeqCoord[i] - 1]);
			}
		}
		else
		{
			int iRowIn = rows_get();
			for(int i = 0 ; i < _iSeqCount ; i++)
			{
				//convert vertical indexes to horizontal indexes
				int iCurIndex		= (i % iColsOut) * iRowsOut + (i / iColsOut);
				int iInIndex		= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
				pst[iCurIndex]	= strdup(m_pstData[iInIndex]);
			}
		}
		
		return pOut;
	}

}

