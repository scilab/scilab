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
#include <math.h>
#include "poly.hxx"
#include "tostring_common.hxx"
#include "double.hxx"

extern "C"
{
#include "log.h"
#include "exp.h"
#include "elem_common.h"
}

using namespace std;

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

		m_pdblCoef->zero_set();
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

	Poly* Poly::getAsSinglePoly(void)
	{
		return this;
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

				if(m_bComplex == false)
				{
					CreatePoly(&pR, NULL, _iRank);
				}
				else
				{
					CreatePoly(&pR, &pI, _iRank);
				}
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
				pCoef = new Double(1, 1, &pR, &pI);
				complex_set(m_pdblCoef->isComplex());
				pCoef->val_set(0,0,0,0);
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
		return RealSinglePoly;
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
		double dblEps = getRelativeMachinePrecision();
		int iNewRank = m_iRank;
		double *pCoefR = coef_get()->real_get();
		double *pCoefI = coef_get()->img_get();
		for(int i = m_iRank - 1; i >= 0 ; i--)
		{
			if(fabs(pCoefR[i]) <= dblEps && (pCoefI != NULL ? fabs(pCoefI[i]) : 0) <= dblEps)
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

	wstring Poly::toString(int _iPrecision, int _iLineLen)
	{
	  wostringstream ostr;

	  // FIXME : implement this.
	  ostr << L"FIXME : implement Poly::toString" << std::endl;

	  return ostr.str();
	}

	void Poly::toStringReal(int _iPrecision, int _iLineLen, wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
	{
		toStringInternal(m_pdblCoef->real_get(),_iPrecision, _iLineLen, _szVar, _pListExp, _pListCoef);
	}

	void Poly::toStringImg(int _iPrecision, int _iLineLen, wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
	{
		if(isComplex() == false)
		{
			_pListExp->clear();
			_pListCoef->clear();
			return;
		}

		toStringInternal(m_pdblCoef->img_get(),_iPrecision, _iLineLen, _szVar, _pListExp, _pListCoef);
	}

	void Poly::toStringInternal(double *_pdblVal, int _iPrecision, int _iLineLen, wstring _szVar, list<wstring>* _pListExp , list<wstring>* _pListCoef)
	{
		wostringstream ostemp;
		wostringstream ostemp2;

		ostemp << L"  ";

		//to add exponant value a the good place
		int *piIndexExp = new int[m_iRank];

		int iLen				= 0;
		int iLastFlush	= 2;
		for(int i = 0 ; i < m_iRank ; i++)
		{
			piIndexExp[i] = 0;
			if(isZero(_pdblVal[i]) == false)
			{
				int iWidth = 0, iPrec = 0;
				bool bFP = false; // FloatingPoint
				GetDoubleFormat(_pdblVal[i], _iPrecision, &iWidth, &iPrec, &bFP);

				if(iLen + iWidth + 2 >= _iLineLen)
				{//flush
					for(int j = iLastFlush ; j < i ; j++)
					{
						if(piIndexExp[j] == 0)
						{
							continue;
						}

						Add_Space(&ostemp2, piIndexExp[j] - static_cast<int>(ostemp2.str().size()));
						if(isZero(_pdblVal[j]) == false)
							ostemp2 << j;
					}
					iLastFlush = i;
					_pListExp->push_back(ostemp2.str());
					ostemp2.str(L""); //reset stream
					Add_Space(&ostemp2, 12); //take from scilab ... why not ...

					_pListCoef->push_back(ostemp.str());
					ostemp.str(L""); //reset stream
					Add_Space(&ostemp, 12); //take from scilab ... why not ...
				}
				AddDoubleValue(&ostemp, _pdblVal[i], iWidth, iPrec, ostemp.str().size() != 2, i == 0);

				if(i != 0)
				{
					ostemp << _szVar;
					piIndexExp[i] = static_cast<int>(ostemp.str().size());
				}
				ostemp << L" ";
				iLen = static_cast<int>(ostemp.str().size());
			}
		}

		if(iLastFlush != 0)
		{
			for(int j = iLastFlush ; j < m_iRank ; j++)
			{
				if(piIndexExp[j] == 0)
				{
					continue;
				}

				Add_Space(&ostemp2, piIndexExp[j] - static_cast<int>(ostemp2.str().size()));
				if(isZero(_pdblVal[j]) == false)
				{
					ostemp2 << j;
				}
			}

			if(ostemp.str() == L"  ")
			{
				ostemp << L"  0";
				Add_Space(&ostemp2, static_cast<int>(ostemp.str().size()));
			}

			_pListExp->push_back(ostemp2.str());
			_pListCoef->push_back(ostemp.str());
		}

		delete[] piIndexExp;
		return;
	}

	bool Poly::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).getType() != RealSinglePoly)
		{
			return false;
		}

		Poly* pP = const_cast<InternalType &>(it).getAsSinglePoly();

		if(rank_get() != pP->rank_get())
		{
			return false;
		}

		double* pR1 = coef_real_get();
		double *pR2 = pP->coef_real_get();

		if(memcmp(pR1, pR2, sizeof(double) * rank_get()) != 0)
		{
			return false;
		}

		if(isComplex())
		{
			double* pI1 = coef_img_get();
			double *pI2 = pP->coef_img_get();

			if(memcmp(pI1, pI2, sizeof(double) * rank_get()) != 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Poly::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}
}


