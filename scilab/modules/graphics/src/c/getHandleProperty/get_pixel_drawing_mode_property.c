/*------------------------------------------------------------------------*/
/* file: get_pixel_drawing_mode_property.c                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the pixel_drawing_mode field of  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "pixel_mode.h"
#include "returnProperty.h"
#include "Scierror.h"
/*------------------------------------------------------------------------*/

int get_pixel_drawing_mode_property( sciPointObj * pobj )
{
  if ( sciGetEntityType (pobj) == SCI_FIGURE )
  {
    return sciReturnString( getPixelMode( pFIGURE_FEATURE (pobj)->gmode.xormode ) ) ;
  }
  else
  {
    Scierror(999,"pixel_drawing_mode do not exist for this handle") ;
    return -1 ;
  }

}

/*------------------------------------------------------------------------*/
