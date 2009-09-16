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
#include "gw_jvm.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "Scierror.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table  Tab[]=
{
	{sci_with_embedded_jre,"with_embedded_jre"},
	{sci_system_setproperty,"system_setproperty"},
	{sci_system_getproperty,"system_getproperty"},
	{sci_javaclasspath,"javaclasspath"},
	{sci_javalibrarypath,"javalibrarypath"}
};
/*--------------------------------------------------------------------------*/
int gw_jvm(void)
{  
	Rhs = Max(0, Rhs);

	if ( (getScilabMode() != SCILAB_NWNI) )
	{
		callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));	
	}
	else
	{
		Scierror(999,_("JVM interface disabled in -nogui or -nwni modes.\n"));
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
