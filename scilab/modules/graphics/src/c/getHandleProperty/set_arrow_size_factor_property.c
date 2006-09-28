/*------------------------------------------------------------------------*/
/* file: set_arrow_size_factor_property.c                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the arrow_size_factor field of     */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_arrow_size_factor_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  if ( sciGetEntityType(pobj) != SCI_POLYLINE )
  {
    sciprint( "arrow_size_factor property undefined for this handle.\n" ) ;
    return -1 ;
  }
  pPOLYLINE_FEATURE(pobj)->arsize_factor = getDoubleFromStack( stackPointer ) ;
  return 0 ;
}
/*------------------------------------------------------------------------*/
