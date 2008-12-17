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

#include "matrixpoly.hxx"

namespace types
{
	MatrixPoly::MatrixPoly()
	{
	}

	MatrixPoly::MatrixPoly(string _szVarName, int _iRows, int _iCols, int *_piRank)
	{
		m_iRows			= _iRows;
		m_iCols			= _iCols;
		m_iSize			= m_iRows * m_iCols;
		m_szVarName	= _szVarName;
		m_bComplex	= false;

		m_poPolyMatrix = new Poly[_iRows * _iCols];
		for(int i = 0 ; i < m_iSize ; i++)
		{
			m_poPolyMatrix[i].CreatePoly(NULL, NULL, _piRank[i]);
			if(m_poPolyMatrix[i].isComplex())
				m_bComplex = true;
		}
	}

	MatrixPoly::~MatrixPoly()
	{
		delete m_poPolyMatrix;
	}

	Poly* MatrixPoly::poly_get(int _iRows, int _iCols)
	{
		if(m_poPolyMatrix == NULL || _iRows >= m_iRows || _iCols >= m_iCols)
		{
			return NULL;
		}
		else
		{
			return &m_poPolyMatrix[_iCols * m_iRows + _iRows];
		}
	}

	Poly* MatrixPoly::poly_get(int _iIdx)
	{
		if(m_poPolyMatrix == NULL || _iIdx >= m_iSize)
		{
			return NULL;
		}
		else
		{
			return &m_poPolyMatrix[_iIdx];
		}
	}

	bool MatrixPoly::poly_set(int _iRows, int _iCols, Double *_pdblCoef)
	{
		return poly_set(_iCols * m_iRows + _iRows, _pdblCoef);
	}

	bool MatrixPoly::poly_set(int _iIdx, Double *_pdblCoef)
	{
		if(_iIdx < m_iSize)
		{
			/*Get old Poly*/
			Poly *poPoly = poly_get(_iIdx);
			poPoly->rank_set(_pdblCoef->size_get());
			poPoly->coef_set(_pdblCoef);
		}
		else
		{
			return false;
		}

		return true;
	}

	bool MatrixPoly::rank_get(int *_piRank)
	{
		if(_piRank == NULL || m_poPolyMatrix == NULL)
		{
			return false;
		}

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_piRank[i] = m_poPolyMatrix[i].rank_get();
		}
		return true;
	}

	MatrixPoly* MatrixPoly::getAsPoly()
	{
		return this;
	}

	GenericType::RealType MatrixPoly::getType()
	{
		return RealPoly;
	}

	void MatrixPoly::whoAmI(void) 
	{
		std::cout << "types::Poly"; 
	}

	bool MatrixPoly::isComplex(void)
	{
		return m_bComplex;
	}

	string MatrixPoly::var_get()
	{
		return m_szVarName;
	}

	void MatrixPoly::complex_set(bool _bComplex)
	{
		if(_bComplex != m_bComplex)
		{
			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_poPolyMatrix[i].complex_set(_bComplex);
			}
			m_bComplex = _bComplex;
		}
	}

	MatrixPoly& MatrixPoly::operator= (MatrixPoly& poPoly)
	{
		m_iRows			= (&poPoly)->rows_get();
		m_iCols			= (&poPoly)->cols_get();
		m_iSize			= m_iRows * m_iCols;
		m_szVarName	= (&poPoly)->var_get();
		
		int *piRank = new int[m_iSize];
		m_bComplex	= false;

		(&poPoly)->rank_get(piRank);

		m_poPolyMatrix = new Poly[m_iRows * m_iCols];
		for(int i = 0 ; i < m_iSize ; i++)
		{
			m_poPolyMatrix[i].CreatePoly(NULL, NULL, piRank[i]);
			if(m_poPolyMatrix[i].isComplex())
				m_bComplex = true;
			Double *pCoef = (&poPoly)->poly_get(i)->coef_get();
			m_poPolyMatrix[i].coef_set(pCoef);

		}

		return *this;
	}

	Double* MatrixPoly::evaluate(Double* _pdblValue)
	{
/*
		for(int iPolyR = 0 ; iPolyR < m_iRows ; iPolyR++)
		{
			for(int iPolyC = 0 ; iPolyC < m_iCols ; iPolyC++)
			{
				Poly *pPoly = poly_get(iPolyR, iPolyC);
				pPoly->evaluate(pInR, pInI, &pOutR, &pOutI);

			}
		}
*/
		double *pR	= _pdblValue->real_get();
		double *pI	= _pdblValue->img_get();
		int iRows		= _pdblValue->rows_get();
		int iCols		= _pdblValue->cols_get();

		double *pReturnR	= NULL;
		double *pReturnI	= NULL;
		Double *pReturn		= new Double(m_iRows * iRows, m_iCols * iCols, &pReturnR, &pReturnI);
		if(pI != NULL)
		{
			pReturn->complex_set(true);
		}
		else
		{
			pReturn->complex_set(false);
		}

		int i = 0;
		//all lines of the matrix remplacement
		for(int iCol = 0 ; iCol < iCols ; iCol++)
		{
			for(int iPolyCol = 0 ; iPolyCol < m_iCols ; iPolyCol++)
			{
				for(int iRow = 0 ; iRow < iRows ; iRow++)
				{
					for(int iPolyRow = 0 ; iPolyRow < m_iRows ; iPolyRow++)
					{
						double OutR	= 0;
						double OutI	= 0;

						Poly *pPoly = poly_get(iPolyRow, iPolyCol);
						if(pReturn->isComplex())
						{
							pPoly->evaluate(pR[iCol * iRows + iRow], pI[iCol * iRows + iRow], &OutR, &OutI);
							pReturnR[i]	= OutR;
							pReturnI[i]	= OutI;
						}
						else
						{
							pPoly->evaluate(pR[iCol * iRows + iRow], 0, &OutR, &OutI);
							pReturnR[i]	= OutR;
						}
						i++;
					}
				}
			}
		}
		return pReturn;
	}

	void MatrixPoly::update_rank(void)
	{
		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = poly_get(i);
			pPoly->update_rank();
		}
	}
}

