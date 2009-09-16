/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_tclsci.h"
#include "stack-c.h"
#include "inisci-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "InitTclTk.h" /* isTkStarted */
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
  {sci_TCL_DoOneEvent,"TCL_DoOneEvent"},
  {sci_TCL_EvalStr,"TCL_EvalStr"},
  {sci_TCL_GetVar,"TCL_GetVar"},
  {sci_TCL_SetVar,"TCL_SetVar"},
  {sci_opentk,"opentk"},
  {sci_TCL_GetVersion,"TCL_GetVersion"},
  {sci_TCL_UnsetVar,"TCL_UnsetVar"},
  {sci_TCL_ExistVar,"TCL_ExistVar"},
  {sci_TCL_UpVar,"TCL_UpVar"},
  {sci_TCL_DeleteInterp,"TCL_DeleteInterp"},
  {sci_TCL_ExistInterp,"TCL_ExistInterp"},
  {sci_TCL_ExistArray,"TCL_ExistArray"},
  {sci_TCL_EvalFile,"TCL_EvalFile"}
 };
/*--------------------------------------------------------------------------*/
int gw_tclsci(void)
{
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (isTkStarted())
		{
			Rhs = Max(0, Rhs);
			callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
		}
		else
		{
			sciprint(_("Warning: Problem(s) with TCL/TK interface. Interface not enabled.\n"));
		}
	}
	else
	{
		Scierror(999,_("Tcl/TK interface disabled in -nogui mode.\n"));
		return 0;
	}

  return 0;
}
/*--------------------------------------------------------------------------*/
