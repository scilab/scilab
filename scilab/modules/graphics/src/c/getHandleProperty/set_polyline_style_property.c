/*------------------------------------------------------------------------*/
/* file: set_polyline_style_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the polyline_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_polyline_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  int value = 0 ;
  if (sciGetEntityType (pobj) != SCI_POLYLINE)
  {
    sciprint( "polyline_style property does not exist for this handle.\n" ) ;
    return -1 ;
  }
    
  value = (int) getDoubleFromStack( stackPointer ) ;
  if ( value < 1 || value > 7 )
  {
    sciprint("Style must be 1,2,3,4,5,6 or 7.\n") ;
    return -1 ;
  }

  pPOLYLINE_FEATURE (pobj)->plot = value ;
  return 0 ;
  
}
/*------------------------------------------------------------------------*/
