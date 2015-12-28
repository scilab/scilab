
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_DYNAMIC_LINK__
#define __GW_DYNAMIC_LINK__

#include "dynlib_dynamic_link_gw.h"
#include "c_gateway_prototype.h"

/*--------------------------------------------------------------------------*/
/* Declaration of all the profile function declared and */
/* used in sci_gateway */
/*--------------------------------------------------------------------------*/
DYNAMIC_LINK_GW_IMPEXP int sci_fort(char *fname, unsigned long fname_len);
DYNAMIC_LINK_GW_IMPEXP  C_GATEWAY_PROTOTYPE(sci_call);

#endif /* __GW_DYNAMIC_LINK__ */
/*--------------------------------------------------------------------------*/
