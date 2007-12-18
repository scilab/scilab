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
    sciprint(_("%s property undefined for this handle.\n"), "y_location") ;
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
    sciprint(_("%s is not correctly defined.\n"), "y_location") ;
    break;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
