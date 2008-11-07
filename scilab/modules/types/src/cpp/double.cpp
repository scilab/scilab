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

using namespace std;

#define SIZE_BETWEEN_TWO_VALUES			2
#define SPACE_BETWEEN_TWO_VALUES		"  "
#define SIZE_BETWEEN_REAL_COMPLEX		1
#define SPACE_BETWEEN_REAL_COMPLEX	" "
#define SIGN_LENGTH									2
#define NO_SIGN											"  "
#define MINUS_STRING								"- "
#define PLUS_STRING									"+ "
#define SYMBOL_I										"i"

#define Max max
#define Min min

void GetFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint);
void Add_Value(ostringstream *_postr, double _dblVal, int _iWidth, int _iPrec);
void Add_Complex_Value(ostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec);
void Config_Stream(ostringstream *_postr, int _iWidth, int _iPrec, char _cFill);
void GetComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, 
											int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI);
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

		zero_set();
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
			memset(m_pdblReal, 0x00, m_iSize * sizeof(double));
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

	string Double::DimToString()
	{
		ostringstream ostr;
		ostr << "(" << rows_get() << " x " << cols_get() << ")";
		return ostr.str();
	}

	string Double::toString(int _iPrecision, int _iLineLen)
	{
		ostringstream ostr;
		//if(isComplex() == false)
		{
			/*Comment tenir compte de la longueur des lignes dans le formatage de variable ? */
			if(cols_get() == 1 && rows_get() == 1)
			{//scalar
				if(isComplex() == false)
				{
					int iWidth = 0, iPrec = 0;
					bool bFP = false; // FloatingPoint
					GetFormat(m_pdblReal[0], _iPrecision, &iWidth, &iPrec, &bFP);
					Add_Value(&ostr, m_pdblReal[0], iWidth, iPrec);
					ostr << endl;
				}
				else
				{//complex value
					int iWidthR = 0, iWidthI = 0;
					int iPrecR = 0, iPrecI = 0;
					bool bFPR = false, bFPI = false; // FloatingPoint
					GetFormat(m_pdblReal[0],	_iPrecision, &iWidthR, &iPrecR, &bFPR);
					GetFormat(m_pdblImg[0],		_iPrecision, &iWidthI, &iPrecI, &bFPI);
					Add_Complex_Value(&ostr, m_pdblReal[0], m_pdblImg[0], iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
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
						GetFormat(m_pdblReal[i], _iPrecision, &iWidth, &iPrec, &bFP);
						Add_Value(&ostr, m_pdblReal[i], iWidth, iPrec);
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
						GetFormat(m_pdblReal[i],	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetFormat(m_pdblImg[i],		_iPrecision, &iWidthI, &iPrecI, &bFPI);
						Add_Complex_Value(&ostr, m_pdblReal[i], m_pdblImg[i], iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
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
						GetFormat(m_pdblReal[i], _iPrecision, &iWidth, &iPrec, &bFP);
						iLen = iWidth + bFP + ostemp.str().size();
						if(iLen > _iLineLen)
						{//Max length, new line
							cout << endl << "       column " << iLastVal + 1 << " to " << i << endl << endl; 
							cout << ostemp.str() << endl;
							ostemp.str("\x00");
							iLastVal = i;
						}

						if(ostemp.str().size() != 0)
						{
							ostemp << SPACE_BETWEEN_TWO_VALUES;
						}

						Add_Value(&ostemp, m_pdblReal[i], iWidth, iPrec);
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
						GetFormat(m_pdblReal[i],	_iPrecision, &iWidthR, &iPrecR, &bFPR);
						GetFormat(m_pdblImg[i],		_iPrecision, &iWidthI, &iPrecI, &bFPI);

						iLen = ostemp.str().size();
						if(m_pdblImg[i] == 0)
						{
							if(m_pdblReal[i] == 0)
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
							if(m_pdblReal[i] == 0)
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

						Add_Complex_Value(&ostemp, m_pdblReal[i], m_pdblImg[i], iWidthR + iWidthI, iWidthR, iWidthI, Max(iPrecR, iPrecI));
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

							GetFormat(m_pdblReal[iCols1 * rows_get() + iRows1], _iPrecision, &iWidth, &iPrec, &bFP);
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
									GetFormat(m_pdblReal[iCols2 * rows_get() + iRows2], _iPrecision, &iWidth, &iPrec, &bFP);
									Add_Value(&ostemp, m_pdblReal[iCols2 * rows_get() + iRows2], piSize[iCols2], iPrec);
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
							int iWidth			= 0;
							int iPrec				= 0;
							bool bFP				= false; // FloatingPoint
							GetFormat(m_pdblReal[iCols2 * rows_get() + iRows2], _iPrecision, &iWidth, &iPrec, &bFP);
							Add_Value(&ostemp, m_pdblReal[iCols2 * rows_get() + iRows2], piSize[iCols2], iPrec);
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

							GetComplexFormat(	m_pdblReal[iCols1 * rows_get() + iRows1], m_pdblImg[iCols1 * rows_get() + iRows1], _iPrecision, 
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							if(iTotalWidth > piSize[iCols1])
							{
								piSize[iCols1] = iTotalWidth;
							}
						}

						if(iLen + piSize[iCols1] > _iLineLen)
						{//find the limit, print this part
							for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
							{
								ostr  << "|%" << piSize[iCols2] << "%|";

								for(int x = 0 ; x < piSize[iCols2]-1 ; x++)
								{
									ostr << " ";
								}
								ostr << SPACE_BETWEEN_REAL_COMPLEX << SYMBOL_I << "|" << SPACE_BETWEEN_TWO_VALUES;
							}

							for(int iRows2 = 0 ; iRows2 < rows_get() ; iRows2++)
							{
								for(int iCols2 = iLastCol ; iCols2 < iCols1 ; iCols2++)
								{
									int iWidthR = 0, iWidthI = 0, iTotalWidth = 0;
									int iPrecR = 0, iPrecI = 0;
									bool bFPR = false, bFPI = false; // FloatingPoint
									int iCurrentLen = 0;

									GetComplexFormat(	m_pdblReal[iCols2 * rows_get() + iRows2], m_pdblImg[iCols2 * rows_get() + iRows2], _iPrecision, 
										&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

									Add_Complex_Value(&ostemp, m_pdblReal[iCols2 * rows_get() + iRows2], m_pdblImg[iCols2 * rows_get() + iRows2], 
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

							GetComplexFormat(	m_pdblReal[iCols2 * rows_get() + iRows2], m_pdblImg[iCols2 * rows_get() + iRows2], _iPrecision, 
								&iTotalWidth, &iWidthR, &iWidthI, &iPrecR, &iPrecI, &bFPR, &bFPI);

							Add_Complex_Value(&ostemp, m_pdblReal[iCols2 * rows_get() + iRows2], m_pdblImg[iCols2 * rows_get() + iRows2], 
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
}

void GetFormat(double _dblVal, int _iPrecNeeded, int *_piWidth, int *_piPrec, bool* _pbFloatingPoint)
{
	double dblDec				= 0;
	double dblEnt				= 0;
	double dblAbs				= fabs(_dblVal);
	int iNbDigit				= 0;
	double dblTmp				= 0;
	int iNbEnt					= 0;
	int iNbDec					= 0;
	bool bStartByZero		= dblAbs < 1;

	dblDec				= modf(dblAbs, &dblEnt);
	*_pbFloatingPoint	= dblDec == 0 ? false : true;

	iNbDigit = ((int)log10(dblEnt + 0.4)) + 1;
	if(iNbDigit <= _iPrecNeeded - 2)
	{
		iNbEnt		= iNbDigit;
		iNbDigit	= _iPrecNeeded - iNbEnt;
		iNbDec		= 0;
		dblTmp		= dblDec * pow(10.0e+0, iNbDigit + 1);
		dblDec		= floor(dblTmp / 10.0e+0 + 0.5);

		if(dblDec != 0)
		{
			iNbDec = iNbDigit;
			while(fmod(dblDec, 10.0e+0) == 0)
			{
				iNbDec -= 1;
				dblDec = dblDec / 10.0e+0;
			}
		}
	}

	*_piPrec	= _iPrecNeeded - 1 - bStartByZero;
	*_piWidth = (_iPrecNeeded < (iNbEnt + *_piPrec) ? _iPrecNeeded : (iNbEnt + *_piPrec)) + SIGN_LENGTH;
}

void GetComplexFormat(double _dblR, double _dblI, int _iPrecNeeded, int *_piTotalWidth, int *_piWidthR, int *_piWidthI, int *_piPrecR,  int *_piPrecI, bool* _pbFloatingPointR,  bool* _pbFloatingPointI)
{
	GetFormat(_dblR, _iPrecNeeded, _piWidthR, _piPrecR, _pbFloatingPointR);
	GetFormat(_dblI, _iPrecNeeded, _piWidthI, _piPrecI, _pbFloatingPointI);

	*_piTotalWidth = 0;
	if(_dblI == 0)
	{
		if(_dblR == 0)
		{
			*_piTotalWidth += 1; //"0"
		}
		else
		{
			*_piTotalWidth		+= *_piWidthR + *_pbFloatingPointR;
			*_piWidthI	= 0;
		}
	}
	else
	{
		if(_dblR == 0)
		{
			*_piTotalWidth		+= *_piWidthI + *_pbFloatingPointI;
			*_piWidthR	= 0;
		}
		else
		{
			*_piTotalWidth += *_piWidthR + *_pbFloatingPointR;
			*_piTotalWidth += SIZE_BETWEEN_REAL_COMPLEX;
			*_piTotalWidth += *_piWidthI + *_pbFloatingPointI;
		}
	}
}

void Add_Value(ostringstream *_postr, double _dblVal, int _iWidth, int _iPrec)
{
	*_postr << (_dblVal < 0 ? MINUS_STRING : NO_SIGN);
	Config_Stream(_postr, _iWidth/* + SIZE_BETWEEN_TWO_VALUES*/, _iPrec, ' ');
	*_postr << left << fabs(_dblVal);
}

void Add_Complex_Value(ostringstream *_postr, double _dblR, double _dblI, int _iTotalWitdh, int _iWidthR, int _iWidthI, int _iPrec)
{
	ostringstream ostemp;
	/*
	if R && !C -> R
	if R && C -> R + Ci
	if !R && !C -> 0
	if(!R aa C	-> Ci
	*/

	*_postr << "|%" << _iTotalWitdh << "%|";
	if(_dblR == 0)
	{//no real part
		if(_dblI == 0)
		{//no imaginary part

			//0
			ostemp << left << 0;
		}
		else
		{//imaginary part

			//I
			ostemp << (_dblI < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthI, _iPrec, ' ');
			if(fabs(_dblI) == 1)
			{//specail case if I == 1 write only i and not 1i
				ostemp << left << SYMBOL_I;
			}
			else
			{
				ostemp << left << fabs(_dblI) << SYMBOL_I;
			}
		}
	}
	else
	{//real part
		if(_dblI == 0)
		{//no imaginary part

			//R
			ostemp << (_dblR < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthR, _iPrec, ' ');
			ostemp << left << fabs(_dblR);
		}
		else
		{//imaginary part

			//R
			ostemp << (_dblR < 0 ? MINUS_STRING : NO_SIGN);
			Config_Stream(&ostemp, _iWidthR, _iPrec, ' ');
			ostemp << left << fabs(_dblR) << SPACE_BETWEEN_REAL_COMPLEX;

			//I
			ostemp << (_dblI < 0 ? MINUS_STRING : PLUS_STRING);
			Config_Stream(&ostemp, _iWidthI, _iPrec, ' ');
			if(fabs(_dblI) == 1)
			{//specail case if I == 1 write only i and not 1i
				ostemp << left << SYMBOL_I;
			}
			else
			{
				ostemp << left << fabs(_dblI) << SYMBOL_I;
			}
		}
	}

	Config_Stream(_postr, _iTotalWitdh  + 2 * SIGN_LENGTH, _iPrec, ' ');
	*_postr << left << ostemp.str();

}

void Config_Stream(ostringstream *_postr, int _iWidth, int _iPrec, char _cFill)
{
	_postr->width(_iWidth);
	_postr->precision(_iPrec);
	_postr->fill(_cFill);
}
