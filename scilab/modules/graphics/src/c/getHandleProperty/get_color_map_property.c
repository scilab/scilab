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
#include "Scierror.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
int get_color_map_property( sciPointObj * pobj )
{
  double * colorMap = NULL ;
  int cmapSize      = 0    ;
  int status        = -1   ;
  if ( sciGetEntityType( pobj ) != SCI_FIGURE )
  {
    Scierror(999, "color_map property does not exist for this handle.\n");
    return -1;
  }

  cmapSize = sciGetNumColors(pobj) ;
  colorMap = MALLOC( cmapSize * 3 * sizeof(double) ) ;
  if ( colorMap == NULL )
  {
    Scierror(999, "Unable to allocate colormap, memory full.\n");
    return -1;
  }

  sciGetColormap(pobj, colorMap);

  status = sciReturnMatrix( colorMap, sciGetNumColors( pobj ), 3 ) ;

  FREE(colorMap) ;

  return status ;
}
/*-----------------------------------------------------------------------------------*/
