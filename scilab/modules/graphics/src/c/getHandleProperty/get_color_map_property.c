/*------------------------------------------------------------------------*/
/* file: get_color_map_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the color_map field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*-----------------------------------------------------------------------------------*/
int get_color_map_property( sciPointObj * pobj )
{
  if ( sciGetEntityType( pobj ) != SCI_FIGURE )
  {
    sciprint("color_map property does not exist for this handle.\n");
    return -1;
  }
  return sciReturnMatrix( pFIGURE_FEATURE(pobj)->pcolormap, sciGetNumColors( pobj ), 3 ) ;
}
/*-----------------------------------------------------------------------------------*/
