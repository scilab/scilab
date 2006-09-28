/*------------------------------------------------------------------------*/
/* file: set_bar_layout_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the bar_layout field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_bar_layout_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint( "bar_layout property does not exist for this handle.\n" ) ;
    return -1 ;
  }
  
  if( isStringParamEqual( stackPointer, "grouped" ) )
  {
    pPOLYLINE_FEATURE (pobj)->bar_layout = 0 ;
  }
  else if( isStringParamEqual( stackPointer, "stacked" ) )
  {
    pPOLYLINE_FEATURE (pobj)->bar_layout = 1 ;
  }
  else
  {
    sciprint( "bar_layout must be set to 'grouped' or 'stacked'.\n" ) ;
    return -1;
  }

  return -1 ;
}
/*------------------------------------------------------------------------*/
