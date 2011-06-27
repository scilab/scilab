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

#ifndef __FUNCTIONS_GW_HXX__
#define __FUNCTIONS_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#include "dynlib_functions_gw.h"

class FunctionsModule
{
private :
	FunctionsModule(){};
	~FunctionsModule(){};
public :
	FUNCTIONS_GW_IMPEXP static int Load();
};

CPP_GATEWAY_PROTOTYPE(sci_exec);
CPP_GATEWAY_PROTOTYPE(sci_execstr);
CPP_GATEWAY_PROTOTYPE(sci_funclist);
CPP_GATEWAY_PROTOTYPE(sci_whereis);
CPP_GATEWAY_PROTOTYPE(sci_getCurrentThreadId);
CPP_GATEWAY_PROTOTYPE(sci_mode);
CPP_GATEWAY_PROTOTYPE(sci_getThreads);

#endif /* __FUNCTIONS_GW_HXX__ */

