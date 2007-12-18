/*------------------------------------------------------------------------*/
/* file: set_data_mapping_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the data_mapping field of          */
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
int set_data_mapping_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"data_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_GRAYPLOT )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"data_mapping") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( isStringParamEqual( stackPointer, "scaled") || isStringParamEqual( stackPointer, "direct") )
  {
     strcpy( pGRAYPLOT_FEATURE (pobj)->datamapping, getStringFromStack(stackPointer) ) ;
     return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    sciprint("Value must be 'direct' or 'scaled'.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  return SET_PROPERTY_ERROR ;


}
/*------------------------------------------------------------------------*/
