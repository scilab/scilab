/*------------------------------------------------------------------------*/
/* file: get_x_shift_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the x_shift field of a           */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int get_x_shift_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) == SCI_POLYLINE )
  {
    sciPolyline *  ppolyline = pPOLYLINE_FEATURE (pobj);

    if( ppolyline->x_shift ==  NULL )
    {
      return sciReturnEmptyMatrix() ;
    }
    else
    {
      return sciReturnRowVector( ppolyline->x_shift, ppolyline->n1 ) ;
    }
  }
  else
  {
    sciprint( "x_shift property does not exist for this handle.\n") ;
    return -1 ;
  }
  return -1 ;
}
/*------------------------------------------------------------------------*/
