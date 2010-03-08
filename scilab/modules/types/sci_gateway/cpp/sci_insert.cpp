/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

using namespace types;

Function::ReturnValue sci_insert(typed_list &in, int _piRetCount, typed_list &out)
{
	if(in.size() == 0)
	{
	  out.push_back(new ListInsert(new ListUndefined()));
	}
	else
	{
		out.push_back(new ListInsert(in[0]->clone()));
	}
  return Function::OK;
}
