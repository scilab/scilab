/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __GW_SIGNAL__
#define __GW_SIGNAL__
/*--------------------------------------------------------------------------*/
#include "dynlib_signal_processing.h"
#include "dynlib_signal_processing_gw.h"
#include "machine.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
SIGNAL_PROCESSING_IMPEXP int gw_signal_processing(void);
/*--------------------------------------------------------------------------*/
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_remez);
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_amell);
EXTERN_SIGNAL_PROCESSING_GW STACK_GATEWAY_PROTOTYPE(sci_conv2);
#endif /*  __GW_SIGNAL__ */
/*--------------------------------------------------------------------------*/

