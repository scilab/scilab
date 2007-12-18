/*------------------------------------------------------------------------*/
/* file: set_zoom_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the zoom_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"
#include "PloEch.h"

/*------------------------------------------------------------------------*/
int set_zoom_box_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"zoom_box") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_SUBWIN )
  {
    sciprint(_("%s property does not exist for this handle.\n"),"zoom_box") ;
    return SET_PROPERTY_ERROR ;
  }

  /* On doit avoir avoir une matrice 4x1 */
  if ( nbRow * nbCol == 4 )
  {
    scizoom( getDoubleMatrixFromStack( stackPointer ), pobj ) ;
  }
  else if ( nbCol * nbRow == 0 )
  {
    unzoom() ;
  }
  else
  {
    sciprint("Argument must be a vector of size 4.\n");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
