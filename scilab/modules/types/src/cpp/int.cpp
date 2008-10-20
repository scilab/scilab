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
	/*--------*/
	/*	~Int	*/
	/*--------*/
	Int::~Int()
	{
		all_delete();
	}

	/*--------------------*/
	/*				Int				*/
	/*	Empty constructor	*/
	/*--------------------*/
	Int::Int(int _iRows, int _iCols, bool _bComplex)
	{
		CreateInt(_iRows, _iCols, NULL, NULL);
		m_bComplex = _bComplex;
		return;
	}

	/*--------------------*/
	/*				Int				*/
	/*	Real constructor	*/
	/*--------------------*/
	Int::Int(int _iReal)
	{
		int *piVal;
		CreateInt(1, 1, &piVal, NULL);
		piVal[0] = _iReal;
		m_bComplex = false;
		return;
	}

	/*--------------------*/
	/*				Int				*/
	/*	Real constructor	*/
	/*--------------------*/
	Int::Int(int _iRows, int _iCols, int **_piReal)
	{
		CreateInt(_iRows, _iCols, _piReal, NULL);
		m_bComplex = false;
		return;
	}
	
	/*----------------------*/
	/*					Int				*/
	/*	Complex constructor	*/
	/*----------------------*/
	Int::Int(int _iRows, int _iCols, int **_piReal, int **_piImg)
	{
		CreateInt(_iRows, _iCols, _piReal, _piImg);
		//m_bComplex = false;
		return;
	}

	/*----------------------*/
	/*			CreateInt				*/
	/*	Commun constructor	*/
	/*----------------------*/
	void Int::CreateInt(int _iRows, int _iCols, int **_piReal, int **_piImg)
	{
		m_iCols	= _iCols;
		m_iRows	= _iRows;
		m_iSize = m_iCols * m_iRows;

		if(_piReal != NULL)
		{
			/*alloc Real array*/
			m_piReal = new int[m_iSize];
		
			/*return it*/
			*_piReal = m_piReal;
		}
		else
			m_piReal = NULL;

		if(_piImg != NULL)
		{
			/*alloc Img array*/
			m_piImg = new int[m_iSize];

			/*return it*/
			*_piImg = m_piImg;

			/*set complex flag*/
			m_bComplex = true;
		}
		else
			m_piImg = NULL;
	}

	bool Int::isComplex()
	{
		return m_bComplex;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	int*	Int::real_get() const
	{ 
		return m_piReal;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	int	Int::real_get(int _iRows, int _iCols) const
	{ 
		if(m_piReal != NULL)
		{
			return m_piReal[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	int*	Int::img_get() const
	{ 
		return m_piImg;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	int	Int::img_get(int _iRows, int _iCols) const
	{ 
		if(m_piImg != NULL)
		{
			return m_piImg[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	real_set	*/
	/*------------*/
	bool Int::real_set(int *_piReal)
	{
		if(_piReal != NULL)
		{
			if(m_piReal == NULL)
			{
				m_piReal = new int[m_iSize];
			}

			memcpy(m_piReal, _piReal, m_iSize * sizeof(int));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	img_set		*/
	/*------------*/
	bool Int::img_set(int *_piImg)
	{
		if(_piImg != NULL)
		{
			if(m_piImg == NULL)
			{
				m_piImg = new int[m_iSize];
			}

			memcpy(m_piImg, _piImg, m_iSize * sizeof(int));
		}
		else
			return false;

		return true;
	}

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void Int::whoAmI() 
	{ 
		std::cout << "types::Int" << std::endl; 
	}

	/*--------------*/
	/*	getAsInt		*/
	/*--------------*/
	Int* Int::getAsInt(void)		
	{ 
		return this; 
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType Int::getType(void)
	{
		return RealInt; 
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void Int::real_delete()
	{
		if(m_piReal != NULL)
		{
			delete m_piReal;
			m_piReal = NULL;
		}
	}

	/*------------*/
	/*	img_clean	*/
	/*------------*/
	void Int::img_delete(bool _bSetReal)
	{
		if(m_piImg != NULL)
		{
			delete m_piImg;
			m_piImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	/*------------*/
	/*	all_clean	*/
	/*------------*/
	void Int::all_delete(bool _bSetReal)
	{
		real_delete();
		img_delete(_bSetReal);
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool Int::zero_set()
	{
		if(m_piReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_piReal[iIndex] = 0;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_piImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_piImg[iIndex] = 0;
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
	bool Int::one_set()
	{
		if(m_piReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_piReal[iIndex] = 1;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_piImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_piImg[iIndex] = 1;
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