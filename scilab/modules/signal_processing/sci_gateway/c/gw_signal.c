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
#include "gw_signal.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_ffir),"ffir"},
	{C2F(sci_fft),"fft"},
	{C2F(sci_fiir),"fiir"},
	{C2F(sci_corr),"corr"},
	{C2F(sci_rpem),"rpem"},
	{C2F(sci_amell),"amell"},
	{C2F(sci_delip),"delip"},
	{C2F(sci_remez),"remez"},
	{C2F(sci_syredi),"syredi"}
};
/*--------------------------------------------------------------------------*/
int gw_signal_processing(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
