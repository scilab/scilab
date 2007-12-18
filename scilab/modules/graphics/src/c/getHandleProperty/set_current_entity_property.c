/*------------------------------------------------------------------------*/
/* file: set_current_entity_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the current_entity field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"
#include "CurrentObjectsManagement.h"

/*------------------------------------------------------------------------*/
int set_current_entity_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  sciPointObj * curEntity = NULL ;
  
  if ( !isParameterHandle( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"current_entity") ;
    return SET_PROPERTY_ERROR ;
  }

  curEntity = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;

  if ( curEntity == NULL )
  {
    sciprint("Object is not valid.\n") ;
    return SET_PROPERTY_ERROR ;
  }
  sciSetCurrentObj( curEntity ) ;
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
