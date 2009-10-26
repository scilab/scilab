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
#include <string.h>
#include "gw_core.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_debug),"debug"},
{C2F(sci_who),"who"},
{C2F(sci_warning),"warning"},
{C2F(sci_argn),"argn"},
{C2F(sci_getvariablesonstack),"getvariablesonstack"},
{C2F(sci_comp),"comp"},
{C2F(sci_getscilabmode),"getscilabmode"},
{C2F(sci_mode),"mode"},
{C2F(sci_type),"type"},
{C2F(sci_error),"error"},
{C2F(sci_resume),"resume"},
{C2F(sci_return),"return"},
{C2F(sci_format),"format"},
{C2F(sci_isdef),"isdef"},
{C2F(sci_exists),"exists"},
{C2F(sci_errcatch),"errcatch"},
{C2F(sci_errclear),"errclear"},
{C2F(sci_iserror),"iserror"},
{C2F(sci_predef),"predef"},
{C2F(sci_newfun),"newfun"},
{C2F(sci_clearfun),"clearfun"},
{C2F(sci_funptr),"funptr"},
{C2F(sci_macr2lst),"macr2lst"},
{C2F(sci_setbpt),"setbpt"},
{C2F(sci_delbpt),"delbpt"},
{C2F(sci_dispbpt),"dispbpt"},
{C2F(sci_funcprot),"funcprot"},
{C2F(sci_where),"where"},
{C2F(sci_havewindow),"havewindow"},
{C2F(sci_stacksize),"stacksize"},
{C2F(sci_mtlb_mode),"mtlb_mode"},
{C2F(sci_clear),"clear"},
{C2F(sci_what),"what"},
{C2F(sci_sciargs),"sciargs"},
{C2F(sci_ieee),"ieee"},
{C2F(sci_typename),"typename"},
{C2F(sci_global),"global"},
{C2F(sci_clearglobal),"clearglobal"},
{C2F(sci_isglobal),"isglobal"},
{C2F(sci_gstacksize),"gstacksize"},
{C2F(sci_intppty),"intppty"},
{C2F(sci_lasterror),"lasterror"},
{C2F(sci_getversion),"getversion"},
{C2F(sci_macr2tree),"macr2tree"},
{C2F(sci_getos),"getos"},
{C2F(sci_banner),"banner"},
{C2F(sci_getmemory),"getmemory"},
{C2F(sci_getmd5),"getmd5"},
{C2F(sci_getmodules),"getmodules"},
{C2F(sci_with_module),"with_module"},
{C2F(sci_getdebuginfo),"getdebuginfo"},
{C2F(sci_readgateway),"readgateway"},
{C2F(sci_exit),"exit"}
};
/*--------------------------------------------------------------------------*/
int gw_core(void)
{  
	Rhs = Max(0, Rhs);

	/* recursion */
	/* getf, deff, exec, execstr (functions module) call comp by "recursion" */
    /* comp can not be in same gateway that others of functions module */
	if ( isRecursionCallToFunction() )
	{
		if ( (getRecursionGatewayToCall() == GW_CORE_ID ) &&
			 (getRecursionFunctionToCall() == RECURSION_CALL_COMP) )
		{
			Fin = 6;
		}
	}
	
	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrErr));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
