/*------------------------------------------------------------------------*/
/* file: get_immediate_drawing_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the immediate_drawing field of a */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_immediate_drawing_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) != SCI_FIGURE)
  {
    sciprint(_("%s undefined for this object.\n"), "immediate_drawing") ;
    return -1;
  }
  if ( pFIGURE_FEATURE(pobj)->auto_redraw )
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}
/*------------------------------------------------------------------------*/
