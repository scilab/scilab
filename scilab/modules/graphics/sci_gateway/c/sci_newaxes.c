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
/* file: sci_newaxes.c                                                    */
/* desc : interface for newaxes routine                                   */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "gw_graphics.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "SetProperty.h"
#include "localization.h"
#include "HandleManagement.h"

/*--------------------------------------------------------------------------*/
int sci_newaxes( char * fname,unsigned long fname_len )
{
    // ???
    abort();
#if 0
  int minrhs = 0,maxrhs = 0,minlhs=0,maxlhs=1;
  sciPointObj *masousfen;
  int outindex,numrow   = 1,numcol   = 1;
  CheckRhs(0, 0);
  CheckLhs(0, 1);


  if ((masousfen = ConstructSubWin (sciGetCurrentFigure())) != NULL)
  {
    sciSetCurrentObj(masousfen);
    sciSetSelectedSubWin(masousfen);
    CreateVar(Rhs+1,GRAPHICAL_HANDLE_DATATYPE,&numrow,&numcol,&outindex);
    *hstk(outindex) = sciGetHandle(masousfen);

    LhsVar(1) = 1;
	PutLhsVar();
  }
  else
  {
    Scierror(999,_("%s: No more memory.\n"),fname);
  }
#endif
  return 0;

}
/*--------------------------------------------------------------------------*/
