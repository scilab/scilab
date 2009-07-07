/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
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
#define SIGNAL_TAB_SIZE 9
static gw_generic_table Tab[SIGNAL_TAB_SIZE]=
{
	{sci_ffir,   "ffir"},
	{sci_fft,    "fft"},
	{sci_fiir,   "fiir"},
	{sci_corr,   "corr"},
	{sci_rpem,   "rpem"},
	{sci_amell,  "amell"},
	{sci_delip,  "delip"},
	{sci_remez,  "remez"},
	{sci_syredi, "syredi"}
};
/*--------------------------------------------------------------------------*/
int gw_signal_processing(void)
{  
	callFunctionFromGateway(Tab,SIGNAL_TAB_SIZE);
	return 0;
}
/*--------------------------------------------------------------------------*/
