/*------------------------------------------------------------------------*/
/* file: set_figure_id_property.c                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the figure_id field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "InitObjects.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_figure_id_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int id = (int) getDoubleFromStack( stackPointer ) ;

  if ( !isParameterDoubleMatrix( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"figure_id") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("figure_id property undefined for this object.\n") ;
    return SET_PROPERTY_ERROR ;
  }

  if ( pobj != getFigureModel() )
  {
    return sciInitUsedWindow( id ) ;
  }
  else
  {
    return sciSetNum( getFigureModel(), id ) ;
  }
  return SET_PROPERTY_ERROR ;
}
/*------------------------------------------------------------------------*/
