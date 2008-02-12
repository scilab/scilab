/*------------------------------------------------------------------------*/
/* file: sci_relocate_handle.h                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for relocate_handle routine                           */
/*------------------------------------------------------------------------*/

#include "sci_relocate_handle.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/

int sci_relocate_handle( char * fname, unsigned long fname_len )
{
  int handleCol ;
  int handleRow ;
  int nbHandle ;
  int handleStkIndex ;
  int parentCol ;
  int parentRow ;
  int parentStkIndex ;
  int outIndex ;
  int i ;
  unsigned long * handleArray = NULL ;

  /* the function should be called with relocate_handle( handle, parent_handle ) */
  CheckRhs(2,2) ;
  CheckLhs(0,1) ;


  GetRhsVar( 1,GRAPHICAL_HANDLE_DATATYPE, &handleRow, &handleCol, &handleStkIndex );
  nbHandle = handleRow * handleCol ;
  GetRhsVar( 2,GRAPHICAL_HANDLE_DATATYPE, &parentRow, &parentCol, &parentStkIndex );

  if ( parentCol * parentRow != 1 )
  {
    Scierror(999,_("%s: Handles must be relocated under a single parent.\n"),fname);
    return 0 ;
  }

  /* create an array of handles */
  handleArray = MALLOC( nbHandle * sizeof( unsigned long ) ) ;
  if ( handleArray == NULL )
  {
    Scierror(999,_("%s: No more memory.\n"),fname);
    return 0 ;
  }

  for ( i = 0 ; i < nbHandle ; i++ )
  {
    handleArray[i] = (unsigned long) *hstk( handleStkIndex + i ) ;
  }

  if ( sciRelocateHandles( handleArray          ,
                           handleRow * handleCol,
                           (unsigned long) *hstk( parentStkIndex ) ) != 0 )
  {
    return 0 ;
  }
  FREE( handleArray ) ;
  CreateVar( Rhs + 1,GRAPHICAL_HANDLE_DATATYPE, &handleCol, &handleRow, &outIndex );
  *hstk(outIndex) = *hstk(handleStkIndex) ;
  LhsVar(1) = Rhs + 1 ;
  return 0 ;

}
/*--------------------------------------------------------------------------*/
