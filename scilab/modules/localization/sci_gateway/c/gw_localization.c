/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_localization.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]=
{
	{C2F(sci_setlanguage),"setlanguage"},
	{C2F(sci_getlanguage),"getlanguage"},
	{C2F(sci_getdefaultlanguage),"getdefaultlanguage"},
	{C2F(sci_gettext),"gettext"},
	{C2F(sci_gettext),"_"},
	{C2F(sci_dgettext),"dgettext"},
	{C2F(sci_setdefaultlanguage),"setdefaultlanguage"}
};
/*--------------------------------------------------------------------------*/
int gw_localization(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
