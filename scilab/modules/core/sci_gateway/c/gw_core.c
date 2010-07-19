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
{NULL, ""}, //argn
{C2F(sci_getvariablesonstack),"getvariablesonstack"},
{C2F(sci_comp),"comp"},
{NULL, ""}, //getscilabmode
{C2F(sci_mode),"mode"},
{NULL ,""}, //type
{C2F(sci_error),"error"},
{C2F(sci_resume),"resume"},
{C2F(sci_return),"return"},
{C2F(sci_format),"format"},
{NULL, ""}, // isdef
{NULL, ""}, //exists
{C2F(sci_errcatch),"errcatch"},
{C2F(sci_errclear),"errclear"},
{C2F(sci_iserror),"iserror"},
{C2F(sci_predef),"predef"},
{NULL,""}, //newfun
{C2F(sci_clearfun),"clearfun"},
{C2F(sci_funptr),"funptr"},
{C2F(sci_macr2lst),"macr2lst"},
{C2F(sci_setbpt),"setbpt"},
{C2F(sci_delbpt),"delbpt"},
{C2F(sci_dispbpt),"dispbpt"},
{NULL,""},//funcprot
{C2F(sci_where),"where"},
{C2F(sci_havewindow),"havewindow"},
{C2F(sci_stacksize),"stacksize"},
{C2F(sci_mtlb_mode),"mtlb_mode"},
{NULL,""}, // clear
{C2F(sci_what),"what"},
{NULL, ""}, //sciargs
{C2F(sci_ieee),"ieee"},
{NULL,""},//typename
{NULL, ""}, //global
{NULL, ""}, //clearglobal
{NULL, ""}, //isglobal
{C2F(sci_gstacksize),"gstacksize"},
{C2F(sci_intppty),"intppty"},
{C2F(sci_lasterror),"lasterror"},
{NULL, ""}, //getversion
{C2F(sci_macr2tree),"macr2tree"},
{NULL, ""}, //getos
{NULL, ""}, //banner
{C2F(sci_getmemory),"getmemory"},
{C2F(sci_getmd5),"getmd5"},
{NULL,""}, //getmodules
{C2F(sci_with_module),"with_module"},
{NULL, ""}, //getdebuginfo
{C2F(sci_readgateway),"readgateway"},
{NULL, ""} //exit
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
	
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
