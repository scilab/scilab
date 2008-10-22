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

#include "uint.hxx"

namespace types
{
	/*--------*/
	/*	~UInt	*/
	/*--------*/
	UInt::~UInt()
	{
		all_delete();
	}

	/*--------------------*/
	/*				UInt				*/
	/*	Empty constructor	*/
	/*--------------------*/
	UInt::UInt(int _iRows, int _iCols, bool _bComplex)
	{
		CreateUInt(_iRows, _iCols, NULL, NULL);
		m_bComplex = _bComplex;
		return;
	}

	/*--------------------*/
	/*				UInt				*/
	/*	Real constructor	*/
	/*--------------------*/
	UInt::UInt(unsigned int _iReal)
	{
		unsigned int *puiVal;
		CreateUInt(1, 1, &puiVal, NULL);
		puiVal[0] = _iReal;
		m_bComplex = false;
		return;
	}

	/*--------------------*/
	/*				UInt				*/
	/*	Real constructor	*/
	/*--------------------*/
	UInt::UInt(int _iRows, int _iCols, int unsigned **_puiReal)
	{
		CreateUInt(_iRows, _iCols, _puiReal, NULL);
		m_bComplex = false;
		return;
	}
	
	/*----------------------*/
	/*					UInt				*/
	/*	Complex constructor	*/
	/*----------------------*/
	UInt::UInt(int _iRows, int _iCols, int unsigned **_puiReal, int unsigned **_puiImg)
	{
		CreateUInt(_iRows, _iCols, _puiReal, _puiImg);
		//m_bComplex = false;
		return;
	}

	/*----------------------*/
	/*			CreateUInt			*/
	/*	Commun constructor	*/
	/*----------------------*/
	void UInt::CreateUInt(int _iRows, int _iCols, int unsigned **_puiReal, int unsigned **_puiImg)
	{
		m_iCols	= _iCols;
		m_iRows	= _iRows;
		m_iSize = m_iCols * m_iRows;

		if(_puiReal != NULL)
		{
			/*alloc Real array*/
			m_puiReal = new unsigned int[m_iSize];
		
			/*return it*/
			*_puiReal = m_puiReal;
		}
		else
			m_puiReal = NULL;

		if(_puiImg != NULL)
		{
			/*alloc Img array*/
			m_puiImg = new unsigned int[m_iSize];

			/*return it*/
			*_puiImg = m_puiImg;

			/*set complex flag*/
			m_bComplex = true;
		}
		else
			m_puiImg = NULL;
	}

	bool UInt::isComplex()
	{
		return m_bComplex;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	int unsigned*	UInt::real_get() const
	{ 
		return m_puiReal;
	}

	/*------------*/
	/*	real_get	*/
	/*------------*/
	int unsigned UInt::real_get(int _iRows, int _iCols) const
	{ 
		if(m_puiReal != NULL)
		{
			return m_puiReal[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	int unsigned*	UInt::img_get() const
	{ 
		return m_puiImg;
	}

	/*------------*/
	/*	img_get	*/
	/*------------*/
	int unsigned	UInt::img_get(int _iRows, int _iCols) const
	{ 
		if(m_puiImg != NULL)
		{
			return m_puiImg[_iCols * m_iRows + _iRows];
		}
		else
		{
			return NULL;
		}
	}

	/*------------*/
	/*	real_set	*/
	/*------------*/
	bool UInt::real_set(int *_puiReal)
	{
		if(_puiReal != NULL)
		{
			if(m_puiReal == NULL)
			{
				m_puiReal = new unsigned int[m_iSize];
			}

			memcpy(m_puiReal, _puiReal, m_iSize * sizeof(int));
		}
		else
			return false;

		return true;
	}

	/*------------*/
	/*	img_set		*/
	/*------------*/
	bool UInt::img_set(int *_puiImg)
	{
		if(_puiImg != NULL)
		{
			if(m_puiImg == NULL)
			{
				m_puiImg = new unsigned int[m_iSize];
			}

			memcpy(m_puiImg, _puiImg, m_iSize * sizeof(int));
		}
		else
			return false;

		return true;
	}

	/*--------------*/
	/*		whoIAm		*/
	/*--------------*/
	void UInt::whoAmI() 
	{ 
		std::cout << "types::UInt" << std::endl; 
	}

	/*--------------*/
	/*	getAsUInt		*/
	/*--------------*/
	UInt* UInt::getAsUInt(void)		
	{ 
		return this; 
	}

	/*------------*/
	/*	getType		*/
	/*------------*/
	GenericType::RealType UInt::getType(void)
	{
		return RealUInt; 
	}

	/*--------------*/
	/*	real_clean	*/
	/*--------------*/
	void UInt::real_delete()
	{
		if(m_puiReal != NULL)
		{
			delete m_puiReal;
			m_puiReal = NULL;
		}
	}

	/*------------*/
	/*	img_clean	*/
	/*------------*/
	void UInt::img_delete(bool _bSetReal)
	{
		if(m_puiImg != NULL)
		{
			delete m_puiImg;
			m_puiImg = NULL;
			m_bComplex = !_bSetReal;
		}
	}

	/*------------*/
	/*	all_clean	*/
	/*------------*/
	void UInt::all_delete(bool _bSetReal)
	{
		real_delete();
		img_delete(_bSetReal);
	}

	/*------------*/
	/*	zero_set	*/
	/*------------*/
	bool UInt::zero_set()
	{
		if(m_puiReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_puiReal[iIndex] = 0;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_puiImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_puiImg[iIndex] = 0;
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
	/*	one_set		*/
	/*------------*/
	bool UInt::one_set()
	{
		if(m_puiReal != NULL)
		{
			for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
			{
				m_puiReal[iIndex] = 1;
			}
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_puiImg != NULL)
			{
				for(int iIndex = 0 ; iIndex < m_iSize ; iIndex++)
				{
					m_puiImg[iIndex] = 1;
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

