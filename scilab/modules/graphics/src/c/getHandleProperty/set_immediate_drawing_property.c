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
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_immediate_drawing_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  char * value = getStringFromStack( stackPointer ) ;
  
  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint( "immediate_drawing property undefined for this object.\n" ) ;
    return -1;
  }
  
  if ( strcmp(value,"on") == 0 )
  {
    pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = TRUE ;
    return 0 ;
  }
  else if ( strcmp(value,"off") == 0 )
  {
    pFIGURE_FEATURE((sciPointObj *)pobj)->auto_redraw = FALSE ;
    return 0 ;
  }
  else
  {
    sciprint("Nothing to do (value must be 'on/off')") ;
    return -1 ;
  }
  
  return -1 ;
}
/*------------------------------------------------------------------------*/
