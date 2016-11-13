
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
