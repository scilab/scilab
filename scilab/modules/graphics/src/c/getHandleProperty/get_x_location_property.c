/*------------------------------------------------------------------------*/
/* file: get_x_location_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the x_location field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_x_location_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_SUBWIN)
  {
    sciprint("x_location property undefined for this handle\n.") ;
    return -1 ;
  }

  switch ( pSUBWIN_FEATURE (pobj)->axes.xdir )
  {
  case 'u': 
    return sciReturnString( "top" ) ; 
    break;
  case 'd': 
    return sciReturnString( "bottom" ) ;
    break;
  case 'c': 
    return sciReturnString( "middle" ) ;
    break;
  default : 
    sciprint("x_location is not correctly defined\n.") ;
    break;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
