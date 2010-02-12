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
#include "float.hxx"

namespace types
{
	/*------------*/
	/*	~Float		*/
	/*------------*/
	Float::~Float()
	{
		if(isDeletable() == true)
		{
			all_delete();
		}
	}

	/*--------------------*/
	/*				Float			*/
	/*	Empty constructor	*/
	/*--------------------*/
	Float::Float(int _iRows, int _iCols, bool _bComplex)
	{
		CreateFloat(_iRows, _iCols, NULL, NULL);
		m_bComplex = _bComplex;
		return;
	}

	/*--------------------*/
	/*				Float			*/
	/*	Real constructor	*/
	/*--------------------*/
	Float::Float(float _fReal)
	{
		float *pfVal;
		CreateFloat(1, 1, &pfVal, NULL);
		pfVal[0] = _fReal;
		m_bComplex = false;
		return;
	}

	/*--------------------*/
	/*		 	 Float				*/
	/*	Real constructor	*/
	/*--------------------*/
	Float::Float(int _iRows, int _iCols, float **_pfReal)
	{
		CreateFloat(_iRows, _iCols, _pfReal, NULL);
		m_bComplex = false;
		return;
	}

	/*----------------------*/
	/*				Float				*/
	/*	Complex constructor	*/
	/*----------------------*/
	Float::Float(int _iRows, int _iCols, float **_pfReal, float **_pfImg)
	{
		CreateFloat(_iRows, _iCols, _pfReal, _pfImg);
		//		m_bComplex = false;
		return;
	}

	/*----------------------*/
	/*			CreateFloat		*/
	/*	Commun constructor	*/
	/*----------------------*/
	void Float::CreateFloat(int _iRows, int _iCols, float **_pfReal, float **_pfImg)
	{
		m_iCols	= _iCols;
		m_iRows	= _iRows;
		m_iSize = m_iCols * m_iRows;

		if(_pfReal != NULL)
		{
			/*alloc Real array*/
			m_pfReal = new float[m_iSize];

			/*return it*/
			*_pfReal = m_pfReal;
		}
		else
			m_pfReal = NULL;

		if(_pfImg != NULL)
		{
			/*alloc Img array*/
			m_pfImg = new float[m_iSize];

			/*return it*/
			*_pfImg = m_pfImg;

			/*set complex flag*/
			m_bComplex = true;
		}
		else
			m_pfImg = NULL;
	}

	Float* Float::clone()
	{
		// FIXME : Implement me.
		return NULL;
	}

	bool Float::isComplex()
	{
		return m_bComplex;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	float*	Float::real_get() const
	{ 
		return m_pfReal;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	float*	Float::img_get() const
	{ 
		return m_pfImg;
	}

	/*------------*/
	/*	real_set	*/
	/*------------*/
	bool Float::real_set(float *_pfReal)
	{
		if(_pfReal != NULL)
		{
			if(m_pfReal == NULL)
			{
				m_pfReal = new float[m_iSize];
			}

			memcpy(m_pfReal, _pfReal, m_iSize * sizeof(float));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	float	Float::real_get(int _iRows, int _iCols) const
	{ 
		if(m_pfReal != NULL)
		{
			return m_pfReal[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	img_set		*/
	/*------------*/
	bool Float::img_set(float *_pfImg)
	{
		if(_pfImg != NULL)
		{
			if(m_pfImg == NULL)
			{
				m_pfImg = new float[m_iSize];
			}

			memcpy(m_pfImg, _pfImg, m_iSize * sizeof(float));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	float	Float::img_get(int _iRows, int _iCols) const
	{ 
		if(m_pfImg != NULL)
		{
			return m_pfImg[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Float::whoAmI() 
	{ 
		std::cout << "types::Float"; 
	}

	/*--------------*/
	/*	getAsUInt		*/
	/*--------------*/
	Float* Float::getAsFloat(void)		
	{ 
		return this; 
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType Float::getType(void)
	{
		return RealFloat; 
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void Float::real_delete()
	{
		if(m_pfReal != NULL)
		{
			delete[] m_pfReal;
			m_pfReal = NULL;
		}
	}

	/*------------*/
	/*	img_clean	*/
	/*------------*/
	void Float::img_delete(bool _bSetReal)
	{
		if(m_pfImg != NULL)
		{
			delete[] m_pfImg;
			m_pfImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	/*------------*/
	/*	all_clean	*/
	/*------------*/
	void Float::all_delete(bool _bSetReal)
	{
		real_delete();
		img_delete(_bSetReal);
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool Float::zero_set()
	{
		if(m_pfReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pfReal[iIndex] = 0;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pfImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_pfImg[iIndex] = 0;
				}
			}
			else
				return false;
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	one_set	*/
	/*------------*/
	bool Float::one_set()
	{
		if(m_pfReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pfReal[iIndex] = 1;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pfImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_pfImg[iIndex] = 1;
				}
			}
			else
				return false;
		}
		else
			return false;

		return true;	
	}

	bool Float::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealFloat)
		{
			return false;
		}

		Float* pf = pIT->getAsFloat();

		if(pf->rows_get() != rows_get() || pf->cols_get() != cols_get())
		{
			return false;
		}

		if(pf->isComplex() != isComplex())
		{
			return false;
		}

		float *pfReal = pf->real_get();
		if(memcmp(m_pfReal, pfReal, size_get() * sizeof(float)) != 0)
		{
			return false;
		}

		if(isComplex())
		{
			float *pfImg = pf->img_get();
			if(memcmp(m_pfImg, pfImg, size_get() * sizeof(float)) != 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Float::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

	GenericType*	Float::get_col_value(int _iPos)
	{
		Float *pf = NULL;
		if(_iPos < m_iCols)
		{
			if(isComplex())
			{
				pf = new Float(m_iRows, 1, true);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pf->val_set(i, 0, real_get(i, _iPos), img_get(i, _iPos));
				}
			}
			else
			{
				pf = new Float(m_iRows, 1);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pf->val_set(i, 0, real_get(i, _iPos));
				}
			}
		}
		return pf;
	}

	bool Float::val_set(int _iRows, int _iCols, float _fReal)
	{
		return val_set(_iRows, _iCols, _fReal, 0);
	}

	bool Float::val_set(int _iRows, int _iCols, float _fReal, float _fImg)
	{
		if(m_pfReal != NULL)
		{
			if(_iRows < m_iRows && _iCols < m_iCols)
			{
				m_pfReal[_iCols * m_iRows + _iRows] = _fReal;
				if(m_bComplex)
				{
					m_pfImg[_iCols * m_iRows + _iRows] = _fImg;
				}
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

	std::string Float::toString(int _iPrecision, int _iLineLen) 
	{
		std::ostringstream ostr;
		// FIXME : Implement display method.
		ostr << "Float Values" << std::endl;

		return ostr.str();
	}

}

