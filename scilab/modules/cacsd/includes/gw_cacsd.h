/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_CACSD__
#define __GW_CACSD__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_cacsd.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
// use in mex gateways
#define CSIZE 64000

#define CHECK_PARAM(CTX, POS) \
    if(checkParam(CTX, POS, fname)) return 0;

int checkParam(void* _pvCtx, int _iPos, char* fname);
char getIntegerValue(void* _pvCtx, int _iPos);

/*--------------------------------------------------------------------------*/
STACK_GATEWAY_PROTOTYPE(sci_contr);
STACK_GATEWAY_PROTOTYPE(sci_dhinf);
STACK_GATEWAY_PROTOTYPE(sci_hinf);
STACK_GATEWAY_PROTOTYPE(sci_linmeq);
STACK_GATEWAY_PROTOTYPE(sci_mucomp);
STACK_GATEWAY_PROTOTYPE(sci_rankqr);
STACK_GATEWAY_PROTOTYPE(sci_ricc);
STACK_GATEWAY_PROTOTYPE(sci_sident);
STACK_GATEWAY_PROTOTYPE(sci_sorder);
STACK_GATEWAY_PROTOTYPE(sci_findbd);

#endif /*  __GW_CACSD__ */
/*--------------------------------------------------------------------------*/

