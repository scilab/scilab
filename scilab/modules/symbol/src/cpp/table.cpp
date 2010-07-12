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

#include "table.hxx"

namespace symbol
{
	void	Table::print (std::wostream& ostr) const
		{
			std::list<Scope*>::const_iterator it_list_scope;
			std::list<Scope*>::size_type	i;

			for (i = (*this).l_scope.size() - 1, it_list_scope = (*this).l_scope.begin() ; it_list_scope != (*this).l_scope.end(); --i, ++it_list_scope)
			{
				ostr << L"Scope level: " << i << std::endl;
				ostr << *(*it_list_scope);
			}
		}
}

