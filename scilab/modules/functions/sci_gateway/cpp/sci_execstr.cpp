/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Antoine ELIAS
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "functions_gw.hxx"
#include "setenvvar.hxx"


extern "C"
{
#ifndef _MSC_VER
#include "stricmp.h"
#endif
#include "cluni0.h"
#include "PATH_MAX.h"
}

using namespace types;
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_execstr(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	if(in.size() < 1 || in.size() > 3)
	{
		return Function::Error;
	}


	return Function::OK;
}
/*--------------------------------------------------------------------------*/
