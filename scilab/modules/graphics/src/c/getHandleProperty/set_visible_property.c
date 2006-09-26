/*------------------------------------------------------------------------*/
/* file: set_visible_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the visible field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_visible_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  char * value = getStringFromStack( stackPointer ) ;
  if ( strcmp(value,"on") == 0 )
  {
    return sciSetVisibility( pobj, TRUE ) ;
  }
  else if ( strcmp(value,"off") == 0 )
  {
    return sciSetVisibility( pobj, FALSE ) ;
  }
  else
  {
    sciprint("Value must be 'on' or 'off'.\n") ;
    return -1 ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
