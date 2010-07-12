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
		m_iSizeMax = m_iSize;

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

    bool Double::isEmpty()
    {
        if(m_iRows == 0 && m_iCols == 0)
        {
            return true;
        }
        return false;
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
	bool Double::val_set(int _iRows, int _iCols, double _dblVal)
	{
		return val_set(_iRows, _iCols, _dblVal, 0);
	}

	/*----------------*/
	/*		val_set		*/
	/*----------------*/
	bool Double::val_set(int _iRows, int _iCols, double _dblReal, double _dblImg)
	{
		if(m_pdblReal != NULL)
		{
			if(_iRows < m_iRows && _iCols < m_iCols)
			{
				m_pdblReal[_iCols * m_iRows + _iRows] = _dblReal;
				if(m_bComplex)
				{
					m_pdblImg[_iCols * m_iRows + _iRows] = _dblImg;
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
	/*	getAsDouble		*/
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

	wstring Double::toString(int _iPrecision, int _iLineLen)
	{
		wostringstream ostr;
		//if(isComplex() == false)
		{
			/*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
			if(isIdentity())
			{
				ostr << L"eye *" << endl << endl;
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
				ostr << L"   []";
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
				wostringstream ostemp;
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
						iLen = iWidth + bFP + static_cast<int>(ostemp.str().size());
						if(iLen > _iLineLen)
						{//Max length, new line
							ostr << endl << L"       column " << iLastVal + 1 << L" to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str(L"");
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
						ostr << endl << L"       column " << iLastVal + 1 << L" to " << cols_get() << endl << endl;
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

						iLen = static_cast<int>(ostemp.str().size());
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
							ostr << endl << L"       column " << iLastVal + 1 << L" to " << i << endl << endl;
							ostr << ostemp.str() << endl;
							ostemp.str(L"");
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
						ostr << endl << L"       column " << iLastVal + 1 << L" to " << cols_get() << endl << endl;
					}
				}
				ostemp << endl;
				ostr << ostemp.str();
			}
			else // matrix
			{
				wostringstream ostemp;
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
							ostr << endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str(L"");
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
						ostr << endl << L"       column " << iLastCol + 1 << L" to " << cols_get() << endl << endl;
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

									GetComplexFormat(	ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]), _iPrecision,
										&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

									AddDoubleComplexValue(&ostemp, ZeroIsZero(m_pdblReal[iCols2 * rows_get() + iRows2]), ZeroIsZero(m_pdblImg[iCols2 * rows_get() + iRows2]),
										piSize[iCols2], iWidthR, iWidthI, Max(iPrecR, iPrecI));
									ostemp << SPACE_BETWEEN_TWO_VALUES;
								}
								ostemp << endl;
							}
							iLen = 0;
							ostr << endl << L"       column " << iLastCol + 1 << L" to " << iCols1 << endl << endl;;
							ostr << ostemp.str();
							ostemp.str(L"");
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
						ostr << endl << L"       column " << iLastCol + 1 << L" to " << cols_get() << endl << endl;
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

		Double *pReturn = new Double(m_iRows, m_iCols, &pReal);
		memcpy(pReal, m_pdblReal, m_iSize * sizeof(double));

		if(m_bComplex)
		{
			pReturn->complex_set(true);
			memcpy(pReturn->img_get(), m_pdblImg, m_iSize * sizeof(double));
		}
		return pReturn;
	}

	bool Double::resize(int _iNewRows, int _iNewCols)
	{
		if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
		{//nothing to do
			return true;
		}

		//alloc new data array
		double* pdblReal = NULL;
		double* pdblImg = NULL;

		if(isComplex())
		{
			if(m_iSizeMax < _iNewRows * _iNewCols)
			{
				//alloc 10% bigger than asked to prevent future resize
				m_iSizeMax = static_cast<int>(_iNewRows * _iNewCols * 1.1);

				pdblReal	= new double[m_iSizeMax];
				memset(pdblReal, 0x00, sizeof(double) * m_iSizeMax);
				pdblImg		= new double[m_iSizeMax];
				memset(pdblImg, 0x00, sizeof(double) * m_iSizeMax);

				for(int i = 0 ; i < rows_get() ; i++)
				{
					for(int j = 0 ; j < cols_get() ; j++)
					{
						pdblReal[j * _iNewRows + i] = m_pdblReal[j * rows_get() + i];
						pdblImg[j * _iNewRows + i]	= m_pdblImg[j * rows_get() + i];
					}
				}

				delete[] m_pdblReal;
				delete[] m_pdblImg;
				m_pdblReal	= pdblReal;
				m_pdblImg		= pdblImg;
			}
			else
			{
				//if vector or if row dimension not change, we don't need to shift data
				if(_iNewRows != 1 && _iNewCols != 1 && rows_get() != _iNewRows)
				{
					for(int i = cols_get() - 1 ; i >= 0 ; i--)
					{
						for(int j = rows_get() - 1 ; j >= 0 ; j--)
						{
							m_pdblReal[(i * _iNewRows) + j] = m_pdblReal[(i * rows_get()) + j];
							m_pdblImg[(i * _iNewRows) + j] 	= m_pdblImg[(i * rows_get()) + j];
						}
						
						//fill zero at the end of column
						memset(m_pdblReal + (i * _iNewRows) + rows_get(), 0x00, sizeof(double) * (_iNewRows - rows_get()));
						memset(m_pdblImg + (i * _iNewRows) + rows_get(), 0x00, sizeof(double) * (_iNewRows - rows_get()));
					}
				}
			}	
			
		}
		else
		{
			if(m_iSizeMax < _iNewRows * _iNewCols)
			{
				//alloc 10% bigger than asked to prevent future resize
				m_iSizeMax = static_cast<int>(_iNewRows * _iNewCols * 1.1);
				
				pdblReal	= new double[m_iSizeMax];
				memset(pdblReal, 0x00, sizeof(double) * m_iSizeMax);

				for(int i = 0 ; i < rows_get() ; i++)
				{
					for(int j = 0 ; j < cols_get() ; j++)
					{
						pdblReal[j * _iNewRows + i] = m_pdblReal[j * rows_get() + i];
					}
				}

				delete[] m_pdblReal;
				m_pdblReal	= pdblReal;
			}
			else
			{
				//if vector or if row dimension not change, we don't need to shift data
				if(_iNewRows != 1 && _iNewCols != 1 && rows_get() != _iNewRows)
				{
					for(int i = cols_get() - 1 ; i >= 0 ; i--)
					{
						for(int j = rows_get() - 1 ; j >= 0 ; j--)
						{
							m_pdblReal[(i * _iNewRows) + j] = m_pdblReal[(i * rows_get()) + j];
						}
						
						//fill zero at the end of column
						memset(m_pdblReal + (i * _iNewRows) + rows_get(), 0x00, sizeof(double) * (_iNewRows - rows_get()));
					}
				}
			}	
		}

		m_iRows = _iNewRows;
		m_iCols	= _iNewCols;
		m_iSize = m_iRows * m_iCols;
		return true;
	}

    bool Double::fillFromCol(int _iCols, Double *_poSource)
    {
        if(m_bComplex)
        {
            int iDestOffset = _iCols * m_iRows;
            memcpy(m_pdblReal + iDestOffset, _poSource->real_get(), _poSource->size_get() * sizeof(double));
            memcpy(m_pdblImg + iDestOffset, _poSource->img_get(), _poSource->size_get() * sizeof(double));
        }
        else
        {
            //memcpy version
/*
            int iDestOffset = _iCols * m_iRows;
            if(_poSource->size_get() ==  1)
            {
                m_pdblReal[iDestOffset] =  _poSource->real_get()[0];
            }
            else
            {
                memcpy(m_pdblReal + iDestOffset, _poSource->real_get(), _poSource->size_get() * sizeof(double));
            }
*/
            //loop version
/*
            int iDestOffset = _iCols * m_iRows;
            for(int i = 0 ; i < _poSource->size_get() ; i++)
            {
                m_pdblReal[iDestOffset + i] =  _poSource->real_get()[i];
            }
*/
            //blas
            int iDestOffset     = _iCols * m_iRows;
            int iSize           = _poSource->size_get();
            double* pdblDest    = m_pdblReal + iDestOffset;
            int iOne            = 1;
            dcopy_(&iSize, _poSource->real_get(), &iOne, pdblDest, &iOne);
        }
        return true;
    }

    bool Double::fillFromRow(int _iRows, Double *_poSource)
    {
        int iCols = _poSource->cols_get();

        if(m_bComplex)
        {
        }
        else
        {
            for(int i = 0 ; i < iCols ; i++)
            {
                //memcpy version
/*                
                int iDestOffset = i * m_iRows + _iRows;
                int iOrigOffset = i * _poSource->rows_get();
                if(_poSource->rows_get() == 1)
                {
                    m_pdblReal[iDestOffset] = _poSource->real_get()[iOrigOffset];
                }
                else
                {
                    memcpy(m_pdblReal + iDestOffset, _poSource->real_get() + iOrigOffset, _poSource->rows_get() * sizeof(double));
                }
*/                
                //loop version
/*
                int iDestOffset = i * m_iRows + _iRows;
                int iOrigOffset = i * _poSource->rows_get();
                for(int j = 0 ; j < _poSource->rows_get() ; j++)
                {
                    m_pdblReal[iDestOffset + j] = _poSource->real_get()[iOrigOffset + j];
                }
*/

                int iDestOffset     = i * m_iRows + _iRows;
                int iOrigOffset     = i * _poSource->rows_get();
                int iSize           = _poSource->rows_get();
                double* pdblDest    = m_pdblReal + iDestOffset;
                double* pdblSource  = _poSource->real_get() + iOrigOffset;
                int iOne            = 1;

                dcopy_(&iSize, pdblSource, &iOne, pdblDest, &iOne);
            }


        }
        return true;
    }

	bool Double::append(int _iRows, int _iCols, Double *_poSource)
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
                int iDestOffset = _iCols * m_iRows + iRow;
                int iOrigOffset = iCols * _poSource->rows_get() + iRow;
                memcpy(m_pdblReal + iDestOffset, _poSource->real_get() + iOrigOffset, iCols * sizeof(double));
                if(_poSource->isComplex())
                {
                    memcpy(m_pdblImg + iDestOffset, _poSource->img_get() + iOrigOffset, iCols * sizeof(double));
                }
                else
                {
                    memset(m_pdblImg + iDestOffset, 0x00, iCols * sizeof(double));
                }

				//for(int iCol = 0 ; iCol < iCols ; iCol++)
				//{
				//	val_set(_iRows + iRow, _iCols + iCol, _poSource->real_get(iRow, iCols), _poSource->img_get(iRow, iCol));
				//}
			}
		}
		else
		{
            if(iRows != 1)
            {
                for(int iCol = 0 ; iCol < iCols ; iCol++)
                {
                    int iDestOffset = (iCol + _iCols ) * m_iRows + _iRows;
                    int iOrigOffset = iCol * _poSource->rows_get();
                    memcpy(m_pdblReal + iDestOffset, _poSource->real_get() + iOrigOffset, iRows * sizeof(double));
                }
            }
            else
            {
                for(int iCol = 0 ; iCol < iCols ; iCol++)
                {
                    val_set(_iRows, _iCols + iCol, _poSource->real_get(0, iCol));
                }
            }

            //for(int iRow = 0 ; iRow < iRows ; iRow++)
            //{
            //    for(int iCol = 0 ; iCol < iCols ; iCol++)
            //    {
            //        val_set(_iRows + iRow, _iCols + iCol, _poSource->real_get(iRow, iCol));
            //    }
            //}
		}
		return true;
	}

	bool Double::operator==(const InternalType& it)
	{
		if(const_cast<InternalType &>(it).getType() != RealDouble)
		{
			return false;
		}

		Double* pdbl = const_cast<InternalType &>(it).getAsDouble();

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

	GenericType*	Double::get_col_value(int _iPos)
	{
		Double *pdbl = NULL;
		if(_iPos < m_iCols)
		{
			if(isComplex())
			{
				pdbl = new Double(m_iRows, 1, true);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pdbl->val_set(i, 0, real_get(i, _iPos), img_get(i, _iPos));
				}
			}
			else
			{
				pdbl = new Double(m_iRows, 1);
				for(int i = 0 ; i < m_iRows ; i++)
				{
					pdbl->val_set(i, 0, real_get(i, _iPos));
				}
			}
		}
		return pdbl;
	}

	bool Double::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
	{
		int iNewRows = rows_get();
		int iNewCols = cols_get();
		//check input size
		if(_bAsVector == false)
		{
			if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
			{//compute new dimensions
				iNewRows = Max(_piMaxDim[0], rows_get());
				iNewCols = Max(_piMaxDim[1], cols_get());
			}
		}
		else
		{
			if(size_get() < _piMaxDim[0])
			{
				if(rows_get() == 1 || size_get() == 0)
				{
					iNewRows = 1;
					iNewCols = _piMaxDim[0];
				}
				else if(cols_get() == 1)
				{
					iNewRows = _piMaxDim[0];
					iNewCols = 1;
				}
				else
				{
					return false;
				}
			}
		}

		//check if the size of _poSource is compatible with the size of the variable
		if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
		{
			return false;
		}
		else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->size_get()))
		{
			return false;
		}


		//check if the count of values is compatible with indexes
		if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
		{
			return false;
		}


		switch(_poSource->getType())
		{
		case InternalType::RealDouble :
			{
				Double *pIn = _poSource->getAsDouble();

				//Only resize after all tests !
				if(resize(iNewRows, iNewCols) == false)
				{
					return false;
				}

				//variable can receive new values.
				if(pIn->size_get() == 1)
				{//a(?) = x
					if(pIn->isComplex())
					{//a(?) = C
						double* pInR = pIn->real_get();
						double* pInI = pIn->img_get();

						complex_set(true);//do nothing if variable is already complex

						if(_bAsVector)
						{//a([]) = C
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[0];
								m_pdblImg[_piSeqCoord[i] - 1]		= pInI[0];
							}
						}
						else
						{//a([],[]) = C
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[0];
								m_pdblImg[iPos]		= pInI[0];
							}
						}
					}
					else
					{//a(?) = R
						double* pInR = pIn->real_get();

						if(_bAsVector)
						{//a([]) = R
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[0];
							}
						}
						else
						{//a([],[]) = R
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[0];
							}
						}
					}
				}
				else
				{//a(?) = [x]
					if(pIn->isComplex())
					{//a(?) = [C]
						double* pInR = pIn->real_get();
						double* pInI = pIn->img_get();

						complex_set(true);//do nothing if variable is already complex

						if(_bAsVector)
						{//a([]) = [C]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[i];
								m_pdblImg[_piSeqCoord[i] - 1]		= pInI[i];
							}
						}
						else
						{//a([],[]) = [C]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								m_pdblReal[iPos]	= pInR[i];
								m_pdblImg[iPos]		= pInI[i];
							}
						}
					}
					else
					{//a(?) = [R]
						double* pInR = pIn->real_get();

						if(_bAsVector)
						{//a([]) = [R]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								m_pdblReal[_piSeqCoord[i] - 1]	= pInR[i];
							}
						}
						else
						{//a([],[]) = [R]
							for(int i = 0 ; i < _iSeqCount ; i++)
							{
								int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
								int iTempR = i / pIn->cols_get();
								int iTempC = i % pIn->cols_get();
								int iNew_i = iTempR + iTempC * pIn->rows_get();

								m_pdblReal[iPos]	= pInR[iNew_i];
							}
						}
					}
				}
			break;
			}
		default :
			return false;
			break;
		}
		return true;
	}

	Double* Double::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Double* _poSource, bool _bAsVector)
	{
		Double* pdbl	= NULL ; 
		
		if(_bAsVector)
		{
			if(_poSource->cols_get() == 1)
			{
				pdbl = new Double(_piMaxDim[0], 1, _poSource->isComplex());
			}
			else if(_poSource->rows_get() == 1)
			{
				pdbl = new Double(1, _piMaxDim[0], _poSource->isComplex());
			}
			else
			{
				return NULL;
			}
		}
		else
		{
			pdbl = new Double(_piMaxDim[0], _piMaxDim[1], _poSource->isComplex());
		}

		pdbl->zero_set();
		if(pdbl->insert(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
		{
			delete pdbl;
			return NULL;
		}

		return pdbl;
	}

	Double* Double::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
	{
		Double *pOut	= NULL;
		int iRowsOut	= 0;
		int iColsOut	= 0;

		//check input param

		if(	(_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
		{
			return NULL;
		}

		if(_bAsVector)
		{//a([])
			if(rows_get() == 1)
			{
				iRowsOut	= 1;
				iColsOut	= _piDimSize[0];
			}
			else
			{
				iRowsOut	= _piDimSize[0];
				iColsOut	= 1;
			}
		}
		else
		{//a([],[])
			iRowsOut	= _piDimSize[0];
			iColsOut	= _piDimSize[1];
		}

		pOut							= new Double(iRowsOut, iColsOut, isComplex());
		double* pdblReal	= pOut->real_get();
		double* pdblImg		= pOut->img_get();


		if(_bAsVector)
		{
			if(isComplex())
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					pdblReal[i] = m_pdblReal[_piSeqCoord[i] - 1];
					pdblImg[i]	= m_pdblImg[_piSeqCoord[i] - 1];
				}
			}
			else
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					pdblReal[i] = m_pdblReal[_piSeqCoord[i] - 1];
				}
			}
		}
		else
		{
			if(isComplex())
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					int iOutIndex				= (i % iColsOut) * iRowsOut + (i / iColsOut);
					int iInIndex				= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
					pdblReal[iOutIndex] = m_pdblReal[iInIndex];
					pdblImg[iOutIndex]	= m_pdblImg[iInIndex];
				}
			}
			else
			{
				for(int i = 0 ; i < _iSeqCount ; i++)
				{
					//convert vertical indexes to horizontal indexes
					int iOutIndex				= (i % iColsOut) * iRowsOut + (i / iColsOut);
					int iInIndex				= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
					pdblReal[iOutIndex] = m_pdblReal[iInIndex];
				}
			}
		}
		
		return pOut;
	}
}
