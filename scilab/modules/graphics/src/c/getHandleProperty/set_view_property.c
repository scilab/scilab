/*------------------------------------------------------------------------*/
/* file: set_view_property.c                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the view field of                  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_view_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"view") ;
    return SET_PROPERTY_ERROR ;
  }

  /* DJ.A 2003 */
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    sciprint(_("%s property does not exist for this handle.\n"),"view");
    return  SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "2d" ) )
  { 
    return sciSetIs3d( pobj, FALSE ) ;
  }
  else if ( isStringParamEqual( stackPointer, "3d" ) )
  {
    return sciSetIs3d( pobj, TRUE ) ;
  }
  else
  {
    sciprint(_("%s: Wrong type for second input argument: '%s' or '%s' expected.\n"), "set_view_property","2d","3d") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
