/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
* 
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
* 
*/

#include <sstream>
#include <cstdio>

#include "listdelete.hxx"
#include "localization.h"

namespace types
{
	ListDelete::~ListDelete()
	{
	}

	void ListDelete::whoAmI()
	{
		std::cout << "types::ListDelete";
	}

	ListDelete* ListDelete::clone(void)
	{
		return new ListDelete();
	}

	ListDelete* ListDelete::getAsListDelete(void)
	{
		return this; 
	}

	InternalType::RealType ListDelete::getType(void)
	{ 
		return RealListDeleteOperation; 
	}

	wstring ListDelete::toString(int _iPrecision, int _iLineLen)
	{
	  wostringstream ostr;

	  //FIXME : Implement me.
	  ostr << L"FIXME : Implement ListDelete::toString" << std::endl;

	  return ostr.str();
	}
}
