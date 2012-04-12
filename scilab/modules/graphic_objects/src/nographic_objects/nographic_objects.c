/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "nographic_objects.h"
#include "Scierror.h"
#include "localization.h"
#include "FigureList.h"
/*--------------------------------------------------------------------------*/
int gw_graphic_objects(void)
{
	Scierror(999,_("Scilab '%s' module not installed.\n"), "graphic_objects");
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL sciHasFigures(void)
{
  return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
	return FALSE;
}
/*--------------------------------------------------------------------------*/
