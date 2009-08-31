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

#include "int.hxx"

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
/*		Int *pi = (*it_scope).second->getAsInt();
		ostr << "( " << pi->rows_get() << ", " << pi->cols_get() << " )" << std::endl;
		int iCols = pi->cols_get();
		int iRows = pi->rows_get();
		if(iRows == 1 && iCols == 1)
		{
			ostr << pi->real_get(0,0);
			if(pi->isComplex())
			{
				ostr << (pi->img_get(0,0) < 0 ? " " : " +") << pi->img_get(0,0) << "i";
			}

			ostr << std::endl;
		}
		else
		{
			ostr << "[ ";
			for(int r = 0 ; r < iRows ; r++)
			{
				for(int c = 0 ; c < iCols ; c++)
				{
					ostr << pi->real_get(r, c);
					if(pi->isComplex())
					{
						ostr << (pi->img_get(r, c) < 0 ? " " : " +") << pi->img_get(r, c) << "i";
					}

					if((c + 1) != iCols || (r + 1) != iRows)
					{
						ostr << " , ";
					}
				}
				ostr  << std::endl;
			}
			ostr << " ]" << std::endl;
		}
*/
		return "TODO Int print\n";
	}

	Int::IntType Int::getIntType(void)
	{
		return m_itType;
	}
}

/*
*/