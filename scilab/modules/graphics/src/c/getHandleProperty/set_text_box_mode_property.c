/*------------------------------------------------------------------------*/
/* file: set_text_box_mode_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the text_box_mode field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_text_box_mode_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint("Incompatible type for property text_box_mode.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_TEXT )
  {
    sciprint( "text_box_mode property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "off" ) )
  {
    int status1 = sciSetCenterPos( pobj, FALSE ) ;
    int status2 = sciSetAutoSize(  pobj, TRUE  ) ;
    return sciSetFinalStatus( status1, status2 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "centered" ) )
  {
    int status1 = sciSetCenterPos( pobj, TRUE  ) ;
    int status2 = sciSetAutoSize(  pobj, TRUE  ) ;
    return sciSetFinalStatus( status1, status2 ) ;
  }
  else if ( isStringParamEqual( stackPointer, "filled" ) )
  {
    int status1 = sciSetCenterPos( pobj, TRUE  ) ;
    int status2 = sciSetAutoSize(  pobj, FALSE ) ;
    return sciSetFinalStatus( status1, status2 ) ;
  }
  else
  {
    sciprint( "Value must be 'off', 'centered' or 'filled'.\n" );
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;

}
/*------------------------------------------------------------------------*/
