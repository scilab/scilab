/*------------------------------------------------------------------------*/
/* file: get_tics_direction_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the tics_direction field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_tics_direction_property( sciPointObj * pobj )
{
  if ( sciGetEntityType( pobj ) != SCI_AXES )
  {
    sciprint(_("%s property undefined for this handle.\n"), "tics_direction") ;
    return -1 ;
  }

  switch ( pAXES_FEATURE (pobj)->dir )
  {
  case 'u': 
    return sciReturnString( "top" ) ;
    break;
  case 'd': 
    return sciReturnString( "bottom" ) ;
    break;
  case 'r': 
    return sciReturnString( "right" ) ;
    break;
  case 'l': 
    return sciReturnString( "left" ) ;
    break;
  default : 
    sciprint(_("%s is not correctly defined.\n"), "tics_direction") ;
    break;
  }
  return -1;

}
/*------------------------------------------------------------------------*/
