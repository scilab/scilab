
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_STRING__
#define __GW_STRING__
#include "dynlib_string.h"
#include "dynlib_string_gw.h"
#include "c_gateway_prototype.h"

/*--------------------------------------------------------------------------*/
STRING_IMPEXP int gw_string(void);
/*--------------------------------------------------------------------------*/
EXTERN_STRING_GW C_GATEWAY_PROTOTYPE(sci_strsplit);
EXTERN_STRING_GW C_GATEWAY_PROTOTYPE(sci_isdigit);
EXTERN_STRING_GW C_GATEWAY_PROTOTYPE(sci_isalphanum);
EXTERN_STRING_GW C_GATEWAY_PROTOTYPE(sci_isascii);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_STRING__ */
/*--------------------------------------------------------------------------*/
