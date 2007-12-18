/*------------------------------------------------------------------------*/
/* file: get_viewport_property.c                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to retrieve in Scilab the viewport field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_viewport_property( sciPointObj * pobj )
{
  int viewport[2] ;

  if( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint(_("%s does not exist for this handle.\n"), "viewport") ;
    return -1 ;
  }

  sciGetViewport( pobj, &viewport[0], &viewport[1] ) ;

  return sciReturnRowIntVector( viewport, 2 ) ;

}
/*------------------------------------------------------------------------*/
