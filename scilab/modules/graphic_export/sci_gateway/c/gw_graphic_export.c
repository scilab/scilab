/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Jean-Baptiste SILVY
 *
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_graphic_export.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{
  {sci_xs2bmp,"xs2bmp"},
  {sci_xs2gif,"xs2gif"},
  {sci_xs2jpg,"xs2jpg"},
  {sci_xs2png,"xs2png"},
  {sci_xs2ppm,"xs2ppm"},
  {sci_xs2eps,"xs2eps"},
  {sci_xs2pdf,"xs2pdf"},
  {sci_xs2svg,"xs2svg"},
  {sci_xs2ps,"xs2ps"}
};
/*--------------------------------------------------------------------------*/
int gw_graphic_export(void)
{  
	Rhs = Max(0, Rhs);
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (!loadedDep) 
		{
			loadOnUseClassPath("graphics");
			loadedDep=TRUE;
		}

		callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	}
	else
	{
		Scierror(999,_("Scilab graphic module disabled -nogui or -nwni mode.\n"));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
