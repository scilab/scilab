/*------------------------------------------------------------------------*/
/* file: get_current_figure_property.c                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the current_figure field of a    */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "CurrentObjectsManagement.h"


extern int GetTclCurrentFigure( void ) ;


/*--------------------------------------------------------------------------*/
int get_current_figure_property( sciPointObj * pobj )
{
  
  if ( get_cf_type() == 1 )
  {
    /* return handle on the current figure */
    return sciReturnHandle( sciGetHandle( sciGetCurrentFigure() ) ) ;
  }
  else
  {
    return sciReturnDouble( (double)GetTclCurrentFigure() ) ;
  }
  

}
/*--------------------------------------------------------------------------*/
