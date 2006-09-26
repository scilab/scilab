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

/*------------------------------------------------------------------------*/
int set_current_entity_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  sciPointObj * curEntity = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;
  
  if ( curEntity == NULL )
  {
    sciprint( "Object is not valid.\n" ) ;
    return -1 ;
  }
  
  return sciSetCurrentObj( curEntity ) ;
}
/*------------------------------------------------------------------------*/
