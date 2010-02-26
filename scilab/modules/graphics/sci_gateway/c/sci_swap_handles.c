/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_swap_handles.c                                               */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_swap_handles( char * fname, unsigned long fname_len )
{
  int firstHdlCol  ;
  int firstHdlRow  ;
  int secondHdlCol ;
  int secondHdlRow ;
  int firstHdlStkIndex  ;
  int secondHdlStkIndex ;

  CheckRhs( 2, 2 ) ;
  CheckLhs( 0, 1 ) ;

  GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &firstHdlRow, &firstHdlCol, &firstHdlStkIndex );
  GetRhsVar( 2,GRAPHICAL_HANDLE_DATATYPE, &secondHdlRow, &secondHdlCol, &secondHdlStkIndex );

  if ( firstHdlRow * firstHdlCol != 1 || secondHdlRow * secondHdlCol != 1 )
  {
    Scierror(999,_("%s: Routine can only swap two single handles.\n"),fname);
    return 0 ;
  }

  /* get the two handles and swap them */
  swapHandles( (unsigned long) *hstk( firstHdlStkIndex  ),
               (unsigned long) *hstk( secondHdlStkIndex ) ) ;
  LhsVar(1) = 0 ;
	C2F(putlhsvar)();
  return 0 ;
}
/*--------------------------------------------------------------------------*/
