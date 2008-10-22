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

#include "string.hxx"

namespace types
{

	String::String(const char *_pcData)
	{
		CreateString(1,1);
		string_set(0,0, _pcData);
	}

	String::String(int _iRows, int _iCols)
	{
		CreateString(_iRows, _iCols);
	}

	void String::CreateString(int _iRows, int _iCols)
	{
		m_iRows		= _iRows;
		m_iCols		= _iCols;
		m_iSize		= m_iRows * m_iCols;

		m_pcData	= new char*[m_iSize];
		for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
		{
			m_pcData[iIndex] = NULL;
		}
	}

	void String::whoAmI()
	{
		std::cout << "types::String" << std::endl;
	}

	char** String::string_get() const
	{
		return m_pcData;
	}

	char* String::string_get(int _iRows, int _iCols) const
	{
		if(m_pcData != NULL)
		{
			return m_pcData[_iCols * m_iRows + _iRows];
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

	bool String::string_set(const char **_pcData)
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
		if(m_pcData == NULL)
		{
			return false;
		}
		
		if(m_pcData[_iPos] != NULL)
		{
			delete m_pcData[_iPos];
		}

		m_pcData[_iPos] = new char[strlen(_pcData) + 1];
		memcpy(m_pcData[_iPos], _pcData, strlen(_pcData) * sizeof(char));
		m_pcData[_iPos][strlen(_pcData)] = '\0';
		//strncpy(m_pcData[_iCols * m_iRows + _iRows], _pcData, strlen(_pcData));

		return true;	}

	
	void String::string_delete(int _iPos)
	{
		if(m_pcData != NULL)
		{
			if(m_pcData[_iPos] != NULL)
			{
				delete m_pcData[_iPos];
				m_pcData[_iPos]	= NULL;
			}
		}
	}

	void String::all_delete()
	{
		for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
		{
			string_delete(iIndex);
		}
		delete m_pcData;
		m_pcData = NULL;
	}


	String* String::getAsString(void)
	{
		return this;
	}

	GenericType::RealType String::getType()
	{
		return GenericType::RealString; 
	}
}