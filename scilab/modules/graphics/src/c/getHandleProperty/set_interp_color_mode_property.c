/*------------------------------------------------------------------------*/
/* file: set_interp_color_mode_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the interp_color_mode field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_interp_color_mode_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint("interp_color_mode can only be set on Polyline objects.\n") ;
    return -1 ;
  }

  if( isStringParamEqual( stackPointer, "on" ) )
  {
    if ( sciGetInterpVector(pobj) == NULL )
    {
      sciprint("You must first specify an interp_color_vector for this object.\n") ;
      return -1 ;
    }
    else
    {
      pPOLYLINE_FEATURE (pobj)->isinterpshaded = TRUE ;
    }
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    pPOLYLINE_FEATURE (pobj)->isinterpshaded = FALSE ;
  }
  else
  {
    sciprint("Property must be set to 'on' or 'off'.\n") ;
    return -1 ;
  }
  return 0 ;

}
/*------------------------------------------------------------------------*/
