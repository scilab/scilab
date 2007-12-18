/*------------------------------------------------------------------------*/
/* file: get_box_property.c                                               */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the box field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_box_property( sciPointObj * pobj )
{
  if ( sciGetEntityType ( pobj ) == SCI_SUBWIN )
  {
    switch( sciGetBoxType( pobj ) )
    {
    case BT_OFF:
      return sciReturnString( "off" ) ;
      break ;
    case BT_ON:
      return sciReturnString( "on" ) ;
      break ;
    case BT_HIDDEN_AXIS:
      return sciReturnString( "hidden_axis" ) ;
      break ;
    case BT_BACK_HALF:
      return sciReturnString( "back_half" ) ;
      break ;
    }
  }
  else
  {
    if ( sciGetIsBoxed(pobj) )
    {
      return sciReturnString( "on" ) ;
    }
    else
    {
      return sciReturnString( "off" ) ;  
    }
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
