/*------------------------------------------------------------------------*/
/* file: set_current_axes_property.c                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to modify in Scilab the current_axes field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "Xcall1.h"

/*------------------------------------------------------------------------*/
int set_current_axes_property( sciPointObj * pobj, int stackPointer, int nbRow, int nbCol )
{
  sciPointObj * curAxes   = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) );
  sciPointObj * parentFig = NULL ;
  int num = -1 ;
  int v    = 1 ;
  
  if ( curAxes == NULL)
  {
    sciprint("Object is not valid.\n") ;
    return -1 ;
  }
  if ( sciGetEntityType( curAxes ) != SCI_SUBWIN )
  {
    sciprint("Object is not an Axes Entity.\n") ;
    return -1 ;
  }

  sciSetSelectedSubWin( curAxes ) ;
  /* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
  /* we must also set the current figure to subwin->parent */
  parentFig = sciGetParentFigure( curAxes );

  num  = sciGetNum( parentFig ) ;
  C2F(dr1)("xset","window",&num,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);

  if ( sciSwitchWindow(&num) != 0 )
  {
    sciprint("An error occured during window creation. Action can not be performed.\n");
    return -1;
  }
  /* End modif. on the 11.02.05 */
  return -1 ;

}
/*------------------------------------------------------------------------*/
