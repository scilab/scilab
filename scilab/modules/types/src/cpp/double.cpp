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

#include "double.hxx"

namespace types
{
	/*------------*/
	/*	~Double		*/
	/*------------*/
	Double::~Double()
	{
		all_delete();
	}

	/*--------------------*/
	/*				Double			*/
	/*	Empty constructor	*/
	/*--------------------*/
	Double::Double(int _iRows, int _iCols, bool _bComplex)
	{
		double *pReal = NULL;
		double *pImg	= NULL;
		if(_bComplex == false)
		{
			CreateDouble(_iRows, _iCols, &pReal, NULL);
		}
		else
		{
			CreateDouble(_iRows, _iCols, &pReal, &pImg);
		}
		m_bComplex = _bComplex;
		return;
	}

	/*--------------------*/
	/*				Double			*/
	/*	Real constructor	*/
	/*--------------------*/
	Double::Double(double _dblReal)
	{
		double *pdblVal;
		CreateDouble(1, 1, &pdblVal, NULL);
		pdblVal[0] = _dblReal;
		m_bComplex = false;
		return;
	}

	/*--------------------*/
	/*		 	 Double				*/
	/*	Real constructor	*/
	/*--------------------*/
	Double::Double(int _iRows, int _iCols, double **_pdblReal)
	{
		CreateDouble(_iRows, _iCols, _pdblReal, NULL);
		m_bComplex = false;
		return;
	}
	
	/*----------------------*/
	/*				Double				*/
	/*	Complex constructor	*/
	/*----------------------*/
	Double::Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg)
	{
		CreateDouble(_iRows, _iCols, _pdblReal, _pdblImg);
//		m_bComplex = false;
		return;
	}

	/*----------------------*/
	/*			CreateDouble		*/
	/*	Commun constructor	*/
	/*----------------------*/
	void Double::CreateDouble(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg)
	{
		m_iCols	= _iCols;
		m_iRows	= _iRows;
		m_iSize = m_iCols * m_iRows;

		if(_pdblReal != NULL)
		{
			/*alloc Real array*/
			m_pdblReal = new double[m_iSize];
		
			/*return it*/
			*_pdblReal = m_pdblReal;
		}
		else
			m_pdblReal = NULL;

		if(_pdblImg != NULL)
		{
			/*alloc Img array*/
			m_pdblImg = new double[m_iSize];

			/*return it*/
			*_pdblImg = m_pdblImg;

			/*set complex flag*/
			m_bComplex = true;
		}
		else
			m_pdblImg = NULL;
	}

	bool Double::isComplex()
	{
		return m_bComplex;
	}

	void Double::complex_set(bool _bComplex)
	{
		if(_bComplex == false)
		{
			if(isComplex() == true)
			{
				img_delete(!_bComplex);
			}
		}
		else // _bComplex == true
		{
			if(isComplex() == false)
			{
				m_pdblImg = new double[m_iSize];
				memset(m_pdblImg, 0x00, sizeof(double) * m_iSize);
			}
		}
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	double*	Double::real_get() const
	{ 
		return m_pdblReal;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	double*	Double::img_get() const
	{ 
		return m_pdblImg;
	}

	/*------------*/
	/*	real_set	*/
	/*------------*/
	bool Double::real_set(double *_pdblReal)
	{
		if(_pdblReal != NULL)
		{
			if(m_pdblReal == NULL)
			{
				m_pdblReal = new double[m_iSize];
			}

			memcpy(m_pdblReal, _pdblReal, m_iSize * sizeof(double));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	double	Double::real_get(int _iRows, int _iCols) const
	{ 
		if(m_pdblReal != NULL)
		{
			return m_pdblReal[_iCols * m_iRows + _iRows];
		}
		else
		{
			return 0;
		}
	}

	/*------------*/
	/*	img_set		*/
	/*------------*/
	bool Double::img_set(double *_pdblImg)
	{
		if(_pdblImg != NULL)
		{
			if(m_pdblImg == NULL)
			{
				m_pdblImg = new double[m_iSize];
			}

			memcpy(m_pdblImg, _pdblImg, m_iSize * sizeof(double));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	double	Double::img_get(int _iRows, int _iCols) const
	{ 
		if(m_pdblImg != NULL)
		{
			return m_pdblImg[_iCols * m_iRows + _iRows];
		}
		else
		{
			return 0;
		}
	}

	/*--------------*/
	/*		val_set		*/
	/*--------------*/
	bool Double::val_set(int _iRow, int _iCol, double _dblVal)
	{
		return val_set(_iRow, _iCol, _dblVal, 0);
	}

	/*--------------*/
	/*		val_set		*/
	/*--------------*/
	bool Double::val_set(int _iRow, int _iCol, double _dblReal, double _dblImg)
	{
		if(m_pdblReal != NULL)
		{
			if(_iRow < m_iRows && _iCol < m_iCols)
			{
				m_pdblReal[_iCol * m_iRows + _iRow] = _dblReal;
				if(m_bComplex)
				{
					m_pdblImg[_iCol * m_iRows + _iRow] = _dblImg;
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

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Double::whoAmI() 
	{ 
		std::cout << "types::Double" << std::endl; 
	}

	/*--------------*/
	/*	getAsUInt		*/
	/*--------------*/
	Double* Double::getAsDouble(void)		
	{ 
		return this; 
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType Double::getType(void)
	{
		return RealDouble; 
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void Double::real_delete()
	{
		if(m_pdblReal != NULL)
		{
			delete m_pdblReal;
			m_pdblReal = NULL;
		}
	}

	/*------------*/
	/*	img_clean	*/
	/*------------*/
	void Double::img_delete(bool _bSetReal)
	{
		if(m_pdblImg != NULL)
		{
			delete m_pdblImg;
			m_pdblImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	/*------------*/
	/*	all_clean	*/
	/*------------*/
	void Double::all_delete(bool _bSetReal)
	{
		real_delete();
		img_delete(_bSetReal);
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool Double::zero_set()
	{
		if(m_pdblReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pdblReal[iIndex] = 0;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pdblImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_pdblImg[iIndex] = 0;
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
	bool Double::one_set()
	{
		if(m_pdblReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_pdblReal[iIndex] = 1;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pdblImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_pdblImg[iIndex] = 1;
				}
			}
			else
				return false;
		}
		else
			return false;

		return true;	
	}
}

