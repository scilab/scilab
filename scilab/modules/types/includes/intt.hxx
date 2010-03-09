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

#ifndef __INTT_HH__
#define __INTT_HH__

#include "core_math.h"
#include "int.hxx"
#include "types.hxx"
#include "tostring_common.hxx"
namespace types
{
	template <typename T> class IntT
	{
		int m_iRows;
		int m_iCols;
		int m_iSize;
		T* m_pData;

	public :
		//IntT
		void createInt(int _iRows, int _iCols)
		{
			m_iRows				= _iRows;
			m_iCols				= _iCols;
			m_iSize				= _iRows * _iCols;

			m_pData				= new T[m_iSize];
		}

		IntT(int _iRows, int _iCols)
		{
			createInt(_iRows, _iCols);
		}

		IntT(T _TData)
		{
			createInt(1, 1);
			m_pData[0]		= _TData;
		}

		IntT(int _iRows, int _iCols, T* _pTData)
		{
			createInt(_iRows, _iCols);
			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_pData[i] = _pTData[i];
			}
		}

		bool data_set(Int* _pTData)
		{
			if(m_iSize != _pTData->size_get())
			{
				return false;
			}

			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_pData[i] = (T)_pTData->data_get(i);
			}
			return true;
		}

		template<typename Y>
		bool data_set(int _iRows, int _iCols, Y _YData)
		{
			if(_iRows >= m_iRows || _iCols >= m_iRows)
			{
				return false;
			}

			return data_set(_iCols * m_iRows + _iRows, _YData);
		}

		template<typename Y>
		bool data_set(int _iPos, Y _YData)
		{
			if(_iPos >= m_iSize)
			{
				return false;
			}

			m_pData[_iPos] = (T)_YData;
			return true;
		}

		T*	data_get()
		{
			return m_pData;
		}

		T data_get(int _iPos)
		{
			return m_pData[_iPos];
		}

		T data_get(int _iRows, int _iCols)
		{
			return data_get(_iCols * m_iRows + _iRows);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			//alloc new data array
			T* pT = NULL;

			pT = new T[_iNewRows * _iNewCols];
			memset(pT, 0x00, sizeof(T) * _iNewRows * _iNewCols);

			for(int i = 0 ; i < m_iRows ; i++)
			{
				for(int j = 0 ; j < m_iCols ; j++)
				{
					pT[j * _iNewRows + i] = m_pData[j * m_iRows + i];
				}
			}
			delete[] m_pData;
			m_pData	= pT;
			return true;
		}

		T* get_col_value(int _iPos)
		{
			return NULL;
		}

		Int* extract(Int* _poOut, int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			if(_bAsVector)
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					T TTemp = data_get(_piSeqCoord[i] - 1);
					(T)_poOut->data_set(i, TTemp);
				}
			}
			else
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					//convert vertical indexes to horizontal indexes
					int iOutIndex				= (i % _poOut->cols_get()) * _poOut->rows_get() + (i / _poOut->cols_get());
					int iInIndex				= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
					(T)_poOut->data_set(iOutIndex,data_get(iInIndex));
				}
			}
			return _poOut;
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			Int* pIn = _poSource->getAsInt();
			if(pIn->size_get() == 1)
			{//a(?) = x
				if(_bAsVector)
				{//a([]) = R
					for(int i = 0 ; i < _iSeqCount ; i++)
					{
						m_pData[_piSeqCoord[i] - 1]	= (T)pIn->data_get(0);
					}
				}
				else
				{//a([],[]) = R
					for(int i = 0 ; i < _iSeqCount ; i++)
					{
						int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
						m_pData[iPos]	= (T)pIn->data_get(0);
					}
				}
			}
			else
			{//a(?) = [x]
				if(_bAsVector)
				{//a([]) = [R]
					for(int i = 0 ; i < _iSeqCount ; i++)
					{
						m_pData[_piSeqCoord[i] - 1]	= (T)pIn->data_get(i);
					}
				}
				else
				{//a([],[]) = [R]
					for(int i = 0 ; i < _iSeqCount ; i++)
					{
						int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * m_iRows;
						int iTempR = i / pIn->cols_get();
						int iTempC = i % pIn->cols_get();
						int iNew_i = iTempR + iTempC * pIn->rows_get();

						m_pData[iPos]	= (T)pIn->data_get(iNew_i);
					}
				}
			}
			return true;
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			ostringstream ostr;
			if(m_iRows == 1 && m_iCols == 1)
			{//scalar
				int iWidth = 0;
				GetIntFormat<T>(m_pData[0], &iWidth);
				AddIntValue<T>(&ostr, m_pData[0], iWidth);
				ostr << endl;
			}
			else if(m_iCols == 1)
			{//column vector
				for(int i = 0 ; i < m_iRows ; i++)
				{
					int iWidth = 0;
					GetIntFormat<T>(m_pData[i], &iWidth);
					AddIntValue<T>(&ostr, m_pData[i], iWidth);
					ostr << endl;
				}
			}
			else if(m_iRows == 1)
			{//row vector
				ostringstream ostemp;
				int iLastVal = 0;

				for(int i = 0 ; i < m_iCols ; i++)
				{
					int iWidth = 0;
					GetIntFormat<T>(m_pData[i], &iWidth);
					int iLen = iWidth + (int)ostemp.str().size();
					if(iLen > _iLineLen)
					{//Max length, new line
						ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
						ostr << ostemp.str() << endl;
						ostemp.str("\x00");
						iLastVal = i;
					}

					AddIntValue<T>(&ostemp, m_pData[i], iWidth);
				}

				if(iLastVal != 0)
				{
					ostr << endl << "       column " << iLastVal + 1 << " to " << m_iCols << endl << endl;
				}
				ostemp << endl;
				ostr << ostemp.str();
			}
			else // matrix
			{
				ostringstream ostemp;
				int iLastVal = 0;
				int iLen = 0;
				int iLastCol = 0;

				//Array with the max printed size of each col
				int *piSize = new int[m_iCols];
				memset(piSize, 0x00, m_iCols * sizeof(int));

				//compute the row size for padding for each printed bloc.
				for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)
				{
					for(int iRows1 = 0 ; iRows1 < m_iRows ; iRows1++)
					{
						int iWidth			= 0;
						int iCurrentLen = 0;

						GetIntFormat<T>(m_pData[iCols1 * m_iRows + iRows1], &iWidth);
						iCurrentLen	= iWidth ;

						if(iCurrentLen > piSize[iCols1])
						{
							piSize[iCols1] = iCurrentLen;
						}
					}

					if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
					{//find the limit, print this part
						for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
						{
							for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
							{
								int iWidth	= 0;
								GetIntFormat<T>(m_pData[iCols2 * m_iRows + iRows2], &iWidth);
								AddIntValue<T>(&ostemp, m_pData[iCols2 * m_iRows + iRows2], piSize[iCols2]);
							}
							ostemp << endl;
						}
						iLen = 0;
						ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;;
						ostr << ostemp.str();
						ostemp.str("");
						iLastCol = iCols1;

					}
					iLen += piSize[iCols1] + SIGN_LENGTH;
				}

				for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
				{
					for(int iCols2 = iLastCol ; iCols2 < m_iCols ; iCols2++)
					{
						int iWidth			= 0;
						GetIntFormat<T>(m_pData[iCols2 * m_iRows + iRows2], &iWidth);
						AddIntValue<T>(&ostemp, m_pData[iCols2 * m_iRows + iRows2], piSize[iCols2]);
					}
					ostemp << endl;
				}
				if(iLastCol != 0)
				{
					ostr << endl << "       column " << iLastCol + 1 << " to " << m_iCols << endl << endl;
				}
				ostr << ostemp.str();
			}
			return ostr.str();
		}
	};

	class Int8 : public Int
	{
	public :
		~Int8(){};

		Int8(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::Type8;
			m_pint = new IntT<char>((char)_llData);
		}

		Int8(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type8;
			m_pint = new IntT<char>(_iRows, _iCols);
		}

		Int8(int _iRows, int _iCols, char* _pcData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type8;
			m_pint = new IntT<char>(_iRows, _iCols, _pcData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			char* pData		= m_pint->get_col_value(_iPos);
			Int8 *pi			= new Int8(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			Int8* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new Int8(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<char> *m_pint;

	};
	
	class UInt8 : public Int
	{
	public :
		~UInt8(){};

		UInt8(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::TypeUnsigned8;
			m_pint = new IntT<unsigned char>((unsigned char)_llData);
		}

		UInt8(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned8;
			m_pint = new IntT<unsigned char>(_iRows, _iCols);
		}

		UInt8(int _iRows, int _iCols, unsigned char* _pucData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned8;
			m_pint = new IntT<unsigned char>(_iRows, _iCols, _pucData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			unsigned char* pData		= m_pint->get_col_value(_iPos);
			UInt8 *pi			= new UInt8(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			UInt8* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new UInt8(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<unsigned char> *m_pint;

	};

//16 bits
	class Int16 : public Int
	{
	public :
		~Int16(){};

		Int16(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::Type16;
			m_pint = new IntT<short>((short)_llData);
		}

		Int16(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type16;
			m_pint = new IntT<short>(_iRows, _iCols);
		}

		Int16(int _iRows, int _iCols, short* _psData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type16;
			m_pint = new IntT<short>(_iRows, _iCols, _psData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			short* pData	= m_pint->get_col_value(_iPos);
			Int16 *pi			= new Int16(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			Int16* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new Int16(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<short> *m_pint;

	};
	
	class UInt16 : public Int
	{
	public :
		~UInt16(){};

		UInt16(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::TypeUnsigned16;
			m_pint = new IntT<unsigned short>((unsigned short)_llData);
		}

		UInt16(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned16;
			m_pint = new IntT<unsigned short>(_iRows, _iCols);
		}

		UInt16(int _iRows, int _iCols, unsigned short* _pusData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned16;
			m_pint = new IntT<unsigned short>(_iRows, _iCols, _pusData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			unsigned short* pData	= m_pint->get_col_value(_iPos);
			UInt16 *pi							= new UInt16(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			UInt16* pOut	= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new UInt16(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<unsigned short> *m_pint;

	};

//32 bits
	class Int32 : public Int
	{
	public :
		~Int32(){};

		Int32(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::Type32;
			m_pint = new IntT<int>((int)_llData);
		}

		Int32(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type32;
			m_pint = new IntT<int>(_iRows, _iCols);
		}

		Int32(int _iRows, int _iCols, int* _piData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type32;
			m_pint = new IntT<int>(_iRows, _iCols, _piData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			int* pData		= m_pint->get_col_value(_iPos);
			Int32 *pi			= new Int32(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			Int32* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new Int32(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<int> *m_pint;

	};
	
	class UInt32 : public Int
	{
	public :
		~UInt32(){};

		UInt32(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::TypeUnsigned32;
			m_pint = new IntT<unsigned int>((unsigned int)_llData);
		}

		UInt32(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned32;
			m_pint = new IntT<unsigned int>(_iRows, _iCols);
		}

		UInt32(int _iRows, int _iCols, unsigned int* _puiData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned32;
			m_pint = new IntT<unsigned int>(_iRows, _iCols, _puiData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			unsigned int* pData	= m_pint->get_col_value(_iPos);
			UInt32 *pi					= new UInt32(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			UInt32* pOut	= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new UInt32(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<unsigned int> *m_pint;

	};

//64 bits
	class Int64 : public Int
	{
	public :
		~Int64(){};

		Int64(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::Type64;
			m_pint = new IntT<long long>((char)_llData);
		}

		Int64(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type64;
			m_pint = new IntT<long long>(_iRows, _iCols);
		}

		Int64(int _iRows, int _iCols, long long* _pllData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::Type64;
			m_pint = new IntT<long long>(_iRows, _iCols, _pllData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			long long* pData	= m_pint->get_col_value(_iPos);
			Int64 *pi					= new Int64(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			Int64* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new Int64(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<long long> *m_pint;

	};
	
	class UInt64 : public Int
	{
	public :
		~UInt64(){};

		UInt64(long long _llData)
			:Int(_llData)
		{
			m_iIntType = Int::TypeUnsigned64;
			m_pint = new IntT<unsigned long long>((unsigned long long)_llData);
		}

		UInt64(int _iRows, int _iCols)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned64;
			m_pint = new IntT<unsigned long long>(_iRows, _iCols);
		}

		UInt64(int _iRows, int _iCols, unsigned long long* _pullData)
			:Int(_iRows, _iCols)
		{
			m_iIntType = Int::TypeUnsigned64;
			m_pint = new IntT<unsigned long long>(_iRows, _iCols, _pullData);
		}

		bool data_set(Int* _pTData)
		{
			return m_pint->data_set(_pTData);
		}

		bool data_set(int _iRows, int _iCols, long long _llData)
		{
			return m_pint->data_set(_iRows, _iCols, _llData);
		}

		bool data_set(int _iPos, long long _llData)
		{
			return m_pint->data_set(_iPos, _llData);
		}

		void*	data_get()
		{
			return (void*)m_pint->data_get();
		}

		long long data_get(int _iPos)
		{
			return m_pint->data_get(_iPos);
		}

		long long data_get(int _iRows, int _iCols)
		{
			return (long long)m_pint->data_get(_iRows, _iCols);
		}

		bool resize(int _iNewRows, int _iNewCols)
		{
			return m_pint->resize(_iNewRows, _iNewCols);
		}

		GenericType* get_col_value(int _iPos)
		{
			unsigned long long* pData	= m_pint->get_col_value(_iPos);
			UInt64 *pi								= new UInt64(rows_get(), 1, pData);

			delete pData;
			return pi;
		}

		Int* extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
		{
			UInt64* pOut		= NULL;
			int iRowsOut	= 0;
			int iColsOut	= 0;

			if(extract_size_get(_piMaxDim, _piDimSize, _bAsVector, &iRowsOut, &iColsOut) == false)
			{
				return NULL;
			}
			
			pOut	= new UInt64(iRowsOut, iColsOut);
			return m_pint->extract(pOut, _iSeqCount, _piSeqCoord, _piMaxDim, _piDimSize, _bAsVector);
		}

		bool insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
		{
			int iNewRows = rows_get();
			int iNewCols = cols_get();

			if(Int::insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
			{
				return false;
			}

			if(resize(iNewRows, iNewCols) == false)
			{
				return false;
			}

			//call insert on m_pint
			return m_pint->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector);
		}

		string toString(int _iPrecision, int _iLineLen)
		{
			return m_pint->toString(_iPrecision, _iLineLen);
		}

	private :
		IntT<unsigned long long> *m_pint;

	};
}
#endif /* __INTT_HH__ */
