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
#include "InitObjects.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/
int set_figure_id_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  int id = (int) getDoubleFromStack( stackPointer ) ;
  if ( sciGetEntityType(pobj) != SCI_FIGURE )
  {
    sciprint("figure_id property undefined for this object") ;
    return -1;
  }

  if ( pobj != getFigureModel() )
  {
    if ( sciInitUsedWindow( id ) != 0 )
    {
      sciprint( "It was not possible to create the requested figure.\n" ) ;
      return -1 ;
    }
  }
  else
  {
    return sciSetNum( getFigureModel(), &id ) ;
  }
}
/*------------------------------------------------------------------------*/
