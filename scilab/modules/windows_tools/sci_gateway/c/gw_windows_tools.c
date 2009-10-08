/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
{sci_winopen,"winopen"},
{sci_winqueryreg,"winqueryreg"},
{sci_findfileassociation,"findfileassociation"},
{sci_dos,"dos"},
{sci_mcisendstring,"mcisendstring"},
{sci_oemtochar,"oemtochar"},
{sci_chartooem,"chartooem"},
{sci_consolebox,"consolebox"},
{sci_win64,"win64"},
{sci_istssession,"istssession"},
{sci_getsystemmetrics,"getsystemmetrics"},
{sci_createGUID,"createGUID"}
};
/*--------------------------------------------------------------------------*/
int gw_windows_tools(void)
{  
	Rhs = Max(0, Rhs);

	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrErr));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
