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
#include "matrixpoly.hxx"
#include "core_math.h"
#include "tostring_common.hxx"
#include "poly.hxx"
#include "double.hxx"

using namespace std;

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
		}
	}

	MatrixPoly::~MatrixPoly()
	{
		if(isDeletable() == true)
		{
			delete[] m_poPolyMatrix;
		}
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

	int MatrixPoly::rank_max_get(void)
	{
		int *piRank = new int[size_get()];
		rank_get(piRank);
		int iMaxRank = 0;
		for(int i = 0 ; i < size_get() ; i++)
		{
			iMaxRank = Max(iMaxRank, piRank[i]);
		}
		return iMaxRank;
	}

	Double* MatrixPoly::coef_get(void)
	{
		int iMaxRank = rank_max_get();
		Double *pCoef = new Double(rows_get(), cols_get() * iMaxRank, false);
		if(isComplex())
		{
			pCoef->complex_set(true);
		}

		double *pCoefR	= pCoef->real_get();
		double *pCoefI	= pCoef->img_get();

		for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
		{
			for(int i = 0 ; i < size_get() ; i++)
			{
				Poly *pPoly	= poly_get(i);
				if(iRank > pPoly->rank_get())
				{
					pCoefR[iRank * size_get() + i] = 0;
					if(isComplex())
					{
						pCoefI[iRank * size_get() + i] = 0;
					}
				}
				else
				{
					double *pR	= pPoly->coef_get()->real_get();
					double *pI	= pPoly->coef_get()->img_get();

					pCoefR[iRank * size_get() + i] = pR[iRank];
					if(isComplex())
					{
						pCoefI[iRank * size_get() + i] = pI[iRank];
					}
				}
			}
		}
		return pCoef;
	}

	void MatrixPoly::coef_set(Double *_pCoef)
	{
		int iMaxRank = rank_max_get();

		complex_set(_pCoef->isComplex());
		double *pR = _pCoef->real_get();
		double *pI = _pCoef->img_get();
		for(int i = 0 ; i < size_get() ; i++)
		{
			Double *pTemp = new Double(1, iMaxRank, _pCoef->isComplex());
			Poly *pPoly = poly_get(i);
			for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
			{
				pTemp->real_get()[iRank] = pR[iRank * size_get() + i];
			}
			if(isComplex())
			{
				for(int iRank = 0 ; iRank < iMaxRank ; iRank++)
				{
					pTemp->img_get()[iRank] = pI[iRank * size_get() + i];
				}
			}

			pPoly->coef_set(pTemp);
		}
	}

	std::string	MatrixPoly::toString(int _iPrecison, int _iLineLen)
	{
		ostringstream ostr;
		ostringstream osExp;
		ostringstream osCoef;

		list<string>::const_iterator it_Exp;
		list<string>::const_iterator it_Coef;
		list<string> listExpR, listCoefR, listExpI, listCoefI;

		if(m_iRows == 1 && m_iCols == 1)
		{
			if(m_bComplex)
			{
				ostr << "Real part" << endl << endl << endl;
				poly_get(0)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
				for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}

				ostr << "Imaginary part" << endl << endl << endl ;
				poly_get(0)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpI, &listCoefI);
				for(it_Coef = listCoefI.begin(), it_Exp = listExpI.begin() ; it_Coef != listCoefI.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}
			}
			else
			{
				poly_get(0)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);

				for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
				{
					ostr << *it_Exp << endl << *it_Coef << endl;
				}
			}
		}
		else if(m_iRows == 1)
		{
			if(m_bComplex)
			{
				ostr << "Real part" << endl << endl;
				ostr << GetRowString(_iPrecison, _iLineLen, false);
				ostr << "Imaginary part" << endl << endl;
				ostr << GetRowString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << GetRowString(_iPrecison, _iLineLen, false);
			}
		}
		else if(m_iCols == 1)
		{
			if(m_bComplex)
			{
				ostr << "Real part" << endl << endl;
				ostr << GetColString(_iPrecison, _iLineLen, false);
				ostr << "Imaginary part" << endl << endl;
				ostr << GetColString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << GetColString(_iPrecison, _iLineLen, false);
			}
		}
		else
		{//Matrix
			if(m_bComplex)
			{
				ostr << "Real part" << endl << endl;
				ostr << GetMatrixString(_iPrecison, _iLineLen, false);
				ostr << "Imaginary part" << endl << endl;
				ostr << GetMatrixString(_iPrecison, _iLineLen, true);
			}
			else
			{
				ostr << GetMatrixString(_iPrecison, _iLineLen, false);
			}
		}
		ostr << endl;
		return ostr.str();
	}

	string MatrixPoly::GetMatrixString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		ostringstream ostr;
		ostringstream osExp;
		ostringstream osCoef;

		list<string>::const_iterator it_Exp;
		list<string>::const_iterator it_Coef;
		list<string> listExpR, listCoefR, listExpI, listCoefI;

		int iLen				= 0;
		int iLastFlush	= 0;
		int iLastCol		= 0;

		string szExp, szCoef;

		int *piMaxLen = new int[m_iCols];
		memset(piMaxLen, 0x00, sizeof(int) * m_iCols);

		//find the largest row for each col
		for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)
		{
			for(int iRows1 = 0 ; iRows1 < m_iRows ; iRows1++)
			{
				int iLen = 0;
				if(_bComplex)
				{
					poly_get(iRows1, iCols1)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
				}
				else
				{
					poly_get(iRows1, iCols1)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
				}

				if(listExpR.size() > 1)
				{
					for(it_Exp = listExpR.begin() ; it_Exp != listExpR.end() ; it_Exp++)
					{
						iLen += (int)(*it_Exp).size();
					}
				}
				else
				{
					if(listExpR.front().size() != 0)
					{
						iLen = (int)listExpR.front().size();
					}
					else
					{
						iLen = (int)listCoefR.front().size();
					}
				}
				piMaxLen[iCols1] = Min(Max(piMaxLen[iCols1], iLen), _iLineLen);
				listExpR.clear();
				listCoefR.clear();
			}

			//We know the length of the column

			if((int)(iLen + piMaxLen[iCols1]) >= _iLineLen && iLen != 0)
			{//if the max length exceeded
				ostringstream ostemp;
				for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
				{
					bool bMultiLine = false;
					for(int iCols2 = iLastCol ; iCols2 < iCols1; iCols2++)
					{
						if(_bComplex)
						{
							poly_get(iRows2, iCols2)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
						}
						else
						{
							poly_get(iRows2, iCols2)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
						}

						if(listCoefR.size() > 1)
						{
							for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
							{
								osExp << *it_Exp;
								Add_Space(&osExp, piMaxLen[iCols2] - (int)(*it_Exp).size());
								osExp << endl;

								osExp << *it_Coef;
								Add_Space(&osExp, piMaxLen[iCols2] - (int)(*it_Coef).size());
								osExp << endl;
								bMultiLine = true;
							}
						}
						else
						{
							osExp << listExpR.front();
							Add_Space(&osExp, piMaxLen[iCols2] - (int)listExpR.front().size());
							osCoef << listCoefR.front();
							Add_Space(&osCoef, piMaxLen[iCols2] - (int)listCoefR.front().size());
							bMultiLine = false;
						}
						listExpR.clear();
						listCoefR.clear();
					}

					if(bMultiLine == false)
					{
						osExp << endl;
						osCoef << endl;
					}
					ostemp << osExp.str();
					ostemp << osCoef.str() << endl;
					osExp.str("\x00");
					osCoef.str("\x00");

				}
				iLen	= piMaxLen[iCols1];

				//write "column x to y"
				if(iLastCol + 1 == iCols1)
				{
					ostr << endl << "         Column " << iCols1 << endl << endl;
				}
				else
				{
					ostr << endl << "         Column " << iLastCol + 1 << " to " << iCols1 << endl << endl;
				}

				ostr << ostemp.str() << endl;

				iLastCol = iCols1;
			}
			else //if((int)(iLen + piMaxLen[iCols1]) <= _iLineLen)
			{
				iLen += piMaxLen[iCols1];
			}
		}//for(int iCols1 = 0 ; iCols1 < m_iCols ; iCols1++)

		if(iLastCol + 1 == m_iCols)
		{
			ostr << endl << "         Column " << m_iCols << endl << endl;
		}
		else
		{
			ostr << endl << "         Column " << iLastCol + 1 << " to " << m_iCols << endl << endl;
		}
		//print the end
		for(int iRows2 = 0 ; iRows2 < m_iRows ; iRows2++)
		{
			for(int iCols2 = iLastCol ; iCols2 < m_iCols ; iCols2++)
			{
				if(_bComplex)
				{
					poly_get(iRows2, iCols2)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
				}
				else
				{
					poly_get(iRows2, iCols2)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
				}

				if(listCoefR.size() > 1)
				{
					for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
					{//normally useless ...
						osExp << *it_Exp;
						Add_Space(&osExp, piMaxLen[iCols2] - (int)(*it_Exp).size());
						osExp << endl;

						osExp << *it_Coef;
						Add_Space(&osExp, piMaxLen[iCols2] - (int)(*it_Coef).size());
						osExp << endl;
					}
				}
				else
				{
					if(listExpR.front().size() != 0)
					{
						osExp << listExpR.front();
					}

					Add_Space(&osExp, piMaxLen[iCols2] - (int)listExpR.front().size());
					osCoef << listCoefR.front();
					Add_Space(&osCoef, piMaxLen[iCols2] - (int)listCoefR.front().size());
				}
				listExpR.clear();
				listCoefR.clear();
			}

			if(osExp.str().size() != 0)
			{
				osExp << endl;
			}
			osCoef << endl;
			ostr << osExp.str();
			ostr << osCoef.str() << endl;
			osExp.str("\x00");
			osCoef.str("\x00");
		}
		return ostr.str();
	}

	string MatrixPoly::GetRowString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		int iLen				= 0;
		int iLastFlush	= 0;

		ostringstream ostr;
		ostringstream osExp;
		ostringstream osCoef;

		list<string>::const_iterator it_Exp;
		list<string>::const_iterator it_Coef;
		list<string> listExpR, listCoefR, listExpI, listCoefI;

		for(int i = 0 ; i < m_iSize ; i++)
		{
			string szExp, szCoef;

			if(_bComplex)
			{
				poly_get(i)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
			}
			else
			{
				poly_get(i)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
			}
			if(iLen != 0 && (int)(iLen + listExpR.front().size()) > _iLineLen)
			{//flush strean
				if(i == iLastFlush + 1)
				{
					ostr << endl << "         Column " << i << endl << endl;
				}
				else
				{
					ostr << endl << "         Column " << iLastFlush + 1 /* 2 is better than 1, no ? */<< " to " << i << endl << endl;
				}

				iLastFlush	= i;
				iLen				= 0;
				ostr << osExp.str() << endl;
				ostr << osCoef.str() << endl;
				osExp.str("\x00");
				osCoef.str("\x00");
			}

			if(listCoefR.size() > 1)
			{
				for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
				{
					osExp << *it_Exp << endl << *it_Coef << endl;
				}
			}
			else
			{
				osExp << listExpR.front();
				osCoef << listCoefR.front();
			}

			if(osExp.str().size() != 0)
			{
				iLen = (int)osExp.str().size();
			}
			else
			{
				iLen = (int)osCoef.str().size();
			}

			listCoefR.clear();
			listExpR.clear();
		}

		if(iLastFlush != 0)
		{//last line of a multiline output
			if(iLastFlush + 1 == m_iSize)
			{
				ostr << endl << "         Column " << m_iSize << endl << endl;
			}
			else
			{
				ostr << endl << "         Column " << iLastFlush + 1 << " to " << m_iSize << endl << endl;
			}
		}
		ostr << osExp.str() << endl;
		ostr << osCoef.str() << endl;
		return ostr.str();
	}

	string MatrixPoly::GetColString(int _iPrecison, int _iLineLen, bool _bComplex)
	{
		int iLen				= 0;
		int iLastFlush	= 0;

		ostringstream ostr;
		ostringstream osExp;
		ostringstream osCoef;

		list<string>::const_iterator it_Exp;
		list<string>::const_iterator it_Coef;
		list<string> listExpR, listCoefR, listExpI, listCoefI;

		for(int i = 0 ; i < m_iSize ; i++)
		{
			string szExp, szCoef;

			if(_bComplex)
			{
				poly_get(i)->toStringImg(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
			}
			else
			{
				poly_get(i)->toStringReal(_iPrecison, _iLineLen, var_get(), &listExpR, &listCoefR);
			}

			for(it_Coef = listCoefR.begin(), it_Exp = listExpR.begin() ; it_Coef != listCoefR.end() ; it_Coef++,it_Exp++)
			{
				ostr << *it_Exp << endl << *it_Coef << endl;
			}
			ostr << endl;
			listCoefR.clear();
			listExpR.clear();
		}
		return ostr.str();
	}

	bool MatrixPoly::insert(int _iRows, int _iCols, MatrixPoly *_poSource)
	{
		int iRows = _poSource->rows_get();
		int iCols = _poSource->cols_get();

		if(_iRows + iRows > m_iRows || _iCols + iCols > m_iCols)
		{
			return false;
		}

		for(int iRow = 0 ; iRow < iRows ; iRow++)
		{
			for(int iCol = 0 ; iCol < iCols ; iCol++)
			{
				poly_set(_iRows + iRow, _iCols + iCol, _poSource->poly_get(iRow, iCol)->coef_get());
			}
		}
		return true;
	}
	Double* MatrixPoly::extract_coef(int _iRank)
	{
		Double *pdbl	= new Double(m_iRows, m_iCols, m_bComplex);
		double *pReal	= pdbl->real_get();
		double *pImg	= pdbl->img_get();

		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = poly_get(i);

			if(pPoly->rank_get() <= _iRank)
			{
				pReal[i] = 0;
				if(m_bComplex)
				{
					pImg[i]		= 0;
				}
			}
			else
			{
				pReal[i]		= pPoly->coef_get()->real_get()[_iRank];
				if(m_bComplex)
				{
					pImg[i]		= pPoly->coef_get()->img_get()[_iRank];
				}
			}
		}

		return pdbl;
	}
	bool MatrixPoly::insert_coef(int _iRank, Double* _pCoef)
	{
		double *pReal	= _pCoef->real_get();
		double *pImg	= _pCoef->img_get();

		for(int i = 0 ; i < m_iSize ; i++)
		{
			Poly *pPoly = poly_get(i);
			if(pPoly->rank_get() <= _iRank)
			{
				return false;
			}

			pPoly->coef_get()->real_get()[_iRank] = pReal[i];
			if(m_bComplex)
			{
				pPoly->coef_get()->img_get()[_iRank] = pImg[i];
			}
		}
		return true;
	}

	bool MatrixPoly::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealPoly)
		{
			return false;
		}

		MatrixPoly* pM = pIT->getAsPoly();

		if(pM->rows_get() != rows_get() || pM->cols_get() != cols_get())
		{
			return false;
		}

		if(pM->isComplex() != isComplex())
		{
			return false;
		}

		for(int i = 0 ; i < size_get() ; i++)
		{
			Poly* p1 = poly_get(i);
			Poly* p2 = pM->poly_get(i);

			if(*p1 != *p2)
			{
				return false;
			}
		}
		return true;
	}

	bool MatrixPoly::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

	GenericType* MatrixPoly::get_col_value(int _iPos)
	{
		MatrixPoly* pMP = NULL;
		if(_iPos < cols_get())
		{
			int *piRank = new int[rows_get()];
			for(int i = 0 ; i < rows_get() ; i++)
			{
				piRank[i] = poly_get(i, _iPos)->rank_get();
			}

			pMP = new MatrixPoly(var_get(), rows_get(), 1, piRank);
			for(int i = 0 ; i < rows_get() ; i++)
			{
				pMP->poly_set(i, 0, poly_get(i, _iPos)->coef_get());
			}

			delete[] piRank;
		}

		return pMP;
	}
}

