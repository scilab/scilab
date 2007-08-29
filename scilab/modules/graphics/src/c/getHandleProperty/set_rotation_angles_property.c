/*------------------------------------------------------------------------*/
/* file: set_rotation_angles_property.c                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the rotation_angles field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "DrawObjects.h" 

/*------------------------------------------------------------------------*/
int set_rotation_angles_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint("Incompatible type for property rotation_angles.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  /* DJ.A 2003 */
  if ( sciGetEntityType (pobj) != SCI_SUBWIN )
  {
    sciprint("rotation_angles property does not exist for this handle.\n" ) ;
    return SET_PROPERTY_ERROR ;
  }

  Obj_RedrawNewAngle( pobj, values[1], values[0] ) ;
  setInfoMessageWithRotationAngles(sciGetParentFigure(pobj), values[0], values[1]);

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
