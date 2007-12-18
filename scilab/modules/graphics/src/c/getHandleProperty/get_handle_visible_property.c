/*------------------------------------------------------------------------*/
/* file: get_handle_visible_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the handle_visible field of      */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_handle_visible_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_UIMENU)
  {
    sciprint(_("%s property does not exist for this handle.\n"),"handle_visible");
    return -1;
  }
  if ( GetHandleVisibilityOnUimenu( pobj ) )
  {
    return sciReturnString( "on" ) ;
  }
  else 
  {
    return sciReturnString( "off" ) ;
  }

}
/*------------------------------------------------------------------------*/
