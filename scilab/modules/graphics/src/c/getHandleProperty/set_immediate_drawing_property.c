/*------------------------------------------------------------------------*/
/* file: set_immediate_drawing_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the immediate_drawing field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_immediate_drawing_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"immediate_drawing") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("immediate_drawing property undefined for this object.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  
  if ( isStringParamEqual( stackPointer, "on" ) )
  {
    pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = TRUE ;
    return SET_PROPERTY_SUCCEED ;
  }
  else if ( isStringParamEqual( stackPointer, "off" ) )
  {
    pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = FALSE ;
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("Nothing to do (value must be 'on/off')") ;
    return SET_PROPERTY_ERROR ;
  }
  
}
/*------------------------------------------------------------------------*/
