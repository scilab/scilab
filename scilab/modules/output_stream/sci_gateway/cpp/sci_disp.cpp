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

using namespace types;

Function::ReturnValue sci_disp(typed_list &in, int _piRetCount, typed_list &out)
{
	typed_list::reverse_iterator it;

	if(in.empty())
	{
		YaspWrite("Pas bien pas de parametre\n");
		return Function::Error;
	}

	for(it = in.rbegin() ; it != in.rend() ; it++)
	{
		YaspWrite("\n");
		YaspWrite(const_cast<char*>((*it)->toString(16,75).c_str()));
	}

	YaspWrite("\n");
  return Function::OK;
}
