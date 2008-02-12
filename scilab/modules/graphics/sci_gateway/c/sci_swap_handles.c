/*------------------------------------------------------------------------*/
/* file: sci_swap_handles.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for swap_handles routine                              */
/*------------------------------------------------------------------------*/

#include "sci_swap_handles.h"
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
    Scierror(999,_("%s : Routine can only swap two single handles.\n"),fname);
    return 0 ;
  }

  /* get the two handles and swap them */
  swapHandles( (unsigned long) *hstk( firstHdlStkIndex  ),
               (unsigned long) *hstk( secondHdlStkIndex ) ) ;
  LhsVar(1) = 0 ;
  return 0 ;
}
/*--------------------------------------------------------------------------*/
