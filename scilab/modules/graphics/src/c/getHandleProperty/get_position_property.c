/*------------------------------------------------------------------------*/
/* file: get_position_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the position field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_position_property( sciPointObj * pobj )
{

  if (sciGetEntityType(pobj) == SCI_UIMENU )
  {
    return sciReturnDouble( pUIMENU_FEATURE(pobj)->MenuPosition ) ;
  }
  else if ( sciGetEntityType(pobj) == SCI_LABEL )
  {
    double position[3] ;
    sciGetTextPos( pobj, position ) ;
    return sciReturnRowVector( position, 2 ) ;
  }
  sciprint("position does not exist for this handle.\n") ;
  return -1;
}
/*------------------------------------------------------------------------*/
