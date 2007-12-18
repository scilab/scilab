/*------------------------------------------------------------------------*/
/* file: set_zoom_state_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the zoom_state field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "PloEch.h"

/*------------------------------------------------------------------------*/
int set_zoom_state_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"zoom_state") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"zoom_state") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "on" ) )
  { 
    if ( ! sciGetZooming(pobj) )
    {
      sciprint("Object is already zoomed.\n") ;
      return SET_PROPERTY_ERROR ;
    }
    else
    {
      sciprint("set zoom box ( set('zoom_box',[xmin ymin xmax ymax])).\n") ;
      return SET_PROPERTY_ERROR ;
    }
  }
  else if ( isStringParamEqual( stackPointer, "off" ) ) 
  { 
    unzoom();
    return sciSetZooming( pobj, FALSE ) ;
  }
  else
  {
    sciprint(_("%s: Wrong input argument: '%s' or '%s' expected.\n"),"set_zoom_state_property","on","off");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
