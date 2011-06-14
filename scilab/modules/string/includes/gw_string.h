
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
#include "c_gateway_prototype.h"

/*--------------------------------------------------------------------------*/
STRING_IMPEXP int gw_string(void);
/*--------------------------------------------------------------------------*/
//STRING_IMPEXP int sci_length(char *fname,unsigned long fname_len); -> C++
//STRING_IMPEXP int sci_string(char *fname,unsigned long fname_len); -> C++
//STRING_IMPEXP int sci_convstr(char *fname,unsigned long fname_len); -> C++
//STRING_IMPEXP int sci_NumTokens(char *fname,unsigned long fname_len);

//YaSp
STRING_IMPEXP C_GATEWAY_PROTOTYPE(sci_strsplit);
STRING_IMPEXP C_GATEWAY_PROTOTYPE(sci_isdigit);
STRING_IMPEXP C_GATEWAY_PROTOTYPE(sci_isalphanum);
STRING_IMPEXP C_GATEWAY_PROTOTYPE(sci_isascii);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_STRING__ */
/*--------------------------------------------------------------------------*/
