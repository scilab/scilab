/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GW_CORE__
#define __GW_CORE__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int C2F(gw_core)(void);
/*--------------------------------------------------------------------------*/ 
int C2F(sci_debug) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_who) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_warning) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_argn) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_comp) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mode) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_type) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_error) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_resume) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_return) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_format) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isdef) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_exists) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_errcatch) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_errclear) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_iserror) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_predef) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_newfun) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clearfun) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_funptr) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_macr2lst) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_setbpt) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_delbpt) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_dispbpt) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_funcprot) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_whereis) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_where) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_havewindow) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_stacksize) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_mtlb_mode) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clear) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_what) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_sciargs) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_chdir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getcwd) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_ieee) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_typename) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_global) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_clearglobal) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isglobal) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_gstacksize) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_intppty) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_lasterror) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getversion) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_macr2tree) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getos) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_banner) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fromjava) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getmemory) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fromc) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getmd5) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getmodules) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_with_module) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_islittleendian) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getscilabmode) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_librarieslist) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_libraryinfo) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getvariablesonstack) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getdebuginfo) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /*  __GW_CORE__ */
/*--------------------------------------------------------------------------*/

