/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - ELIAS Antoine
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "alltypes.hxx"
#include "funcmanager.hxx"
#include "yaspio.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

using namespace types;

Function::ReturnValue sci_disp(typed_list &in, int _piRetCount, typed_list &out)
{
	typed_list::reverse_iterator it;

	if(in.empty())
	{
		Scierror(999, _("%s: Wrong number of input arguments: At least %d expected.\n"), "disp", 1);
		return Function::Error;
	}

	for(it = in.rbegin() ; it != in.rend() ; it++)
	{
		YaspWriteW(L"\n");
		YaspWriteW((*it)->toString(ConfigVariable::getFormat(), ConfigVariable::getConsoleWidth()).c_str());
	}

	YaspWriteW(L"\n");
  return Function::OK;
}
