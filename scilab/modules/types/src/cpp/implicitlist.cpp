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
#include "implicitlist.hxx"
#include "core_math.h"
#include "double.hxx"
#include "int.hxx"
#include "tostring_common.hxx"
#include "alltypes.hxx"

extern "C"
{
	#include "elem_common.h"
}

string printInLinePoly(types::Poly* _pPoly, string _stVar, int _iPrecision, int _iLineLen);
string printDouble(types::Double* _pD, int _iPrecision, int _iLineLen);
long long convert_input(types::InternalType* _poIT);
unsigned long long convert_unsigned_input(types::InternalType* _poIT);

namespace types
{
	ImplicitList::~ImplicitList()
	{
		if(isDeletable() == true)
		{
		}
	}

	ImplicitList::ImplicitList()
	{
	}

	ImplicitList::ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd)
	{
		m_iSize = -1;
		m_eOutSubType = Int::Type8;
		m_eOutType = RealGeneric;
		start_set(_poStart);
		step_set(_poStep);
		end_set(_poEnd);
		compute();
	}

	  ImplicitList::ImplicitList* ImplicitList::clone()
	  {
	    //FIXME : Implement me.
	    return NULL;
	  }

	InternalType::RealType ImplicitList::start_type_get()
	{
		return m_poStart->getType();
	}

	InternalType::RealType ImplicitList::step_type_get()
	{
		return m_poStep->getType();
	}

	InternalType::RealType ImplicitList::end_type_get()
	{
		return m_poEnd->getType();
	}

	InternalType* ImplicitList::start_get()
	{
		return m_poStart;
	}

	void ImplicitList::start_set(InternalType *_poIT)
	{
		m_poStart = _poIT;
		if(m_poStart != NULL)
		{
			m_eStartType = m_poStart->getType();
		}
	}

	InternalType* ImplicitList::step_get()
	{
		return m_poStep;
	}

	void ImplicitList::step_set(InternalType *_poIT)
	{
		m_poStep = _poIT;
		if(m_poStep != NULL)
		{
			m_eStepType = m_poStep->getType();
		}
	}

	InternalType* ImplicitList::end_get()
	{
		return m_poEnd;
	}

	void ImplicitList::end_set(InternalType* _poIT)
	{
		m_poEnd = _poIT;
		if(m_poEnd != NULL)
		{
			m_eEndType = m_poEnd->getType();
		}
	}

	int ImplicitList::size_get()
	{
		return m_iSize;
	}

	bool ImplicitList::compute()
	{
		m_iSize = -1;
		if(computable() == true)
		{
			m_iSize = 0;
			if(m_eOutType == RealDouble)
			{
				double dblStart	= m_poStart->getAsDouble()->real_get(0,0);
				double dblStep	= m_poStep->getAsDouble()->real_get(0,0);
				double dblEnd		= m_poEnd->getAsDouble()->real_get(0,0);

				if(dblStep > 0)
				{
					double dblTemp = dblStart;
					while(dblTemp <= dblEnd)
					{
						m_iSize++;
						dblTemp += dblStep;
					}
				}
				else if(dblStep < 0)
				{
					double dblTemp = dblEnd;
					while(dblTemp <= dblStart)
					{
						m_iSize++;
						dblTemp -= dblStep;
					}
				}
			}
			else //m_eOutType == RealInt
			{
				if(m_eOutSubType > Int::Type64)//Unsigned
				{
					unsigned long long ullStart = convert_unsigned_input(m_poStart);
					unsigned long long ullStep	= convert_unsigned_input(m_poStep);
					unsigned long long ullEnd		= convert_unsigned_input(m_poEnd);

					if(ullStep > 0)
					{
						unsigned long long ullTemp = ullStart;
						while(ullTemp <= ullEnd)
						{
							m_iSize++;
							ullTemp += ullStep;
						}
					}
					else if(ullStep < 0) //Signed
					{
						unsigned long long ullTemp = ullEnd;
						while(ullTemp <= ullStart)
						{
							m_iSize++;
							ullTemp -= ullStep;
						}
					}
				}
				else //Signed
				{
					long long llStart = convert_input(m_poStart);
					long long llStep	= convert_input(m_poStep);
					long long llEnd		= convert_input(m_poEnd);

					if(llStep > 0)
					{
						long long llTemp = llStart;
						while(llTemp <= llEnd)
						{
							m_iSize++;
							llTemp += llStep;
						}
					}
					else if(llStep < 0)
					{
						long long llTemp = llEnd;
						while(llTemp <= llStart)
						{
							m_iSize++;
							llTemp -= llStep;
						}
					}
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ImplicitList::computable()
	{
		if(m_eStartType != RealDouble && m_eStartType != RealInt)
		{
			return false;
		}

		if(m_eStepType != RealDouble && m_eStepType != RealInt)
		{
			return false;
		}

		if(m_eEndType != RealDouble && m_eEndType != RealInt)
		{
			return false;
		}

		//"compute" output type
		m_eOutType = RealGeneric; //not defined type
		if(m_eStartType == RealInt)
		{
			m_eOutType		= RealInt;
			m_eOutSubType = m_poStart->getAsInt()->getIntType();
		}
		else if(m_eStepType == RealInt)
		{
			m_eOutType		= RealInt;
			m_eOutSubType = m_poEnd->getAsInt()->getIntType();
		}
		else if(m_eEndType == RealInt)
		{
			m_eOutType		= RealInt;
			m_eOutSubType = m_poEnd->getAsInt()->getIntType();
		}
		else
		{
			m_eOutType		= RealDouble;
		}

		return true;
	}

	string ImplicitList::toString(int _iPrecision, int _iLineLen)
	{
		if(computable())
		{
			return extract_matrix()->toString(_iPrecision, _iLineLen);
		}
		else
		{
			ostringstream ostr;
			ostr << " ";
			if(m_eStartType == RealDouble)
			{
				Double *pD = m_poStart->getAsDouble();
				ostr << printDouble(pD, _iPrecision, _iLineLen);
			}
			else //MatrixPoly
			{
				MatrixPoly* pMP = m_poStart->getAsPoly();
				ostr << printInLinePoly(pMP->poly_get(0,0), pMP->var_get(), _iPrecision, _iLineLen);
			}

			ostr << ":";

			if(m_eStepType == RealDouble)
			{
				Double *pD = m_poStep->getAsDouble();
				ostr << printDouble(pD, _iPrecision, _iLineLen);
			}
			else //MatrixPoly
			{
				MatrixPoly* pMP = m_poStep->getAsPoly();
				ostr << printInLinePoly(pMP->poly_get(0,0), pMP->var_get(), _iPrecision, _iLineLen);
			}

			ostr << ":";

			if(m_eEndType == RealDouble)
			{
				Double *pD = m_poEnd->getAsDouble();
				ostr << printDouble(pD, _iPrecision, _iLineLen);
			}
			else //MatrixPoly
			{
				MatrixPoly* pMP = m_poEnd->getAsPoly();
				ostr << printInLinePoly(pMP->poly_get(0,0), pMP->var_get(), _iPrecision, _iLineLen);
			}
			ostr << endl;
			return ostr.str();
		}
	}

	InternalType::RealType ImplicitList::getOutputType()
	{
		return m_eOutType;
	}

	Int::IntType ImplicitList::getOutputSubType()
	{
		return m_eOutSubType;
	}

	//extract single value in a InternalType
	InternalType* ImplicitList::extract_value(int _iOccur)
	{
		InternalType* pIT = NULL;
		if(compute())
		{
			if(m_eOutType == RealInt)
			{
				Int *pI	= NULL;
				pI = Int::createInt(1,1, m_eOutSubType);
				pI->data_set(0,0, convert_input(m_poStart) + _iOccur * convert_input(m_poStep));
				pIT	= pI;
			}
			else //RealDouble
			{
				double dblStart		= m_poStart->getAsDouble()->real_get(0,0);
				double dblStep		= m_poStep->getAsDouble()->real_get(0,0);
				Double* pD				= new Double(dblStart + _iOccur * dblStep);
				pIT = pD;
			}
		}
		return pIT;
	}

	//extract matrix in a Internaltype
	InternalType* ImplicitList::extract_matrix()
	{
		InternalType* pIT = NULL;
		if(compute())
		{
			if(m_eOutType == RealInt)
			{
				Int *pI	= Int::createInt(1, m_iSize, m_eOutSubType);

				for(int i = 0 ; i < m_iSize ; i++)
				{
					pI->data_set(i, convert_input(m_poStart) + convert_input(m_poStep) * i);
				}

				pIT	= pI;
			}
			else //RealDouble
			{
				Double* pD				= new Double(1, m_iSize);
				extract_matrix(pD->real_get());
				pIT = pD;
			}
		}
		return pIT;
	}

	//extract double array
	void ImplicitList::extract_matrix(double *_pData)
	{
		if(m_eOutType == RealDouble)
		{
			if(m_iSize > 0)
			{
				double dblStart = m_poStart->getAsDouble()->real_get()[0];
				double dblStep	= m_poStep->getAsDouble()->real_get()[0];

				_pData[0] = dblStart;
				for(int i = 1 ; i < m_iSize ; i++)
				{
					_pData[i] = _pData[i - 1] + dblStep;
				}
			}
		}
	}

	//extract integer  array
	void ImplicitList::extract_matrix(char *_pc)
	{
		char cStart = (char)convert_input(m_poStart);
		char cStep	= (char)convert_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pc[i] = cStart + cStep * i;
		}
	}

	void ImplicitList::extract_matrix(short *_ps)
	{
		short sStart	= (short)convert_input(m_poStart);
		short sStep		= (short)convert_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_ps[i] = sStart + sStep * i;
		}
	}

	void ImplicitList::extract_matrix(int *_pi)
	{
		int iStart	= (int)convert_input(m_poStart);
		int iStep		= (int)convert_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pi[i] = iStart + iStep * i;
		}
	}

	void ImplicitList::extract_matrix(long long *_pll)
	{
		long long llStart	= convert_input(m_poStart);
		long long llStep		= convert_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pll[i] = llStart + llStep * i;
		}
	}

	//extract unsigned integer
	void ImplicitList::extract_matrix(unsigned char *_puc)
	{
		unsigned char ucStart	= (unsigned char)convert_unsigned_input(m_poStart);
		unsigned char ucStep	= (unsigned char)convert_unsigned_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_puc[i] = ucStart + ucStep * i;
		}
	}

	void ImplicitList::extract_matrix(unsigned short *_pus)
	{
		unsigned short usStart	= (unsigned short)convert_unsigned_input(m_poStart);
		unsigned short usStep		= (unsigned short)convert_unsigned_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pus[i] = usStart + usStep * i;
		}
	}

	void ImplicitList::extract_matrix(unsigned int *_pui)
	{
		unsigned int uiStart	= (unsigned int)convert_unsigned_input(m_poStart);
		unsigned int uiStep		= (unsigned int)convert_unsigned_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pui[i] = uiStart + uiStep * i;
		}
	}

	void ImplicitList::extract_matrix(unsigned long long *_pull)
	{
		unsigned long long ullStart	= convert_unsigned_input(m_poStart);
		unsigned long long ullStep	= convert_unsigned_input(m_poStep);

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pull[i] = ullStart + ullStep * i;
		}
	}
}

string printInLinePoly(types::Poly* _pPoly, string _stVar, int _iPrecision, int _iLineLen)
{
	ostringstream ostr;
	for(int i = 0 ; i < _pPoly->rank_get() ; i++)
	{
		double dbl = _pPoly->coef_get()->real_get()[i];
		if(dbl != 0)
		{
			int iWidth = 0, iPrec = 0;
			bool bFP = false; // FloatingPoint
			GetDoubleFormat(dbl, _iPrecision, &iWidth, &iPrec, &bFP);
			AddDoubleValue(&ostr, dbl, iWidth, iPrec, ostr.str().size() != 0, i == 0, false);
			if(i != 0)
			{
				ostr <<_stVar;
				if(i > 1)
				{
					ostr << "^" << i;
				}
			}
		}
	}
	return ostr.str();
}

string printDouble(types::Double* _pD, int _iPrecision, int _iLineLen)
{
	ostringstream ostr;
	int iWidth = 0, iPrec = 0;
	bool bFP = false; // FloatingPoint
	GetDoubleFormat(_pD->real_get(0,0), _iPrecision, &iWidth, &iPrec, &bFP);
	AddDoubleValue(&ostr, _pD->real_get(0,0), iWidth, iPrec, false, true, false);
	return ostr.str();
}

long long convert_input(types::InternalType* _poIT)
{
	long long llValue = 0;
	switch(_poIT->getType())
	{
	case types::GenericType::RealDouble :
		llValue = (long long)_poIT->getAsDouble()->real_get(0,0);
		break;
	case types::GenericType::RealInt :
			llValue = (long long)_poIT->getAsInt()->data_get(0,0);
		break;
	}
	return llValue;
}

unsigned long long convert_unsigned_input(types::InternalType* _poIT)
{
	unsigned long long ullValue = 0;
	switch(_poIT->getType())
	{
	case types::GenericType::RealDouble :
		ullValue = (unsigned long long)_poIT->getAsDouble()->real_get(0,0);
		break;
	case types::GenericType::RealInt :
		ullValue = (unsigned long long)_poIT->getAsInt()->data_get(0,0);
		break;
	}
	return ullValue;
}
