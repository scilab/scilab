/*------------------------------------------------------------------------*/
/* file: set_x_shift_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the x_shift field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int set_x_shift_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{

  int nbElement = nbRow * nbCol ;

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint( "This handle has no x_shift property.\n" ) ;
    return -1;
  }

  if ( nbRow > 1 && nbCol > 1 )
  {
    sciprint( "Bad input, x_shift should be a row or column vector.\n" ) ;
    return -1;
  }

  if ( nbElement != 0 && nbElement != pPOLYLINE_FEATURE (pobj)->n1 ) /* we can specify [] (null vector) to reset to default */
  {
    sciprint("Wrong size for input vector.\n");
    return -1;
  }

  FREE( pPOLYLINE_FEATURE(pobj)->x_shift ) ;
  pPOLYLINE_FEATURE(pobj)->x_shift = NULL;

  if( nbElement != 0 )
  {
    pPOLYLINE_FEATURE(pobj)->x_shift = MALLOC( nbElement * sizeof (double) ) ;

    if ( pPOLYLINE_FEATURE (pobj)->x_shift == NULL )
    {
      sciprint("No memory left for allocating temporary tics_coord.\n") ;
      return -1;
    }

    copyDoubleVectorFromStack( stackPointer, pPOLYLINE_FEATURE(pobj)->x_shift, nbElement ) ;
  }
  return 0 ;
}
/*------------------------------------------------------------------------*/
