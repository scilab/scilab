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
#include "tostring_common.hxx"
#include "alltypes.hxx"

extern "C"
{
	#include "elem_common.h"
}

string printInLine(types::Poly* _pPoly, string _stVar, int _iPrecision, int _iLineLen);

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
		CreateImplicitList(-1, -1, -1);
	}

	ImplicitList::ImplicitList(double _iStart, double _iStep, double _iEnd)
	{
		CreateImplicitList(_iStart, _iStep, _iEnd);
	}

	ImplicitList::ImplicitList(InternalType* _poStart, InternalType* _poStep, InternalType* _poEnd)
	{
		if(_poStart->isDouble() && _poStep->isDouble() && _poEnd->isDouble())
		{
			CreateImplicitList(
						_poStart->getAsDouble()->real_get(0,0),
						_poStep->getAsDouble()->real_get(0,0),
						_poEnd->getAsDouble()->real_get(0,0));
		}
		else
		{
			if(_poStart->isDouble())
			{
				start_set(_poStart->getAsDouble()->real_get(0,0));
			}
			else if(_poStart->isPoly())
			{
				start_set((MatrixPoly*)_poStart);
			}

			if(_poStep->isDouble())
			{
				step_set(_poStep->getAsDouble()->real_get(0,0));
			}
			else if(_poStep->isPoly())
			{
				step_set((MatrixPoly*)_poStep);
			}

			if(_poEnd->isDouble())
			{
				end_set(_poEnd->getAsDouble()->real_get(0,0));
			}
			else if(_poEnd->isPoly())
			{
				end_set((MatrixPoly*)_poEnd);
			}
		}
	}

	ImplicitList::ImplicitList(double _iStart, double _iEnd)
	{
		CreateImplicitList(_iStart, 1, _iEnd);
	}

	void ImplicitList::CreateImplicitList(double _dblStart, double _dblStep, double _dblEnd)
	{
		m_dblStart	= _dblStart;
		m_dblStep		= _dblStep;
		m_dblEnd		= _dblEnd;
		m_iSize		= 0;

		m_eStartType = InternalType::RealDouble;
		m_eStepType = InternalType::RealDouble;
		m_eEndType = InternalType::RealDouble;
		compute();
	}

	double ImplicitList::start_get()
	{
		return m_dblStart;
	}

	MatrixPoly* ImplicitList::start_poly_get()
	{
		if(start_type_get() == InternalType::RealPoly)
		{
			return m_poStart;
		}
		return NULL;
	}

	void ImplicitList::start_set(double _dblStart)
	{
		m_dblStart		= _dblStart;
		m_eStartType	= InternalType::RealDouble; 
		compute();
	}

	void ImplicitList::start_set(MatrixPoly *_poPoly)
	{
		m_poStart			= _poPoly;
		m_eStartType	= InternalType::RealPoly; 
	}

	double ImplicitList::step_get()
	{
		return m_dblStep;
	}

	MatrixPoly* ImplicitList::step_poly_get()
	{
		if(step_type_get() == InternalType::RealPoly)
		{
			return m_poStep;
		}
		return NULL;
	}

	void ImplicitList::step_set(double _dblStep)
	{
		m_dblStep = _dblStep;
		m_eStepType	= InternalType::RealDouble; 
		compute();
	}

	void ImplicitList::step_set(MatrixPoly *_poPoly)
	{
		m_poStep = _poPoly;
		m_eStepType	= InternalType::RealPoly; 
	}

	double ImplicitList::end_get()
	{
		return m_dblEnd;
	}

	MatrixPoly* ImplicitList::end_poly_get()
	{
		if(end_type_get() == InternalType::RealPoly)
		{
			return m_poEnd;
		}
		return NULL;
	}

	void ImplicitList::end_set(double _dblEnd)
	{
		m_dblEnd = _dblEnd;
		m_eEndType	= InternalType::RealDouble; 
		compute();
	}

	void ImplicitList::end_set(MatrixPoly *_poPoly)
	{
		m_poEnd = _poPoly;
		m_eEndType	= InternalType::RealPoly; 
	}

	int ImplicitList::size_get()
	{
		//compute size of the result matrix
		return m_iSize;
	}

	void ImplicitList::extract_matrix(double *_pData)
	{
		if(_pData != NULL)
		{
			for(int i = 0 ; i < m_iSize ; i++)
			{
				_pData[i]	= m_dblStart + i * m_dblStep;
			}
		}
	}

	double ImplicitList::extract_value(int _iOccur)
	{
		return (m_dblStart + _iOccur * m_dblStep);
	}

	void ImplicitList::compute()
	{
		if(computable() == true)
		{
			m_iSize = 0;
			if(m_dblStep > 0)
			{
				double dblTemp = m_dblStart;
				while(dblTemp <= m_dblEnd)
				{
					m_iSize++;
					dblTemp += m_dblStep;
				}
			}
			else if(m_dblStep < 0)
			{
				double dblTemp = m_dblEnd;
				while(dblTemp <= m_dblStart)
				{
					m_iSize++;
					dblTemp -= m_dblStep;
				}
			}
			else
			{//Houston
			}

/*			m_iSize		= (int)(((m_dblEnd - m_dblStart) / m_dblStep) + 0.5);
			//for bad case
			m_iSize		= m_iSize < 0 ? 0 : m_iSize;
*/
		}
	}

	InternalType::RealType ImplicitList::start_type_get()
	{
		return m_eStartType;
	}

	InternalType::RealType ImplicitList::step_type_get()
	{
		return m_eStepType;
	}

	InternalType::RealType ImplicitList::end_type_get()
	{
		return m_eEndType;
	}

	bool ImplicitList::computable()
	{
		if(	m_eStartType == InternalType::RealDouble &&
				m_eStepType == InternalType::RealDouble &&
				m_eEndType == InternalType::RealDouble)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	string ImplicitList::toString(int _iPrecision, int _iLineLen)
	{
		if(computable())
		{
			Double dbl(1, size_get());
			for(int i = 0 ; i < dbl.size_get() ; i++)
			{
				dbl.val_set(0, i, extract_value(i));
			}
			return dbl.toString(_iPrecision, _iLineLen);
		}
		else
		{
			ostringstream ostr;
			ostr << " ";
			if(m_eStartType == RealDouble)
			{
				int iWidth = 0, iPrec = 0;
				bool bFP = false; // FloatingPoint
				GetDoubleFormat(m_dblStart, _iPrecision, &iWidth, &iPrec, &bFP);
				AddDoubleValue(&ostr, m_dblStart, iWidth, iPrec, false, true, false);
			}
			else
			{
				ostr << printInLine(m_poStart->poly_get(0,0), m_poStart->var_get(), _iPrecision, _iLineLen);
			}

			ostr << ":";

			if(m_eStepType == RealDouble)
			{
				int iWidth = 0, iPrec = 0;
				bool bFP = false; // FloatingPoint
				GetDoubleFormat(m_dblStep, _iPrecision, &iWidth, &iPrec, &bFP);
				AddDoubleValue(&ostr, m_dblStep, iWidth, iPrec, false, true, false);
			}
			else
			{
				ostr << printInLine(m_poStep->poly_get(0,0), m_poStep->var_get(), _iPrecision, _iLineLen);
			}

			ostr << ":";

			if(m_eEndType == RealDouble)
			{
				int iWidth = 0, iPrec = 0;
				bool bFP = false; // FloatingPoint
				GetDoubleFormat(m_dblEnd, _iPrecision, &iWidth, &iPrec, &bFP);
				AddDoubleValue(&ostr, m_dblEnd, iWidth, iPrec, false, true, false);
			}
			else
			{
				ostr << printInLine(m_poEnd->poly_get(0,0), m_poEnd->var_get(), _iPrecision, _iLineLen);
			}
			return ostr.str();
		}
	}
}

string printInLine(types::Poly* _pPoly, string _stVar, int _iPrecision, int _iLineLen)
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
