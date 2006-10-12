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
#include "Xcall1.h"

#ifdef WITH_TK
extern int GetTclCurrentFigure( void ) ;
#endif

/*-----------------------------------------------------------------------------------*/
int get_current_figure_property( sciPointObj * pobj )
{
  
  if ( get_cf_type() == 1 )
  {
    /* graphic window */
    C2F(sciwin)() ;
    if( version_flag() == 0 )
    {
      /* return handle on the current figure */
      return sciReturnHandle( sciGetHandle( sciGetCurrentFigure() ) ) ;
    }
    else
    {
      /* return id of the current figure */
      double *XGC,dv=0;
      int v=0;
      struct BCG * CurrentScilabXgc = NULL;
      C2F(dr)("xget","gc",&v,&v,&v,&v,&v,&v,(double *)&XGC,&dv,&dv,&dv,5L,10L); /* ajout cast ???*/
      CurrentScilabXgc = (struct BCG *) XGC ;

      return sciReturnDouble( CurrentScilabXgc->CurWindow ) ;
    }
  }
  else
  {
    
#ifdef WITH_TK
    return sciReturnDouble( (double)GetTclCurrentFigure() ) ;
#else
    return sciReturnEmptyMatrix() ;
#endif
  }
  
  return -1 ;

}
/*-----------------------------------------------------------------------------------*/
