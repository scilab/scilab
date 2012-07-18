/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*------------------------------------------------------------------------*/
/* file: sci_clear_pixmap.c                                               */
/* desc : interface for clear_pixmap routine                              */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "warningmode.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
//
// FIXME: Remove GW after Scilab 5.4.0
//
int sci_clear_pixmap(char *fname, unsigned long fname_len )
{
  CheckRhs(0,0);
  CheckLhs(0,1);

  if (getWarningMode())
  {
      sciprint(_("WARNING: %s\n"), _("clear_pixmap function is obsolete."));
      sciprint(_("WARNING: %s\n"), _("It will be removed after Scilab 5.4.0."));
      sciprint(_("WARNING: %s\n"), _("Please use drawlater/drawnow instead."));
  }

  LhsVar(1)=0;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
