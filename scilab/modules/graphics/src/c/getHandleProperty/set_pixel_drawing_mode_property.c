/*------------------------------------------------------------------------*/
/* file: set_pixel_drawing_mode_property.c                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the pixel_drawing_mode field of    */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "pixel_mode.h"

/*------------------------------------------------------------------------*/
int set_pixel_drawing_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  int v = -1 ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint( "pixel_drawing_mode: unknown property for this handle.\n" ) ;
    return -1 ;
  }
  v = getPixelModeIndex( getStringFromStack( stackPointer ) ) ;

  if ( v < 0 )
  {
    sciprint( "pixel_drawing_mode: unknown property for this handle.\n" ) ;
  }

  return sciSetXorMode( pobj, v );
}
/*------------------------------------------------------------------------*/
