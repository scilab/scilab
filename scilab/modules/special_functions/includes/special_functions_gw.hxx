/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Sylvain Genin
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
