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
#ifndef __GW_CORE__
#define __GW_CORE__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "c_gateway_prototype.h"
/*--------------------------------------------------------------------------*/
int gw_core(void);
/*--------------------------------------------------------------------------*/
C_GATEWAY_PROTOTYPE(C2F(sci_debug));
int C2F(sci_who)(char *fname, unsigned long fname_len);
//int C2F(sci_warning)(char *fname,unsigned long fname_len); -> C++
//int C2F(sci_mode)(char *fname, unsigned long fname_len); -> c++
//int C2F(sci_error)(char *fname,unsigned long fname_len); -> C++
int C2F(sci_resume)(char *fname, unsigned long fname_len);
int C2F(sci_return)(char *fname, unsigned long fname_len);
int C2F(sci_isdef)(char *fname, unsigned long fname_len);
int C2F(sci_errcatch)(char *fname, unsigned long fname_len);
//int C2F(sci_errclear)(char *fname, unsigned long fname_len); -> C++
int C2F(sci_iserror)(char *fname, unsigned long fname_len);
C_GATEWAY_PROTOTYPE(C2F(sci_predef));
int C2F(sci_clearfun)(char *fname, unsigned long fname_len);
int C2F(sci_funptr)(char *fname, unsigned long fname_len);
int C2F(sci_macr2lst)(char *fname, unsigned long fname_len);
int C2F(sci_setbpt)(char *fname, unsigned long fname_len);
int C2F(sci_delbpt)(char *fname, unsigned long fname_len);
int C2F(sci_dispbpt)(char *fname, unsigned long fname_len);
int C2F(sci_whereis)(char *fname, unsigned long fname_len);
int C2F(sci_where)(char *fname, void* pvApiCtx);
int C2F(sci_havewindow)(char *fname, unsigned long fname_len);
int C2F(sci_stacksize)(char *fname, unsigned long fname_len);
int C2F(sci_mtlb_mode)(char *fname, unsigned long fname_len);
int C2F(sci_what)(char *fname, unsigned long fname_len);
int C2F(sci_ieee)(char *fname, unsigned long fname_len);
int C2F(sci_gstacksize)(char *fname, unsigned long fname_len);
int C2F(sci_intppty)(char *fname, unsigned long fname_len);
int C2F(sci_macr2tree)(char *fname, unsigned long fname_len);
int C2F(sci_getos)(char *fname, unsigned long fname_len);
int C2F(sci_fromjava)(char *fname, unsigned long fname_len);
int C2F(sci_getmemory)(char *fname, unsigned long fname_len);
int C2F(sci_fromc)(char *fname, unsigned long fname_len);
//int C2F(sci_getmd5)(char *fname,unsigned long fname_len); -> C++
int C2F(sci_getmodules)(char *fname, unsigned long fname_len);
int C2F(sci_with_module)(char *fname, unsigned long fname_len);
int C2F(sci_getscilabmode)(char *fname, unsigned long fname_len);
int C2F(sci_librarieslist)(char *fname, unsigned long fname_len);
int C2F(sci_getvariablesonstack)(char *fname, unsigned long fname_len);
int C2F(sci_readgateway)(char *fname, unsigned long fname_len);
int C2F(sci_comp)(char *fname, unsigned long fname_len);

//C_GATEWAY_PROTOTYPE(sci_typename); -> c++
int C2F(sci_funcprot)(char *fname, void* pvApiCtx);
int C2F(sci_newfun)(char *fname, void* pvApiCtx);

//Yasp
C_GATEWAY_PROTOTYPE(sci_type);
C_GATEWAY_PROTOTYPE(sci_getdebuginfo);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_CORE__ */
/*--------------------------------------------------------------------------*/

