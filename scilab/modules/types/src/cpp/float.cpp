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
	Float::~Float()
	{
		if(isDeletable() == true)
		{
			deleteAll();
		}
	}

	Float::Float(int _iRows, int _iCols, bool _bComplex)
	{
		createFloat(_iRows, _iCols, NULL, NULL);
		m_bComplex = _bComplex;
		return;
	}

	Float::Float(float _fReal)
	{
		float *pfVal;
		createFloat(1, 1, &pfVal, NULL);
		pfVal[0] = _fReal;
		m_bComplex = false;
		return;
	}

	Float::Float(int _iRows, int _iCols, float **_pfReal)
	{
		createFloat(_iRows, _iCols, _pfReal, NULL);
		m_bComplex = false;
		return;
	}

	Float::Float(int _iRows, int _iCols, float **_pfReal, float **_pfImg)
	{
		createFloat(_iRows, _iCols, _pfReal, _pfImg);
		//		m_bComplex = false;
		return;
	}

	void Float::createFloat(int _iRows, int _iCols, float **_pfReal, float **_pfImg)
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

	InternalType* Float::clone()
	{
		// FIXME : Implement me.
		return NULL;
	}

	bool Float::isComplex()
	{
		return m_bComplex;
	}

	float*	Float::getReal() const
	{
		return m_pfReal;
	}

	float*	Float::getImg() const
	{
		return m_pfImg;
	}

	bool Float::set(float *_pfReal)
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

	float	Float::getReal(int _iRows, int _iCols) const
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

	bool Float::setImg(float *_pfImg)
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

	float	Float::getImg(int _iRows, int _iCols) const
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

	void Float::whoAmI()
	{
		std::cout << "types::Float";
	}

	GenericType::RealType Float::getType(void)
	{
		return RealFloat;
	}

	void Float::deleteReal()
	{
		if(m_pfReal != NULL)
		{
			delete[] m_pfReal;
			m_pfReal = NULL;
		}
	}

	void Float::deleteImg(bool _bSetReal)
	{
		if(m_pfImg != NULL)
		{
			delete[] m_pfImg;
			m_pfImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	void Float::deleteAll(bool _bSetReal)
	{
		deleteReal();
		deleteImg(_bSetReal);
	}

	bool Float::setZeros()
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

	bool Float::setOnes()
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
		if(const_cast<InternalType &>(it).isFloat() == false)
		{
			return false;
		}

		Float* pf = const_cast<InternalType &>(it).getAs<Float>();

		if(pf->getRows() != getRows() || pf->getCols() != getCols())
		{
			return false;
		}

		if(pf->isComplex() != isComplex())
		{
			return false;
		}

		float *pfReal = pf->getReal();
		if(memcmp(m_pfReal, pfReal, getSize() * sizeof(float)) != 0)
		{
			return false;
		}

		if(isComplex())
		{
			float *pfImg = pf->getImg();
			if(memcmp(m_pfImg, pfImg, getSize() * sizeof(float)) != 0)
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

	GenericType*	Float::getColumnValues(int _iPos)
	{
		Float *pf = NULL;
		if(_iPos < m_iCols)
		{
			if(isComplex())
			{
				pf = new Float(m_iRows, 1, true);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pf->setValue(i, 0, getReal(i, _iPos), getImg(i, _iPos));
				}
			}
			else
			{
				pf = new Float(m_iRows, 1);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pf->setValue(i, 0, getReal(i, _iPos));
				}
			}
		}
		return pf;
	}

	bool Float::setValue(int _iRows, int _iCols, float _fReal)
	{
		return setValue(_iRows, _iCols, _fReal, 0);
	}

	bool Float::setValue(int _iRows, int _iCols, float _fReal, float _fImg)
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

    bool Float::toString(std::wostringstream& ostr)
	{
		// FIXME : Implement display method.
		ostr << L"Float Values" << std::endl;

		return true;
	}

}

