/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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
#ifndef __GW_CORE__
#define __GW_CORE__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
int gw_core(void);
/*--------------------------------------------------------------------------*/
STACK_GATEWAY_PROTOTYPE(C2F(sci_debug));
int C2F(sci_who)(char *fname, unsigned long fname_len);
//int C2F(sci_warning)(char *fname,unsigned long fname_len); -> C++
//int C2F(sci_mode)(char *fname, unsigned long fname_len); -> c++
//int C2F(sci_error)(char *fname,unsigned long fname_len); -> C++
int C2F(sci_resume)(char *fname, unsigned long fname_len);
int C2F(sci_return)(char *fname, unsigned long fname_len);
int C2F(sci_isdef)(char *fname, unsigned long fname_len);
//int C2F(sci_errclear)(char *fname, unsigned long fname_len); -> C++
STACK_GATEWAY_PROTOTYPE(C2F(sci_predef));
int C2F(sci_clearfun)(char *fname, unsigned long fname_len);
int C2F(sci_funptr)(char *fname, unsigned long fname_len);
//int C2F(sci_macr2lst)(char *fname, unsigned long fname_len);
int C2F(sci_whereis)(char *fname, unsigned long fname_len);
int C2F(sci_where)(char *fname, void* pvApiCtx);
//int C2F(sci_stacksize)(char *fname, unsigned long fname_len);
int C2F(sci_mtlb_mode)(char *fname, unsigned long fname_len);
int C2F(sci_what)(char *fname, unsigned long fname_len);
int C2F(sci_ieee)(char *fname, unsigned long fname_len);
int C2F(sci_getos)(char *fname, unsigned long fname_len);
int C2F(sci_fromjava)(char *fname, unsigned long fname_len);
int C2F(sci_getmemory)(char *fname, unsigned long fname_len);
int C2F(sci_fromc)(char *fname, unsigned long fname_len);
//int C2F(sci_getmd5)(char *fname,unsigned long fname_len); -> C++
int C2F(sci_getmodules)(char *fname, unsigned long fname_len);
int C2F(sci_with_module)(char *fname, unsigned long fname_len);
int C2F(sci_getscilabmode)(char *fname, unsigned long fname_len);
int C2F(sci_librarieslist)(char *fname, unsigned long fname_len);
//int C2F(sci_getvariablesonstack)(char *fname, unsigned long fname_len);

//STACK_GATEWAY_PROTOTYPE(sci_typename); -> c++
int C2F(sci_funcprot)(char *fname, void* pvApiCtx);
//int C2F(sci_newfun)(char *fname, void* pvApiCtx); -> C++

//Scilab 6
STACK_GATEWAY_PROTOTYPE(sci_getdebuginfo);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CORE__ */
/*--------------------------------------------------------------------------*/

