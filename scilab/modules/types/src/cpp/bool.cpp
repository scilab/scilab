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

#include "bool.hxx"
#include <sstream>

namespace types
{
	/*--------*/
	/*	~Bool	*/
	/*--------*/
	Bool::~Bool()
	{
		all_delete();
	}

	/*--------------------*/
	/*				Bool				*/
	/*	Empty constructor	*/
	/*--------------------*/
	Bool::Bool(int _iRows, int _iCols)
	{
		bool *pbool = NULL;
		CreateBool(_iRows, _iCols, &pbool);
		return;
	}

	/*--------------------*/
	/*				Bool				*/
	/*	Real constructor	*/
	/*--------------------*/
	Bool::Bool(bool _bReal)
	{
		bool *pbVal;
		CreateBool(1, 1, &pbVal);
		pbVal[0] = _bReal;
		return;
	}

	/*--------------------*/
	/*				Bool				*/
	/*	Real constructor	*/
	/*--------------------*/
	Bool::Bool(int _iRows, int _iCols, bool **_pbData)
	{
		CreateBool(_iRows, _iCols, _pbData);
		return;
	}

	/*----------------------*/
	/*			CreateBool			*/
	/*	Commun constructor	*/
	/*----------------------*/
	void Bool::CreateBool(int _iRows, int _iCols, bool **_pbData)
	{
		m_iCols	= _iCols;
		m_iRows	= _iRows;
		m_iSize = m_iCols * m_iRows;

		if(_pbData != NULL)
		{
			/*alloc Real array*/
			m_pbData = new bool[m_iSize];

			/*return it*/
			*_pbData = m_pbData;
		}
		else
			m_pbData = NULL;
	}

	bool Bool::isComplex()
	{
		return false;
	}

	/*------------*/
	/*	bool_get	*/
	/*------------*/
	bool*	Bool::bool_get() const
	{ 
		return m_pbData;
	}

	/*------------*/
	/*	bool_get	*/
	/*------------*/
	bool	Bool::bool_get(int _iRow, int _iCol) const
	{ 
		if(m_pbData != NULL)
		{
			return m_pbData[_iCol * m_iRows + _iRow];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	bool_set	*/
	/*------------*/
	bool Bool::bool_set(bool *_pbData)
	{
		if(_pbData != NULL)
		{
			if(m_pbData == NULL)
			{
				m_pbData = new bool[m_iSize];
			}

			memcpy(m_pbData, _pbData, m_iSize * sizeof(bool));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	bool_set	*/
	/*------------*/
	bool Bool::bool_set(int _iRow, int _iCol, bool _bData)
	{
		if(m_pbData != NULL)
		{
			if(_iRow <= m_iRows && _iCol <= m_iCols)
			{
				m_pbData[_iCol * m_iRows + _iRow] = _bData;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Bool::whoAmI() 
	{ 
		std::cout << "types::Bool"; 
	}

	/*--------------*/
	/*	getAsInt		*/
	/*--------------*/
	Bool* Bool::getAsBool(void)		
	{ 
		return this; 
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType Bool::getType(void)
	{
		return RealBool; 
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void Bool::all_delete()
	{
		if(m_pbData != NULL)
		{
			delete m_pbData;
			m_pbData = NULL;
		}
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool Bool::false_set()
	{
		if(m_pbData != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pbData[iIndex] = false;
			}
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	one_set	*/
	/*------------*/
	bool Bool::true_set()
	{
		if(m_pbData != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pbData[iIndex] = true;
			}
		}
		else
			return false;

		return true;	
	}

	string Bool::toString(int _iLineLen)
	{
		std::ostringstream ostr;
		/*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar

		}
		else if(cols_get() == 1)
		{//column vector
			ostr << "[" << std::endl;
			for(int i = 0 ; i < rows_get() ; i++)
			{
				ostr << (m_pbData[i] == true ? "true" : "false");
			}
			ostr << "\x09]" << std::endl;
		}
		else if(rows_get() == 1)
		{//row vector
			bool bWordWarp = false;
			int iLineTag = 5000; //or not Oo
			string szTemp;

			if(_iLineLen == -1)
			{
				bWordWarp = true;
			}

			ostr << "[ ";
			for(int i = 0 ; i < cols_get() ; i++)
			{
				if(i != 0)
				{
					szTemp += "\x09,";
				}

				if(bWordWarp == false && (int)(szTemp.size() + strlen(m_pbData[i] == true ? "true" : "false")) >= _iLineLen)
				{
					bWordWarp = true;
					iLineTag	= i;
				}

				if(bWordWarp == true && i%iLineTag == 0)
				{
					ostr << std::endl << "\t\tcolumn " << (i - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << i << std::endl;
					ostr << szTemp << std::endl;
					szTemp	= "";
				}

				szTemp += m_pbData[i] == true ? "true" : "false";
			}
			if(bWordWarp == true)
			{
				ostr << std::endl << "\t\tcolumn " << (cols_get() - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << cols_get() << std::endl;
			}
			ostr << szTemp;
			ostr << " ]" << std::endl;
		}
		else 
		{
			bool bWordWarp = false;
			int iLineTag = 5000; //or not Oo
			string szTemp;

			if(_iLineLen == -1)
			{
				bWordWarp = true;
			}

			ostr << "[ ";
			for(int i = 0 ; i < cols_get() ; i++)
			{
				if(i != 0)
				{
					szTemp += "\x09,";
				}

				if(bWordWarp == false && (int)(szTemp.size() + strlen(m_pbData[i * rows_get()] == true ? "true" : "false")) >= _iLineLen)
				{
					bWordWarp = true;
					iLineTag	= i;
					ostr << "elem by line : " << i << std::endl;
				}

				if(bWordWarp == true && i%iLineTag == 0)
				{
					ostr << std::endl << "\t\tcolumn " << (i - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << i << std::endl;
					ostr << szTemp << std::endl;
					for(int j = 1 ; j < rows_get() ; j++)
					{
						for(int k = (i - 1) / (iLineTag + 1) * iLineTag ; k <= i ; k++)
						{
							if(k != (i - 1) / (iLineTag + 1) * iLineTag)
							{
								ostr << "\t,";
							}
							ostr << (m_pbData[k * cols_get() + j] == true ? "true" : "false");
						}
						ostr << " ;" << std::endl;
					}
					szTemp	= "";
				}

				szTemp += m_pbData[i * rows_get()] == true ? "true" : "false";
			}
			if(bWordWarp == true)
			{
				ostr << std::endl << "\t\tcolumn " << (cols_get() - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << cols_get() << std::endl;
			}
			ostr << szTemp;
			ostr << " ]" << std::endl;
		}
		return ostr.str();
	}
}
