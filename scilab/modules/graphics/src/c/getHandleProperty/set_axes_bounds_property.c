/*------------------------------------------------------------------------*/
/* file: set_axes_bounds_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the axes_bounds field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
#include "PloEch.h"

/*------------------------------------------------------------------------*/
int set_axes_bounds_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"axes_bounds") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"axes_bounds") ;
    return SET_PROPERTY_ERROR ;
  }
  if ( nbRow * nbCol != 4 )
  {
    sciprint("Second argument must have 4 elements.\n");
    return SET_PROPERTY_ERROR ;
  }
  
  copyDoubleVectorFromStack( stackPointer, pSUBWIN_FEATURE (pobj)->WRect, 4 ) ;

  set_scale( "ttffff", pSUBWIN_FEATURE (pobj)->WRect, NULL, NULL, NULL, NULL ) ;
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
