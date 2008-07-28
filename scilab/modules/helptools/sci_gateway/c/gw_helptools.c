/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008 - INRIA/DIGITEO - Sylvestre LEDRU
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_helptools.h"
#include "stack-c.h"
#include "inisci-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
 static gw_generic_table Tab[]=
 {
  {sci_buildDoc,"buildDoc"}
 };
/*--------------------------------------------------------------------------*/
int gw_helptools(void)
{
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
