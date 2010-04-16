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
#include "tostring_common.hxx"

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
			if(m_poStart)
			{
				delete m_poStart;
			}

			if(m_poStep)
			{
				delete m_poStep;
			}

			if(m_poEnd)
			{
				delete m_poEnd;
			}
		}
	}

	ImplicitList::ImplicitList()
	{
		m_bComputed = false;
	}

	ImplicitList::ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd)
	{
		m_iSize = -1;
		m_eOutSubType = Int::Type8;
		m_eOutType = RealGeneric;
		start_set(_poStart->clone());
		step_set(_poStep->clone());
		end_set(_poEnd->clone());
		compute();
	}

	ImplicitList* ImplicitList::clone()
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
		m_bComputed = false;
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
		m_bComputed = false;
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
		m_bComputed = false;
	}

	long long ImplicitList::size_get()
	{
		return m_iSize;
	}

	bool ImplicitList::compute()
	{
		if(m_bComputed == true)
		{
			return true;
		}

		m_iSize = -1;
		if(computable() == true)
		{
			m_iSize = 0;
			if(m_eOutType == RealDouble)
			{
				double dblStart	= m_poStart->getAsDouble()->real_get(0,0);
				double dblStep	= m_poStep->getAsDouble()->real_get(0,0);
				double dblEnd		= m_poEnd->getAsDouble()->real_get(0,0);

				m_iSize = static_cast<long long>(floor(fabs(dblEnd - dblStart) / fabs(dblStep))) + 1;
			}
			else //m_eOutType == RealInt
			{
				if(m_eOutSubType > Int::Type64)//Unsigned
				{
					unsigned long long ullStart = convert_unsigned_input(m_poStart);
					unsigned long long ullStep	= convert_unsigned_input(m_poStep);
					unsigned long long ullEnd		= convert_unsigned_input(m_poEnd);

#ifdef _MSC_VER
					m_iSize = static_cast<long long>(floor(static_cast<double>(_abs64(ullEnd - ullStart) / _abs64(ullStep)) )) + 1;
#else
					m_iSize = static_cast<long long>(floor(static_cast<double>(llabs(ullEnd - ullStart) / llabs(ullStep)) )) + 1;
#endif
				}
				else //Signed
				{
					long long llStart = convert_input(m_poStart);
					long long llStep	= convert_input(m_poStep);
					long long llEnd		= convert_input(m_poEnd);

#ifdef _MSC_VER
					m_iSize = static_cast<long long>(floor( static_cast<double>(_abs64(llEnd - llStart) / _abs64(llStep)) )) + 1;
#else
					m_iSize = static_cast<long long>(floor( static_cast<double>(llabs(llEnd - llStart) / llabs(llStep)) )) + 1;
#endif
				}
			}
			m_bComputed = true;
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

	double ImplicitList::extract_value_double(int _iOccur)
	{
		double dblStart		= m_poStart->getAsDouble()->real_get(0,0);
		double dblStep		= m_poStep->getAsDouble()->real_get(0,0);
		return dblStart + _iOccur * dblStep;
	}

	long long ImplicitList::extract_value_int(int _iOccur)
	{
		return convert_input(m_poStart) + _iOccur * convert_input(m_poStep);
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

    template<typename T>
	void ImplicitList::extract_matrix(T *_pT)
	{
        T tStart = static_cast<T>(convert_input(m_poStart));
		T tStep	= static_cast<T>(convert_input(m_poStep));

		for(int i = 0 ; i < m_iSize ; i++)
		{
			_pT[i] = tStart + tStep * i;
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
		llValue = static_cast<long long>(_poIT->getAsDouble()->real_get(0,0));
		break;
	case types::GenericType::RealInt :
        llValue = static_cast<long long>(_poIT->getAsInt()->data_get(0,0));
		break;
    default:
        // FIXME : Trigger an error ??
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
		ullValue = static_cast<unsigned long long>(_poIT->getAsDouble()->real_get(0,0));
		break;
	case types::GenericType::RealInt :
		ullValue = static_cast<unsigned long long>(_poIT->getAsInt()->data_get(0,0));
		break;
    default:
        // FIXME : Trigger an error ??
        break;
	}
	return ullValue;
}
