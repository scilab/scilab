/*------------------------------------------------------------------------*/
/* file: get_y_location_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the y_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_y_location_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    sciprint("y_location property undefined for this handle\n.") ;
    return -1 ;
  }

  switch ( pSUBWIN_FEATURE (pobj)->axes.ydir )
  {
  case 'l': 
    return sciReturnString( "left" ) ; 
    break;
  case 'r': 
    return sciReturnString( "right" ) ;
    break;
  case 'c': 
    return sciReturnString( "middle" ) ;
    break;
  default : 
    sciprint("y_location is not correctly defined\n.") ;
    break;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
