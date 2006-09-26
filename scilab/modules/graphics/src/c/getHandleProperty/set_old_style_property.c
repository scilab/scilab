/*------------------------------------------------------------------------*/
/* file: set_old_style_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the old_style field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_old_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  char * value = getStringFromStack( stackPointer ) ;
  if ( strcmp(value,"on") == 0 )
  {
    setVersionFlag( 1 ) ;
    return 0 ;
  }
  else if ( strcmp(value,"off") == 0 )
  {
    setVersionFlag( 0 ) ;
    return 0 ;
  }
  else
  {
    sciprint("old_style must be 'on' or 'off'.\n");
    return -1;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
