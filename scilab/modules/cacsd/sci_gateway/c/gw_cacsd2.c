
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
#include "gw_cacsd2.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static int C2F(sci_xxxx)(char *fname,unsigned long fname_len)
{
	/* not used */
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_xxxx),"xxxx"}, /* not used */
	{C2F(sci_ppol),"ppol"},
	{C2F(sci_tzer),"tzer"},
	{C2F(sci_freq),"freq"},
	{C2F(sci_ltitr),"ltitr"},
	{C2F(sci_rtitr),"rtitr"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd2(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
