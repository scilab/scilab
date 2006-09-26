/*------------------------------------------------------------------------*/
/* file: set_rotation_style_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the rotation_style field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_rotation_style_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  char * value = getStringFromStack( stackPointer ) ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE ) 
  {
    sciprint( "rotation_style_property undefined for this object.\n" ) ;
    return -1;
  }

  if ( strcmp(value,"unary") == 0 )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 0 ;
    return 0 ;
  }
  else if ( strcmp(value,"multiple") == 0 )
  {
    pFIGURE_FEATURE(pobj)->rotstyle = 1 ;
    return 0 ;
  }
  else
  {
    sciprint("Nothing to do (value must be 'unary/multiple').\n");
    return -1;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
