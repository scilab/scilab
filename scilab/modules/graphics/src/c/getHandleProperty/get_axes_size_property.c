/*------------------------------------------------------------------------*/
/* file: get_axes_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the axes_size field of a         */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "InitObjects.h"

/*------------------------------------------------------------------------*/
int get_axes_size_property( sciPointObj * pobj )
{
  double axesSize[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("axes_size property undefined for this object") ;
    return -1;
  }
  
  axesSize[0] = sciGetWidth(  pobj ) ; 
  axesSize[1] = sciGetHeight( pobj ) ; 

  return sciReturnRowVector( axesSize, 2 ) ;

}
/*------------------------------------------------------------------------*/
