/*------------------------------------------------------------------------*/
/* file: set_margins_property.c                                           */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the margins field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_margins_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"margins") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint("margins property does not exist for this handle.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 4 )
  {
    sciprint("Second argument must have 4 elements.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  copyDoubleVectorFromStack( stackPointer, pSUBWIN_FEATURE (pobj)->ARect, 4 ) ;

  return SET_PROPERTY_SUCCEED ;
  
}
/*------------------------------------------------------------------------*/
