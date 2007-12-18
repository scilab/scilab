/*------------------------------------------------------------------------*/
/* file: set_z_bounds_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the z_bounds field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "ColorMapManagement.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

/*------------------------------------------------------------------------*/
int set_z_bounds_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"z_bounds") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FEC )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"z_bounds") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("Second argument must have 2 elements.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  pFEC_FEATURE (pobj)->zminmax[0] = values[0] ;
  pFEC_FEATURE (pobj)->zminmax[1] = values[1] ;

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
