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
#include "core_math.h"

namespace types
{
	/*--------*/
	/*	~Bool	*/
	/*--------*/
	Bool::~Bool()
	{
		if(isDeletable() == true)
		{
			all_delete();
		}
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

	Bool::Bool *Bool::clone()
	{
	  Bool::Bool *pbClone =  new Bool::Bool(rows_get(), cols_get());
	  pbClone->bool_set(m_pbData);

	  return pbClone;
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
	bool	Bool::bool_get(int _iRows, int _iCols) const
	{ 
		if(m_pbData != NULL)
		{
			return m_pbData[_iCols * m_iRows + _iRows];
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
	bool Bool::bool_set(int _iRows, int _iCols, bool _bData)
	{
		if(m_pbData != NULL)
		{
			if(_iRows <= m_iRows && _iCols <= m_iCols)
			{
				m_pbData[_iCols * m_iRows + _iRows] = _bData;
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
			delete[] m_pbData;
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

	string Bool::toString(int _iPrecision, int _iLineLen)
	{
		std::ostringstream ostr;
		ostr << std::endl;
		/*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
		if(cols_get() == 1 && rows_get() == 1)
		{//scalar
				ostr << (m_pbData[0] == true ? "T" : "F");
				ostr << std::endl;
		}
		else if(cols_get() == 1)
		{//column vector
			for(int i = 0 ; i < rows_get() ; i++)
			{
				ostr << (m_pbData[i] == true ? "T" : "F");
				ostr << std::endl;
			}
			ostr << std::endl;
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

			for(int i = 0 ; i < cols_get() ; i++)
			{
				if(i != 0)
				{
					szTemp += "  ";
				}

				if(bWordWarp == false && (int)(szTemp.size() + strlen(m_pbData[i] == true ? "T" : "F")) >= _iLineLen)
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

				szTemp += m_pbData[i] == true ? "T" : "F";
			}
			if(bWordWarp == true)
			{
				ostr << std::endl << "\t\tcolumn " << (cols_get() - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << cols_get() << std::endl;
			}
			ostr << szTemp;
			ostr << std::endl;
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

			for(int i = 0 ; i < cols_get() ; i++)
			{
				if(i != 0)
				{
					szTemp += "  ";
				}

				if(bWordWarp == false && (int)(szTemp.size() + strlen(m_pbData[i * rows_get()] == true ? "T" : "F")) >= _iLineLen)
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
							ostr << (m_pbData[k * cols_get() + j] == true ? "T" : "F");
						}
						ostr << " ;" << std::endl;
					}
					szTemp	= "";
				}

				szTemp += m_pbData[i * rows_get()] == true ? "T" : "F";
			}
			if(bWordWarp == true)
			{
				ostr << std::endl << "\t\tcolumn " << (cols_get() - 1) / (iLineTag + 1) * iLineTag + 1 << " to " << cols_get() << std::endl;
			}
			ostr << szTemp;
			ostr << std::endl;
		}
		ostr << std::endl;
		return ostr.str();
	}

	bool Bool::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealBool)
		{
			return false;
		}

		Bool* pb = pIT->getAsBool();

		if(pb->rows_get() != rows_get() || pb->cols_get() != cols_get())
		{
			return false;
		}

		bool* pbool = pb->bool_get();

		if(memcmp(m_pbData, pbool, size_get() * sizeof(bool)) != 0)
		{
			return false;
		}
		return true;
	}

	bool Bool::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

	bool Bool::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		//alloc new data array
		bool* pb = NULL;

		pb = new bool[_iNewRows * _iNewCols];
		memset(pb, 0x00, sizeof(bool) * _iNewRows * _iNewCols);

		for(int i = 0 ; i < rows_get() ; i++)
		{
			for(int j = 0 ; j < cols_get() ; j++)
			{
				pb[j * _iNewRows + i] = m_pbData[j * rows_get() + i];
			}
		}
		delete[] m_pbData;
		m_pbData	= pb;

		m_iRows = _iNewRows;
		m_iCols	= _iNewCols;
		m_iSize = m_iRows * m_iCols;
		//copy existing values
		return true;
	}

	bool Bool::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
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
				Bool *pIn = _poSource->getAsBool();

				//Only resize after all tests !
				if(resize(iNewRows, iNewCols) == false)
				{
					return false;
				}

				bool* pbIn = pIn->bool_get();

				//variable can receive new values.
				if(pIn->size_get() == 1)
				{//a(?) = x
					if(_bAsVector)
					{//a([]) = R
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							m_pbData[_piSeqCoord[i] - 1]	= pbIn[0];
						}
					}
					else
					{//a([],[]) = R
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
							m_pbData[iPos]	= pbIn[0];
						}
					}
				}
				else
				{//a(?) = [x]
					if(_bAsVector)
					{//a([]) = [R]
						for(int i = 0 ; i < _iSeqCount ; i++)
						{
							m_pbData[_piSeqCoord[i] - 1]	= pbIn[i];
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

							m_pbData[iPos]	= pbIn[iNew_i];
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

	Bool* Bool::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Bool* _poSource, bool _bAsVector)
	{
		Bool* pb	= NULL ; 
		
		if(_bAsVector)
		{
			if(_poSource->cols_get() == 1)
			{
				pb = new Bool(_piMaxDim[0], 1);
			}
			else if(_poSource->rows_get() == 1)
			{
				pb = new Bool(1, _piMaxDim[0]);
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			pb = new Bool(_piMaxDim[0], _piMaxDim[1]);
		}

		pb->false_set();
		if(pb->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
		{
			delete pb;
			return NULL;
		}

		return pb;
	}

	Bool* Bool::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
	{
		Bool* pOut		= NULL;
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
			iRowsOut		= _piDimSize[0];
			iColsOut		= _piDimSize[1];
		}

		pOut					= new Bool(iRowsOut, iColsOut);
		bool* pb			= pOut->bool_get();


		if(_bAsVector)
		{
			for(int i = 0 ; i < _iSeqCount ; i++)
			{
				pb[i]			= m_pbData[_piSeqCoord[i] - 1];
			}
		}
		else
		{
			int iRowIn	= rows_get();
			for(int i = 0 ; i < _iSeqCount ; i++)
			{
				//convert vertical indexes to horizontal indexes
				int iCurIndex		= (i % iColsOut) * iRowsOut + (i / iColsOut);
				int iInIndex		= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
				pb[iCurIndex]		= m_pbData[iInIndex];
			}
		}
		return pOut;
	}
}
