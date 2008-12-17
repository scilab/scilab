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
#include <math.h>
#include "poly.hxx"

extern "C"
{
#include "log.h"
#include "exp.h"
}
namespace types
{
	Poly::Poly()
	{
	}

	Poly::Poly(double** _pdblCoefR, int _iRank)
	{
		CreatePoly(_pdblCoefR, NULL, _iRank);
	}

	Poly::Poly(double** _pdblCoefR, double** _pdblCoefI, int _iRank)
	{
		CreatePoly(_pdblCoefR, _pdblCoefI, _iRank);
	}

	Poly::Poly(Double** _poCoefR, int _iRank)
	{
		double *pR	= NULL;
		double *pI	= NULL;
		CreatePoly(&pR, &pI, _iRank);
		*_poCoefR = m_pdblCoef;
	}


	Poly::~Poly()
	{
		delete m_pdblCoef;
	}

	/*Real constructor, private only*/
	void Poly::CreatePoly(double** _pdblCoefR, double** _pdblCoefI, int _iRank)
	{
		double *pR	= NULL;
		double *pI	= NULL;
		m_bComplex	= false;
		m_iRank			= _iRank;

		if(m_iRank == 0)
		{
			return;
		}
	
		if(_pdblCoefI != NULL)
		{
			m_pdblCoef = new Double(1, _iRank, &pR, &pI);
		}
		else
		{
			m_pdblCoef = new Double(1, _iRank, &pR);
		}

		if(_pdblCoefR != NULL)
		{
			*_pdblCoefR = pR;
		}

		if(_pdblCoefI != NULL)
		{
			m_bComplex = true;
			*_pdblCoefI = pI;
		}
	}

	int Poly::rank_get()
	{
		return m_iRank;
	}

	bool Poly::rank_set(int _iRank, bool bSave)
	{
		double *pR	= NULL;
		double *pI	= NULL;
		if(bSave == false)
		{
			if(m_iRank != _iRank)
			{
				delete m_pdblCoef;

				CreatePoly(&pR, &pI, _iRank);
				return true;
			}
			return true;
		}
		else
		{
			Double *pCoef = NULL;
			if(_iRank != 0)
			{
				pCoef = new Double(1, _iRank, &pR, &pI);
				pCoef->real_set(m_pdblCoef->real_get());
				complex_set(m_pdblCoef->isComplex());
				if(m_pdblCoef->isComplex())
				{
					pCoef->img_set(m_pdblCoef->img_get());
				}
				m_iRank = _iRank;
			}
			else
			{
				m_iRank = 1;
				pCoef = new Double(1, 1, &pR);
				pCoef->val_set(0,0,0);
			}
			delete m_pdblCoef;
			m_pdblCoef = pCoef;
			return true;
		}
		return false;
	}

	Double* Poly::coef_get()
	{
		return m_pdblCoef;
	}

	double* Poly::coef_real_get()
	{
		return m_pdblCoef->real_get();
	}

	double* Poly::coef_img_get()
	{
		return m_pdblCoef->img_get();
	}

	bool	Poly::coef_set(Double* _pdblCoefR)
	{
		if(m_pdblCoef == NULL || _pdblCoefR == NULL)
		{
			return false;
		}

		double *pInR	= _pdblCoefR->real_get();
		double *pInI	= _pdblCoefR->img_get();

		return coef_set(pInR, pInI);
	}

	void Poly::complex_set(bool _bComplex)
	{
		m_pdblCoef->complex_set(_bComplex);
		m_bComplex = _bComplex;
	}

	bool Poly::coef_set(double* _pdblCoefR, double* _pdblCoefI)
	{
		if(m_pdblCoef == NULL)
		{
			return false;
		}

		if(_pdblCoefI != NULL && isComplex() == false)
		{
			m_pdblCoef->complex_set(true);
			m_bComplex = true;
		}

		double *pR = m_pdblCoef->real_get();
		double *pI = m_pdblCoef->img_get();

		if(_pdblCoefR != NULL && pR != NULL)
		{
			memcpy(pR, _pdblCoefR, m_iRank * sizeof(double));
		}
		
		if(_pdblCoefI != NULL && pI != NULL)
		{
			memcpy(pI, _pdblCoefI, m_iRank * sizeof(double));
		}

		return true;
	}

	void Poly::whoAmI()
	{
		std::cout << "types::Poly"; 
	}

	bool Poly::isComplex()
	{
		return m_bComplex;
	}

	GenericType::RealType Poly::getType(void)
	{
		return RealPoly;
	}

	bool Poly::evaluate(double _dblInR, double _dblInI, double *_pdblOutR, double *_pdblOutI)
	{
		double *pCoefR = m_pdblCoef->real_get();
		double *pCoefI = m_pdblCoef->img_get();

		*_pdblOutR = 0;
		*_pdblOutI = 0;

		for(int i = m_iRank - 1 ; i >= 0 ; i--)
		{
			//(a1 + ib1)(a2 + ib2)**n = (a1 + ib1) * exp(n * log(a2 + ib2))
			double dblLogR = 0;
			double dblLogI = 0;
			double dblExpR = 0;
			double dblExpI = 0;

			//log(a2 + ib2)
			if(_dblInI != 0)
			{
				wlog(_dblInR, _dblInI, &dblLogR, &dblLogI);
			}
			else
			{
				dblLogR = dlogs(_dblInR);
			}

			//n * log(a2 + ib2)
			dblLogR *= i;
			dblLogI *= i;

			//exp(n * log(a2 + ib2))
			if(dblLogI != 0)
			{
				zexps(dblLogR, dblLogI, &dblExpR, &dblExpI);
			}
			else
			{
				dblExpR = dexps(dblLogR);
			}

			//(a1 + ib1) * exp(n * log(a2 + ib2))
			if(m_pdblCoef->isComplex())
			{
				*_pdblOutR += (dblExpR * pCoefR[i] - dblExpI * pCoefI[i]);
				*_pdblOutI += (dblExpR * pCoefI[i] + dblExpI * pCoefR[i]);
			}
			else
			{
				*_pdblOutR += (dblExpR * pCoefR[i]);
				*_pdblOutI += (dblExpI * pCoefR[i]);
			}
		}
		return true;
	}

	void Poly::update_rank(void)
	{
		int iNewRank = m_iRank;
		double *pCoefR = coef_get()->real_get();
		double *pCoefI = coef_get()->img_get();
		for(int i = m_iRank - 1; i >= 0 ; i--)
		{
			if(pCoefR[i] == 0 && (pCoefI != NULL ? pCoefI[i] : 0) == 0)
			{
				iNewRank--;
			}
			else
				break;
		}

		if(iNewRank < m_iRank)
		{
			rank_set(iNewRank, true);
		}
	}
}

