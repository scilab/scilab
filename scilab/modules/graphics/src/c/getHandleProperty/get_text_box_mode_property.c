/*------------------------------------------------------------------------*/
/* file: get_text_box_mode_property.c                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the text_box_mode field of a     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_text_box_mode_property( sciPointObj * pobj )
{
  if ( sciGetEntityType(pobj) == SCI_TEXT ) 
  {
    if ( sciGetAutoSize( pobj ) )
    {
      if ( sciGetCenterPos( pobj ) )
      {
        return sciReturnString( "centered" ) ;
      }
      else
      {
        return sciReturnString( "off" ) ;
      }
    }
    else
    {
      return sciReturnString( "filled" ) ;
    }
  }
  sciprint(_("%s property does not exist for this handle.\n"),"text_box_mode");
  return -1;
}
/*------------------------------------------------------------------------*/
