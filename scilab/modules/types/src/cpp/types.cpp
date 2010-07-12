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
#include "types.hxx"

namespace types
{
	/*commun functions*/
	int GenericType::cols_get()
	{
		return m_iCols;
	}

	int GenericType::rows_get()
	{
		return m_iRows;
	}

	int GenericType::size_get()
	{
		return m_iSize;
	}

	wstring GenericType::DimToString()
	{
		wostringstream ostr;
		ostr << L"(" << rows_get() << L" x " << cols_get() << L")";
		return ostr.str();
	}

	bool GenericType::isIdentity(void)
	{
		if(m_iRows == -1 && m_iCols == -1)
		{
			return true;
		}
		return false;
	}
}
