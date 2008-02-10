/*------------------------------------------------------------------------*/
/* file: set_labels_font_size_property.c                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the tics_segment field of          */
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
int set_labels_font_size_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"labels_font_size") ;
    return SET_PROPERTY_ERROR ;
  }

  if (   sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE
      || sciGetEntityType (pobj) == SCI_AXES)
  {
    return sciSetFontSize( pobj, getDoubleFromStack(stackPointer) ) ; /* F.Leray 08.04.04 */
  }
  else
  {
    sciprint(_("%s property does not exist for this handle.\n"),"labels_font_size") ;
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
