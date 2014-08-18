/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SPEC_FUNC_GW_HXX__
#define __SPEC_FUNC_GW_HXX__

#include "cpp_gateway_prototype.hxx"

extern "C"
{
#include "dynlib_special_functions.h"
}

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_gamma, SPECIAL_FUNCTIONS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_lgamma, SPECIAL_FUNCTIONS_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_dlgamma, SPECIAL_FUNCTIONS_IMPEXP);

#endif /* !__SPEC_FUNC_GW_HXX__ */
