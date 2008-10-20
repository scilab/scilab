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
#include "implicitlist.hxx"

namespace types
{
	ImplicitList::ImplicitList()
	{
		CreateImplicitList(-1, -1, -1);
	}

	ImplicitList::ImplicitList(double _iStart, double _iStep, double _iEnd)
	{
		CreateImplicitList(_iStart, _iStep, _iEnd);
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
		m_iSize		= (int)(((m_dblEnd - m_dblStart) / m_dblStep) + 1.5);
		//for bad case
		m_iSize		= m_iSize < 0 ? 0 : m_iSize;
	}

	double ImplicitList::start_get()
	{
		return m_dblStart;
	}

	void ImplicitList::start_set(double _dblStart)
	{
		m_dblStart = _dblStart;
	}

	double ImplicitList::step_get()
	{
		return m_dblStep;
	}

	void ImplicitList::step_set(double _dblStep)
	{
		m_dblStep = _dblStep;
	}

	double ImplicitList::end_get()
	{
		return m_dblEnd;
	}

	void ImplicitList::end_set(double _dblEnd)
	{
		m_dblEnd = _dblEnd;
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

}
