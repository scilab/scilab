/*------------------------------------------------------------------------*/
/* file: set_axes_size_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the axes_size field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_axes_size_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * newWindowSize = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_size") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("axes_size property undefined for this object") ;
    return SET_PROPERTY_ERROR ;
  }

  sciSetDimension(pobj, (int) newWindowSize[0], (int) newWindowSize[1] ) ;

  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
