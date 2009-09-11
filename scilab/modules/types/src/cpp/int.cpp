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
#include "int.hxx"
#include "tostring_common.hxx"

using namespace std;

namespace types
{
	Int::~Int()
	{
	}

	void Int::whoAmI(void)
	{
		std::cout << "types::Int";
	}

	InternalType::RealType Int::getType(void)
	{
		return RealInt;
	}

	Int* Int::getAsInt(void)
	{
		return this;
	}

	Int::Int(char _cData)
	{
		CreateInt(1, 1, Type8, &_cData);
	}

	Int::Int(unsigned char _ucData)
	{
		CreateInt(1, 1, TypeUnsigned8, &_ucData);
	}

	Int::Int(short _sData)
	{
		CreateInt(1, 1, Type16, &_sData);
	}

	Int::Int(unsigned short _usData)
	{
		CreateInt(1, 1, TypeUnsigned16, &_usData);
	}

	Int::Int(int _iData)
	{
		CreateInt(1, 1, Type32, &_iData);
	}

	Int::Int(unsigned int _uiData)
	{
		CreateInt(1, 1, TypeUnsigned32, &_uiData);
	}

	Int::Int(long long _llData)
	{
		CreateInt(1, 1, Type64, &_llData);
	}

	Int::Int(unsigned long long _ullData)
	{
		CreateInt(1, 1, TypeUnsigned64, &_ullData);
	}

	Int::Int(int _iRows, int _iCols, char* _pcData)
	{
		CreateInt(_iRows, _iCols, Type8, _pcData);
	}

	Int::Int(int _iRows, int _iCols, unsigned char* _pucData)
	{
		CreateInt(_iRows, _iCols, TypeUnsigned8, _pucData);
	}

	Int::Int(int _iRows, int _iCols, short* _psData)
	{
		CreateInt(_iRows, _iCols, Type16, _psData);
	}

	Int::Int(int _iRows, int _iCols, unsigned short* _pusData)
	{
		CreateInt(_iRows, _iCols, TypeUnsigned16, _pusData);
	}

	Int::Int(int _iRows, int _iCols, int* _piData)
	{
		CreateInt(_iRows, _iCols, Type32, _piData);
	}

	Int::Int(int _iRows, int _iCols, unsigned int* _puiData)
	{
		CreateInt(_iRows, _iCols, TypeUnsigned32, _puiData);
	}

	Int::Int(int _iRows, int _iCols, long long* _pllData)
	{
		CreateInt(_iRows, _iCols, Type8, _pllData);
	}

	Int::Int(int _iRows, int _iCols, unsigned long long* _pullData)
	{
		CreateInt(_iRows, _iCols, TypeUnsigned64, _pullData);
	}

	Int::Int(int _iRows, int _iCols, IntType _itType)
	{
		CreateInt(_iRows, _iCols, _itType, NULL);
	}

	void Int::CreateInt(int _iRows, int _iCols, IntType _itType, void* _pvData)
	{
		void* pvData	= NULL;
		m_iRows			= _iRows;
		m_iCols			= _iCols;
		m_iSize			= m_iRows * m_iCols;
		m_itType		= _itType;

		switch(_itType)
		{
		case Type8 :
			m_pcData = new char[m_iSize];
			pvData	= m_pcData;
			break;
		case TypeUnsigned8 :
			m_pucData = new unsigned char[m_iSize];
			pvData	= m_pucData;
			break;
		case Type16 :
			m_psData = new short[m_iSize];
			pvData	= m_psData;
			break;
		case TypeUnsigned16 :
			m_pusData = new unsigned short[m_iSize];
			pvData	= m_pusData;
			break;
		case Type32 :
			m_piData = new int[m_iSize];
			pvData	= m_piData;
			break;
		case TypeUnsigned32 :
			m_puiData = new unsigned int[m_iSize];
			pvData	= m_puiData;
			break;
		case Type64 :
			m_pllData = new long long[m_iSize];
			pvData	= m_pllData;
			break;
		case TypeUnsigned64 :
			m_pullData = new unsigned long long[m_iSize];
			pvData	= m_pullData;
			break;
		default :
			pvData	= NULL;
			break;
		}

		if(_pvData && pvData)
		{//is this case we copy data
			memcpy(pvData, _pvData, (_itType % 10) * m_iSize); 
		}
	}

	void* Int::data_get(void) const
	{
		void* pvData = NULL;
		switch(m_itType)
		{
		case Type8 :
			pvData	= m_pcData;
			break;
		case TypeUnsigned8 :
			pvData	= m_pucData;
			break;
		case Type16 :
			pvData	= m_psData;
			break;
		case TypeUnsigned16 :
			pvData	= m_pusData;
			break;
		case Type32 :
			pvData	= m_piData;
			break;
		case TypeUnsigned32 :
			pvData	= m_puiData;
			break;
		case Type64 :
			pvData	= m_pllData;
			break;
		case TypeUnsigned64 :
			pvData	= m_pullData;
			break;
		default :
			pvData	= NULL;
			break;
		}
		return pvData;
	}

	char Int::data8_get(int _iRows, int _iCols)
	{
		if(m_itType != Type8)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_pcData != NULL)
			{
				return m_pcData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	unsigned char Int::dataUnsigned8_get(int _iRows, int _iCols)
	{
		if(m_itType != TypeUnsigned8)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_pucData != NULL)
			{
				return m_pucData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	short Int::data16_get(int _iRows, int _iCols)
	{
		if(m_itType != Type16)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_psData != NULL)
			{
				return m_psData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	unsigned short Int::dataUnsigned16_get(int _iRows, int _iCols)
	{
		if(m_itType != TypeUnsigned16)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_pusData != NULL)
			{
				return m_pusData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	int Int::data32_get(int _iRows, int _iCols)
	{
		if(m_itType != Type32)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_piData != NULL)
			{
				return m_piData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	unsigned int Int::dataUnsigned32_get(int _iRows, int _iCols)
	{
		if(m_itType != TypeUnsigned32)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_puiData != NULL)
			{
				return m_puiData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	long long Int::data64_get(int _iRows, int _iCols)
	{
		if(m_itType != Type64)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_pllData != NULL)
			{
				return m_pllData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	unsigned long long Int::dataUnsigned64_get(int _iRows, int _iCols)
	{
		if(m_itType != TypeUnsigned64)
		{
			return 0;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			if(m_pullData != NULL)
			{
				return m_pullData[_iCols * m_iRows + _iRows];
			}
		}
		return 0;
	}

	inline bool Int::data_set(char *_pcData)
	{
		return data_set(Type8, _pcData);
	}

	inline bool Int::data_set(unsigned char *_pucData)
	{
		return data_set(TypeUnsigned8, _pucData);
	}

	inline bool Int::data_set(short *_psData)
	{
		return data_set(Type16, _psData);
	}

	inline bool Int::data_set(unsigned short *_pusData)
	{
		return data_set(TypeUnsigned16, _pusData);
	}

	inline bool Int::data_set(int *_piData)
	{
		return data_set(Type32, _piData);
	}

	inline bool Int::data_set(unsigned int *_puiData)
	{
		return data_set(TypeUnsigned32, _puiData);
	}

	inline bool Int::data_set(long long *_pllData)
	{
		return data_set(Type64, _pllData);
	}

	inline bool Int::data_set(unsigned long long *_pullData)
	{
		return data_set(TypeUnsigned64, _pullData);
	}

	bool Int::data_set(IntType _itType, void *_pvData)
	{
		if(m_itType != _itType)
		{
			return false;
		}

		void* pvData = data_get();
		if(pvData == NULL || _pvData == NULL)
		{
			return false;
		}

		memcpy(pvData, _pvData, (m_itType % 10) * m_iRows * m_iCols);
		return true;
	}

	bool Int::data_set(int _iRows, int _iCols, char _cData)
	{
		if(m_itType != Type8)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			char* pcData = (char*)data_get();
			if(pcData != NULL)
			{
				pcData[_iCols * m_iRows + _iCols] = _cData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, unsigned char _ucData)
	{
		if(m_itType != TypeUnsigned8)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			unsigned char* pucData = (unsigned char*)data_get();
			if(pucData != NULL)
			{
				pucData[_iCols * m_iRows + _iCols] = _ucData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, short _sData)
	{
		if(m_itType != Type16)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			short* psData = (short*)data_get();
			if(psData != NULL)
			{
				psData[_iCols * m_iRows + _iCols] = _sData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, unsigned short _usData)
	{
		if(m_itType != TypeUnsigned16)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			unsigned short* pusData = (unsigned short*)data_get();
			if(pusData != NULL)
			{
				pusData[_iCols * m_iRows + _iCols] = _usData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, int _iData)
	{
		if(m_itType != Type32)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			int* piData = (int*)data_get();
			if(piData != NULL)
			{
				piData[_iCols * m_iRows + _iCols] = _iData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, unsigned int _uiData)
	{
		if(m_itType != TypeUnsigned32)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			unsigned int* puiData = (unsigned int*)data_get();
			if(puiData != NULL)
			{
				puiData[_iCols * m_iRows + _iCols] = _uiData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, long long _llData)
	{
		if(m_itType != Type64)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			long long* pllData = (long long*)data_get();
			if(pllData != NULL)
			{
				pllData[_iCols * m_iRows + _iCols] = _llData;
				return true;
			}
		}
		return false;
	}

	bool Int::data_set(int _iRows, int _iCols, unsigned long long _ullData)
	{
		if(m_itType != TypeUnsigned64)
		{
			return false;
		}

		if(_iRows < m_iRows && _iCols < m_iCols)
		{
			unsigned long long* pullData = (unsigned long long*)data_get();
			if(pullData != NULL)
			{
				pullData[_iCols * m_iRows + _iCols] = _ullData;
				return true;
			}
		}
		return false;
	}

	string Int::toString(int _iPrecision, int _iLineLen)
	{
		string str;

		switch(m_itType)
		{
		case Type8 :
			str = toString8(_iPrecision, _iLineLen);
			break;
		case TypeUnsigned8 :
			str = toStringUnsigned8(_iPrecision, _iLineLen);
			break;
		case Type16 :
			str = toString16(_iPrecision, _iLineLen);
			break;
		case TypeUnsigned16 :
			str = toStringUnsigned16(_iPrecision, _iLineLen);
			break;
		case Type32 :
			str = toString32(_iPrecision, _iLineLen);
			break;
		case TypeUnsigned32 :
			str = toStringUnsigned32(_iPrecision, _iLineLen);
			break;
		case Type64 :
			str = toString64(_iPrecision, _iLineLen);
			break;
		case TypeUnsigned64 :
			str = toStringUnsigned64(_iPrecision, _iLineLen);
			break;
		default :
			// Oo
			break;
		}

		return str;
	}

	string Int::toString8(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_pcData[0], &iWidth);
			AddIntValue(&ostr, m_pcData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pcData[i], &iWidth);
				AddIntValue(&ostr, m_pcData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pcData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_pcData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_pcData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_pcData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_pcData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_pcData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_pcData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toString16(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_psData[0], &iWidth);
			AddIntValue(&ostr, m_psData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_psData[i], &iWidth);
				AddIntValue(&ostr, m_psData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_psData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_psData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_psData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_psData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_psData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_psData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_psData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toString32(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_piData[0], &iWidth);
			AddIntValue(&ostr, m_piData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_piData[i], &iWidth);
				AddIntValue(&ostr, m_piData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_piData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_piData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_piData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_piData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_piData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_piData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_piData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toString64(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_pllData[0], &iWidth);
			AddIntValue(&ostr, m_pllData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pllData[i], &iWidth);
				AddIntValue(&ostr, m_pllData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pllData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_pllData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_pllData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_pllData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_pllData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_pllData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_pllData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toStringUnsigned8(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_pucData[0], &iWidth);
			AddIntValue(&ostr, m_pucData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pucData[i], &iWidth);
				AddIntValue(&ostr, m_pucData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pucData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_pucData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_pucData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_pucData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_pucData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_pucData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_pucData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toStringUnsigned16(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_pusData[0], &iWidth);
			AddIntValue(&ostr, m_pusData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pusData[i], &iWidth);
				AddIntValue(&ostr, m_pusData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pusData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_pusData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_pusData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_pusData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_pusData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_pusData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_pusData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toStringUnsigned32(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_puiData[0], &iWidth);
			AddIntValue(&ostr, m_puiData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_puiData[i], &iWidth);
				AddIntValue(&ostr, m_puiData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_puiData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_puiData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_puiData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_puiData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_puiData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_puiData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_puiData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	string Int::toStringUnsigned64(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
			int iWidth = 0;
			GetIntFormat(m_pullData[0], &iWidth);
			AddIntValue(&ostr, m_pullData[0], iWidth);
			ostr << endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pullData[i], &iWidth);
				AddIntValue(&ostr, m_pullData[i], iWidth);
				ostr << endl;
			}
		}
		else if(rows_get() == 1)
		{//row vector
			ostringstream ostemp;
			int iLastVal = 0;

			for(int i = 0 ; i < cols_get() ; i++)
			{
				int iWidth = 0;
				GetIntFormat(m_pullData[i], &iWidth);
				int iLen = iWidth + (int)ostemp.str().size();
				if(iLen > _iLineLen)
				{//Max length, new line
					ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
					ostr << ostemp.str() << endl;
					ostemp.str("\x00");
					iLastVal = i;
				}

				AddIntValue(&ostemp, m_pullData[i], iWidth);
			}

			if(iLastVal != 0)
			{
				ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
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
			int *piSize = new int[cols_get()];
			memset(piSize, 0x00, cols_get() * sizeof(int));

			//compute the row size for padding for each printed bloc.
			for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
			{
				for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
				{
					int iWidth			= 0;
					int iCurrentLen = 0;

					GetIntFormat(m_pullData[iCols1 * rows_get() + iRows1], &iWidth);
					iCurrentLen	= iWidth ;

					if(iCurrentLen > piSize[iCols1])
					{
						piSize[iCols1] = iCurrentLen;
					}
				}

				if(iLen + piSize[iCols1] + SIGN_LENGTH > _iLineLen)
				{//find the limit, print this part
					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
						{
							int iWidth	= 0;
							GetIntFormat(m_pullData[iCols2 * rows_get() + iRows2], &iWidth);
							AddIntValue(&ostemp, m_pullData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
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

			for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
			{
				for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
				{
					int iWidth			= 0;
					GetIntFormat(m_pullData[iCols2 * rows_get() + iRows2], &iWidth);
					AddIntValue(&ostemp, m_pullData[iCols2 * rows_get() + iRows2], piSize[iCols2]);
				}
				ostemp << endl;
			}
			if(iLastCol != 0)
			{
				ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
			}
			ostr << ostemp.str();
		}
		return ostr.str();
	}

	Int::IntType Int::getIntType(void)
	{
		return m_itType;
	}

	bool Int::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealInt)
		{
			return false;
		}

		Int* pi = pIT->getAsInt();

		if(pi->getIntType() != getIntType())
		{
			return false;
		}

		if(pi->rows_get() != rows_get() || pi->cols_get() != cols_get())
		{
			return false;
		}

		void *pv1 = data_get();
		void *pv2 = pi->data_get();
		if(memcmp(pv1, pv2, size_get() * (m_itType % 10)) != 0)
		{
			return false;
		}

		return true;
	}

	bool Int::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

	GenericType* Int::get(int _iPos)
	{
		Int* pi = NULL;
		if(_iPos < cols_get())
		{
			pi = new Int(rows_get(), 1, getIntType());
			switch(getIntType())
			{
			case Type8 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, data8_get(i, 0));
					}
				}
				break;
			case TypeUnsigned8 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, dataUnsigned8_get(i, 0));
					}
				}
				break;
			case Type16 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, data16_get(i, 0));
					}
				}
				break;
			case TypeUnsigned16 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, dataUnsigned16_get(i, 0));
					}
				}
				break;
			case Type32 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, data32_get(i, 0));
					}
				}
				break;
			case TypeUnsigned32 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, dataUnsigned32_get(i, 0));
					}
				}
				break;
			case Type64 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, data64_get(i, 0));
					}
				}
				break;
			case TypeUnsigned64 : 
				{
					for(int i = 0 ; i < m_iRows ; i++)
					{
						pi->data_set(0, 1, dataUnsigned64_get(i, 0));
					}
				}
				break;
			}
		}
		return pi;
	}

}

/*
*/