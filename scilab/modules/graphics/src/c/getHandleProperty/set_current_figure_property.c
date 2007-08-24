/*------------------------------------------------------------------------*/
/* file: set_current_figure_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the current_figure field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/*------------------------------------------------------------------------*/
int set_current_figure_property( sciPointObj * pobj, int stackPointer, int valueType, int nbRow, int nbCol )
{
  int figNum = -1 ;
  int res = -1 ;

  if ( isParameterHandle( valueType ) )
  {
    sciPointObj * curFig = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;
    
    if ( curFig == NULL )
    {
      sciprint( "Object is not valid.\n" ) ;
      return -1 ;
    }
    if ( sciGetEntityType( curFig ) != SCI_FIGURE )
    {
      sciprint("Object is not a handle on a figure.\n");
      return -1;
    }
    figNum = sciGetNum( curFig ) ;
  }
  else if ( isParameterDoubleMatrix( valueType ) )
  {
    figNum = (int) getDoubleFromStack( stackPointer ) ;
  }
  else
  {
    sciprint("Bad argument to determine the current figure: should be a window number or a handle (available under new graphics mode only).\n") ;
    return -1 ;
  }

  /* select the figure num */
  res = sciSetUsedWindow( figNum ) ;
  if ( res < 0 )
  {
    sciprint("It was not possible to create the requested figure.\n");
  }
  return res ;

}
/*------------------------------------------------------------------------*/
