
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
    {NULL, ""}, /* not used */
    {sci_ppol, "ppol"},
    {sci_tzer, "tzer"},
    {sci_freq, "freq"},
    {sci_ltitr, "ltitr"},
    {sci_rtitr, "rtitr"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd2(void)
{  
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/
