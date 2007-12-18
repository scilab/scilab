/*------------------------------------------------------------------------*/
/* file: set_figure_name_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the figure_name field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_name_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  if ( !isParameterStringMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_name") ;
    return SET_PROPERTY_ERROR ;
  }

  return sciSetName( pobj, getStringFromStack( stackPointer ), nbCol * nbRow ) ;
}
/*------------------------------------------------------------------------*/
