/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_draw.h                                                       */
/* desc : interface for draw routine                                      */
/*------------------------------------------------------------------------*/
#include "stack-c.h"
#include "gw_graphics.h"
#include "warningmode.h"
#include "sciprint.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
//
// FIXME: Remove GW after Scilab 5.4.0
//
int sci_draw( char * fname, unsigned long fname_len )
{

  CheckRhs(0,1) ;
  CheckLhs(0,1) ;

  if (getWarningMode())
  {
      sciprint(_("WARNING: %s\n"), _("draw function is obsolete."));
      sciprint(_("WARNING: %s\n"), _("It will be removed after Scilab 5.4.0."));
      sciprint(_("WARNING: %s\n"), _("Please see documentation for more details."));
  }

  LhsVar(1) = 0;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
