/*------------------------------------------------------------------------*/
/* file: set_viewport_property.c                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to modify in Scilab the auto_rotation field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_viewport_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int status1 ;
  int status2 ;
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"viewport") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("viewport does not exist for this handle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("Argument must be a vector of size 2.\n");
    return SET_PROPERTY_ERROR ;
  }

  /* force auto_resize. With auto_resize disable, resize does not work */
  status1 = sciSetResize( pobj, FALSE ) ;
  status2 = sciSetViewport( pobj, (int)values[0], (int)values[1]  ) ;

  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;
}
/*------------------------------------------------------------------------*/
