/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/ 
#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
#include "gw_matio.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/ 
int gw_matio(void);
/*-----------------------------------------------------------------------------------*/ 
static gw_generic_table Tab[] =
{
  {sci_matfile_open,"matfile_open"},
  {sci_matfile_close,"matfile_close"},
  {sci_matfile_listvar,"matfile_listvar"},
  {sci_matfile_varreadnext,"matfile_varreadnext"},
  {sci_matfile_varwrite,"matfile_varwrite"}
};
/*-----------------------------------------------------------------------------------*/  
int gw_matio(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 

