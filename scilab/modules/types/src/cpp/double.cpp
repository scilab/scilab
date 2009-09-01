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
#include "double.hxx"
#include "tostring_common.hxx"

extern "C"
{
	#include "elem_common.h"
}

using namespace std;


namespace types
{
	/*------------*/
	/*	~Double		*/
	/*------------*/
	Double::~Double()
	{
		if(isDeletable() == true)
		{
			all_delete();
		}
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
	/*				Double			*/
	/*	Real constructor	*/
	/*--------------------*/
	Double::Double(double _dblReal, double _dblImg)
	{
		double *pdblR;
		double *pdblI;
		CreateDouble(1, 1, &pdblR, &pdblI);
		pdblR[0] = _dblReal;
		pdblI[0] = _dblImg;
		m_bComplex = true;
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

//		zero_set();
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
				m_bComplex = false;
			}
		}
		else // _bComplex == true
		{
			if(isComplex() == false)
			{
				m_pdblImg = new double[m_iSize];
				memset(m_pdblImg, 0x00, sizeof(double) * m_iSize);
				m_bComplex = true;
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

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Double::val_set(int _iRow, int _iCol, double _dblVal)
	{
		return val_set(_iRow, _iCol, _dblVal, 0);
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
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
		std::cout << "types::Double";
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
			delete[] m_pdblReal;
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
			delete[] m_pdblImg;
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
			for(int i = 0 ; i < m_iSize ; i++)
			{
				m_pdblReal[i] = 0;
			}
			//memset(m_pdblReal, 0x00, m_iSize * sizeof(double));
		}
		else
			return false;

		if(m_bComplex == true)
		{
			if(m_pdblImg != NULL)
			{
				memset(m_pdblImg, 0x00, m_iSize * sizeof(double));
			}
			else
				return false;
		}

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

		return true;
	}

	string Double::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		//if(isComplex() == false)
		{
			/*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
			if(isIdentity())
			{
				ostr << "eye *" << endl << endl;
				if(isComplex() == false)
				{
					int iWidth = 0, iPrec = 0;
					bool bFP = false; // FloatingPoint
					GetDoubleFormat((m_pdblReal[0]), _iPrecision, &iWidth, &iPrec, &bFP);
					AddDoubleValue(&ostr, (m_pdblReal[0]), iWidth, iPrec);
					ostr << endl;
				}
				else
				{//complex value
					int iWidthR = 0, iWidthI = 0;
					int iPrecR = 0, iPrecI = 0;
					bool bFPR = false, bFPI = false; // FloatingPoint
					GetDoubleFormat(ZeroIsZero(m_pdblReal[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
					GetDoubleFormat(ZeroIsZero(m_pdblImg[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
					AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[0]), ZeroIsZero(m_pdblImg[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
					ostr << endl;
				}
				ostr << endl;
			}
			else if(cols_get() == 0 || rows_get() == 0)
			{
				ostr << "   []";
				ostr << endl;
			}
			else if(cols_get() == 1 && rows_get() == 1)
			{//scalar
				if(isComplex() == false)
				{
					int iWidth = 0, iPrec = 0;
					bool bFP = false; // FloatingPoint
					GetDoubleFormat((m_pdblReal[0]), _iPrecision, &iWidth, &iPrec, &bFP);
					AddDoubleValue(&ostr, (m_pdblReal[0]), iWidth, iPrec);
					ostr << endl;
				}
				else
				{//complex value
					int iWidthR = 0, iWidthI = 0;
					int iPrecR = 0, iPrecI = 0;
					bool bFPR = false, bFPI = false; // FloatingPoint
					GetDoubleFormat(ZeroIsZero(m_pdblReal[0]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
					GetDoubleFormat(ZeroIsZero(m_pdblImg[0]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
					AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[0]), ZeroIsZero(m_pdblImg[0]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
					ostr << endl;
				}
			}
			else if(cols_get() == 1)
			{//column vector

				//1 test and two loops or 1 loop and many tests ? good question isn't it ?
				//First is harder to maintaint but execution is faster ( 1 test and 1 loop )
				//Second is slower, 1 loop but many tests.

				//1 test and two loops for me ( AE )
				if(isComplex() == false)
				{
					for(int i = 0 ; i < rows_get() ; i++)
					{
						int iWidth = 0, iPrec = 0;
						bool bFP = false; // FloatingPoint
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]), _iPrecision, &iWidth, &iPrec, &bFP);
						AddDoubleValue(&ostr, ZeroIsZero(m_pdblReal[i]), iWidth, iPrec);
						ostr << endl;
					}
				}
				else
				{
					for(int i = 0 ; i < rows_get() ; i++)
					{//complex value
						int iWidthR = 0, iWidthI = 0;
						int iPrecR = 0, iPrecI = 0;
						bool bFPR = false, bFPI = false; // FloatingPoint
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetDoubleFormat(ZeroIsZero(m_pdblImg[i]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);
						AddDoubleComplexValue(&ostr, ZeroIsZero(m_pdblReal[i]), ZeroIsZero(m_pdblImg[i]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
						ostr << endl;
					}
				}
			}
			else if(rows_get() == 1)
			{//row vector
				ostringstream ostemp;
				int iLastVal = 0;

				//1 test and two loops or 1 loop and many tests ? good question isn't it ?
				//First is harder to maintaint but execution is faster ( 1 test and 1 loop )
				//Second is slower, 1 loop but many tests.

				//1 test and two loops for me ( AE )
				if(isComplex() == false)
				{
					for(int i = 0 ; i < cols_get() ; i++)
					{
						int iWidth = 0, iPrec = 0;
						bool bFP = false; // FloatingPoint
						int iLen = 0;
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]), _iPrecision, &iWidth, &iPrec, &bFP);
						iLen = iWidth + bFP + (int)ostemp.str().size();
						if(iLen > _iLineLen)
						{//Max length, new line
							ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str("\x00");
							iLastVal = i;
						}

						if(ostemp.str().size() != 0)
						{
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}

						AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[i]), iWidth, iPrec);
					}

					if(iLastVal != 0)
					{
						ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
					}
				}
				else //complex case
				{
					for(int i = 0 ; i < cols_get() ; i++)
					{
						int iWidthR = 0, iWidthI = 0;
						int iPrecR = 0, iPrecI = 0;
						bool bFPR = false, bFPI = false; // FloatingPoint
						int iLen = 0;
						GetDoubleFormat(ZeroIsZero(m_pdblReal[i]),	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetDoubleFormat(ZeroIsZero(m_pdblImg[i]),		_iPrecision, &iWidthI, &iPrecI, &bFPI);

						iLen = (int)ostemp.str().size();
						if(isZero(m_pdblImg[i]))
						{
							if(isZero(m_pdblReal[i]))
							{
								iLen += 1; //"0"
							}
							else
							{
								iLen		+= iWidthR + bFPR;
								iWidthI	= 0;
							}
						}
						else
						{
							if(isZero(m_pdblReal[i]))
							{
								iLen		+= iWidthI + bFPI;
								iWidthR	= 0;
							}
							else
							{
								iLen += iWidthR + bFPR;
								iLen += SIZE_BETWEEN_REAL_COMPLEX;
								iLen += iWidthI + bFPI;
							}
						}

						if(iLen > _iLineLen)
						{//Max length, new line
							ostr << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str("");
							iLastVal = i;
						}

						if(ostemp.str().size() != 0)
						{
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}

						AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[i]), ZeroIsZero(m_pdblImg[i]), iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
					}

					if(iLastVal != 0)
					{
						ostr << endl << "       column " << iLastVal + 1 << " to " << cols_get() << endl << endl;
					}
				}
				ostemp << endl;
				ostr << ostemp.str();
			}
			else // matrix
			{
				ostringstream ostemp;
				int iLastVal = 0;
				int iLen = 0;
				int iLastCol = 0;

				//Array with the max printed size of each col
				int *piSize = new int[cols_get()];
				memset(piSize, 0x00, cols_get() * sizeof(int));

				//1 test and two loops or 1 loop and many tests ? good question isn't it ?
				//First is harder to maintaint but execution is faster ( 1 test and 1 loop )
				//Second is slower, 1 loop but many tests.

				//1 test and two loops for me ( AE )
				if(isComplex() == false)
				{
					//compute the row size for padding for each printed bloc.
					for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
					{
						for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
						{
							int iWidth			= 0;
							int iPrec				= 0;
							bool bFP				= false; // FloatingPoint
							int iCurrentLen = 0;

							GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols1 * rows_get() + iRows1]), _iPrecision, &iWidth, &iPrec, &bFP);
							iCurrentLen	= iWidth + bFP;

							if(iCurrentLen > piSize[iCols1])
							{
								piSize[iCols1] = iCurrentLen;
							}
						}

						if(iLen + piSize[iCols1] > _iLineLen)
						{//find the limit, print this part
							for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
							{
								for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
								{
									int iWidth			= 0;
									int iPrec				= 0;
									bool bFP				= false; // FloatingPoint
									GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), _iPrecision, &iWidth, &iPrec, &bFP);
									AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), piSize[iCols2], iPrec);
									ostemp << SPACE_BETWEEN_TWO_VALUES;
								}
								ostemp << endl;
							}
							iLen = 0;
							ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str("");
							iLastCol = iCols1;

						}
						iLen += piSize[iCols1] + SIGN_LENGTH + SIZE_BETWEEN_TWO_VALUES;
					}

					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
						{
							int iWidth			= 0;
							int iPrec				= 0;
							bool bFP				= false; // FloatingPoint
							GetDoubleFormat(ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), _iPrecision, &iWidth, &iPrec, &bFP);
							AddDoubleValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), piSize[iCols2], iPrec);
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}
						ostemp << endl;
					}
					if(iLastCol != 0)
					{
						ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
					}
					ostr << ostemp.str();
				}
				else //Complex case
				{
					//compute the row size for padding for each printed bloc.
					for(int iCols1 = 0 ; iCols1 < cols_get() ; iCols1++)
					{
						for(int iRows1 = 0 ; iRows1 < rows_get() ; iRows1++)
						{
							int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
							int iPrecR = 0, iPrecI = 0;
							bool bFPR = false, bFPI = false; // FloatingPoint
							int iCurrentLen = 0;

							GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols1 * rows_get() + iRows1]), ZeroIsZero(m_pdblImg[iCols1 * rows_get() + iRows1]), _iPrecision,
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							iTotalWidth += (iWidthR == 0 ? 0 : SIGN_LENGTH) + (iWidthI == 0 ? 0 : SIGN_LENGTH + 1);
							if(iTotalWidth > piSize[iCols1])
							{
								piSize[iCols1] = iTotalWidth;
							}
						}

						if(iLen + piSize[iCols1] > _iLineLen)
						{//find the limit, print this part
/*
							for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
							{
//								ostr  << "|%" << piSize[iCols2] << "%|";

								for(int x = 0 ; x < piSize[iCols2]-1 ; x++)
								{
									ostr << " ";
								}
								ostr << SPACE_BETWEEN_REAL_COMPLEX << SYMBOL_I << "|" << SPACE_BETWEEN_TWO_VALUES;
							}
*/
							for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
							{
								for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
								{
									int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
									int iPrecR = 0, iPrecI = 0;
									bool bFPR = false, bFPI = false; // FloatingPoint
									int iCurrentLen = 0;

									GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]), _iPrecision,
										&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

									AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]),
										piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
									ostemp << SPACE_BETWEEN_TWO_VALUES;
								}
								ostemp << endl;
							}
							iLen = 0;
							ostr << endl << "       column " << iLastCol + 1 << " to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str("");
							iLastCol = iCols1;

						}
						iLen += piSize[iCols1] + SIZE_BETWEEN_TWO_VALUES;
					}

					for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
					{
						for(int iCols2 = iLastCol ; iCols2 < cols_get() ; iCols2++)
						{
							int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
							int iPrecR = 0, iPrecI = 0;
							bool bFPR = false, bFPI = false; // FloatingPoint
							int iCurrentLen = 0;

							GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]), _iPrecision,
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]),
								piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}
						ostemp << endl;
					}

					if(iLastCol != 0)
					{
						ostr << endl << "       column " << iLastCol + 1 << " to " << cols_get() << endl << endl;
					}
					ostr << ostemp.str();
				}
			}
		}
		/*
		else
		{
		ostr << "C'est complex ca :'(" << endl;
		}
		*/
		return ostr.str();
	}

	Double* Double::clone()
	{
		double *pReal		= NULL;
		double *pImg		= NULL;

		Double *pReturn = new Double(m_iRows, m_iCols, &pReal);
		memcpy(pReal, m_pdblReal, m_iSize * sizeof(double));

		if(m_bComplex)
		{
			pReturn->complex_set(true);
			memcpy(pReturn->img_get(), m_pdblImg, m_iSize * sizeof(double));
		}
		return pReturn;
	}

	Double* Double::resize(int _iNewRows, int _iNewCols)
	{
		int iNewSize = _iNewRows * _iNewCols;

		if(_iNewRows <= m_iRows && _iNewCols <= m_iCols)
		{
			return NULL;
		}

		Double *pNew = new Double(_iNewRows, _iNewCols, m_bComplex);
		pNew->zero_set();

		if(m_bComplex)
		{
			for(int iRow = 0 ; iRow < m_iRows ; iRow++)
			{
				for(int iCol = 0 ; iCol < m_iCols ; iCol++)
				{
					pNew->val_set(iRow, iCol, real_get(iRow,iCol), img_get(iRow,iCol));
				}
			}
		}
		else
		{
			for(int iRow = 0 ; iRow < m_iRows ; iRow++)
			{
				for(int iCol = 0 ; iCol < m_iCols ; iCol++)
				{
					pNew->val_set(iRow, iCol, real_get(iRow,iCol));
				}
			}
		}
		return pNew;
	}

	bool Double::insert(int _iRows, int _iCols, Double *_poSource)
	{
		int iRows = _poSource->rows_get();
		int iCols = _poSource->cols_get();

		//insert without resize
		if(iRows + _iRows > m_iRows || iCols + _iCols > m_iCols)
		{
			return false;
		}

		if(m_bComplex)
		{
			for(int iRow = 0 ; iRow < iRows ; iRow++)
			{
				for(int iCol = 0 ; iCol < iCols ; iCol++)
				{
					val_set(_iRows + iRow, _iCols + iCol, _poSource->real_get(iRow, iCol), _poSource->img_get(iRow, iCol));
				}
			}
		}
		else
		{
			for(int iRow = 0 ; iRow < iRows ; iRow++)
			{
				for(int iCol = 0 ; iCol < iCols ; iCol++)
				{
					val_set(_iRows + iRow, _iCols + iCol, _poSource->real_get(iRow, iCol));
				}
			}
		}
		return true;
	}

	bool Double::operator==(const InternalType& it)
	{
		InternalType* pIT = (InternalType*)&it;
		if(pIT->getType() != RealDouble)
		{
			return false;
		}

		Double* pdbl = pIT->getAsDouble();

		if(pdbl->rows_get() != rows_get() || pdbl->cols_get() != cols_get())
		{
			return false;
		}

		if(pdbl->isComplex() != isComplex())
		{
			return false;
		}

		double *pdblReal = pdbl->real_get();
		if(memcmp(m_pdblReal, pdblReal, size_get() * sizeof(double)) != 0)
		{
			return false;
		}

		if(isComplex())
		{
			double *pdblImg = pdbl->img_get();
			if(memcmp(m_pdblImg, pdblImg, size_get() * sizeof(double)) != 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Double::operator!=(const InternalType& it)
	{
		return !(*this == it);
	}

}
