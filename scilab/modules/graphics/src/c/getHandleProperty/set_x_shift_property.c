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
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_x_shift_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  int nbElement = nbRow * nbCol ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"x_shift") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint( "This handle has no x_shift property.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow > 1 && nbCol > 1 )
  {
    sciprint( "Bad input, x_shift should be a row or column vector.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbElement != 0 && nbElement != pPOLYLINE_FEATURE (pobj)->n1 ) /* we can specify [] (null vector) to reset to default */
  {
    sciprint("Wrong size for input vector.\n");
    return SET_PROPERTY_ERROR ;
  }

  FREE( pPOLYLINE_FEATURE(pobj)->x_shift ) ;
  pPOLYLINE_FEATURE(pobj)->x_shift = NULL;

  if( nbElement != 0 )
  {
    pPOLYLINE_FEATURE(pobj)->x_shift = createCopyDoubleVectorFromStack( stackPointer, nbElement ) ;

    if ( pPOLYLINE_FEATURE (pobj)->x_shift == NULL )
    {
      sciprint("No memory left for allocating temporary tics_coord.\n") ;
      return SET_PROPERTY_ERROR ;
    }
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
