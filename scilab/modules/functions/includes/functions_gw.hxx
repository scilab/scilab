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

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_exec, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_execstr, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_funclist, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_whereis, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_getCurrentThreadId, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_mode, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_getThreads, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_macrovar, FUNCTIONS_GW_IMPEXP);

#endif /* __FUNCTIONS_GW_HXX__ */

