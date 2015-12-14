/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_statistics_gw.h"
#include "dynlib_statistics.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
STATISTICS_IMPEXP int gw_statistics(void);
/*--------------------------------------------------------------------------*/

EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfbet);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfbin);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfchi);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfchn);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdff);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdffnc);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfgam);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfnbn);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfnor);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdfpoi);
EXTERN_STATISTICS_GW STACK_GATEWAY_PROTOTYPE(sci_cdft);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_STATISTICS__ */
/*--------------------------------------------------------------------------*/

