/*------------------------------------------------------------------------*/
/* file: get_y_shift_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the y_shift field of a           */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_y_shift_property( sciPointObj * pobj )
{
  if (sciGetEntityType (pobj) == SCI_POLYLINE )
  {
    sciPolyline *  ppolyline = pPOLYLINE_FEATURE (pobj);

    if( ppolyline->y_shift ==  NULL )
    {
      return sciReturnEmptyMatrix() ;
    }
    else
    {
      return sciReturnRowVector( ppolyline->y_shift, ppolyline->n1 ) ;
    }
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"y_shift") ;
    return -1 ;
  }
}
/*------------------------------------------------------------------------*/
