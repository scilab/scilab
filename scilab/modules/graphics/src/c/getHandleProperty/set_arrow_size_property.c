/*------------------------------------------------------------------------*/
/* file: set_arrow_size_property.c                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the arrow_size field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int set_arrow_size_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"arrow_size_factor") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SEGS )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"arrow_size") ;
    return SET_PROPERTY_ERROR ;
  }

  pSEGS_FEATURE (pobj)->arrowsize = getDoubleFromStack( stackPointer ) ;

  return SET_PROPERTY_SUCCEED ;

}
/*------------------------------------------------------------------------*/
