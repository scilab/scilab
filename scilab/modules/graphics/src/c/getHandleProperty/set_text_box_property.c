/*------------------------------------------------------------------------*/
/* file: set_text_box_property.c                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the text_box field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_text_box_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  double * values = getDoubleMatrixFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"text_box") ;
    return SET_PROPERTY_ERROR ;
  }

  if (sciGetEntityType (pobj) != SCI_TEXT)
  {
    sciprint("text_box property does not exist for this handle.\n");
    return SET_PROPERTY_ERROR ;
  }

  if ( nbRow * nbCol != 2 )
  {
    sciprint("text_box must be a 2D vector.\n");
    return SET_PROPERTY_ERROR ;
  }
  return sciSetUserSize( pobj, values[0], values[1] ) ;


}
/*------------------------------------------------------------------------*/
