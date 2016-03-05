/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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

