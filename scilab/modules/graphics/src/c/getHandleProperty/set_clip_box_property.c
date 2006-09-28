/*------------------------------------------------------------------------*/
/* file: set_clip_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the clip_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_clip_box_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  int status1 ;
  int status2 ;

  /* On doit avoir avoir une matrice 4x1 */
  if ( nbRow * nbCol != 4 )
  {
    sciprint("Argument must be a vector of size 4.\n");
    return SET_PROPERTY_ERROR ;
  }
  status1 = sciSetClipping( pobj, getDoubleMatrixFromStack( stackPointer ) ) ;
  status2 = sciSetIsClipping( pobj, 1 ) ;
  return sciSetFinalStatus( status1, status2 ) ;

}
/*------------------------------------------------------------------------*/
