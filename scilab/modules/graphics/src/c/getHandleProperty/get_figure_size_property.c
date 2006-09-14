/*------------------------------------------------------------------------*/
/* file: get_figure_size_property.c                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figure_size field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sciprint.h"
#include "InitObjects.h"
#include "Xcall1.h"

/*------------------------------------------------------------------------*/
int get_figure_size_property( sciPointObj * pobj )
{
  double figureSize[2] ;

  if ( sciGetEntityType (pobj) != SCI_FIGURE )
  {
    sciprint("figure_size property undefined for this object.\n") ;
    return -1;
  }

  if ( pobj != getFigureModel() )
  {
    int cur ;
    int num=pFIGURE_FEATURE(pobj)->number ;
    integer itmp = 0 ;
    integer na   = 0 ;
    integer x[2] ;
     
    C2F(dr)("xget","window",&itmp,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
    C2F(dr)("xset","window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    C2F(dr)("xget","wpdim",&itmp,x,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    C2F(dr)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

    pFIGURE_FEATURE(pobj)->figuredimwidth  = x[0] ;  
    pFIGURE_FEATURE(pobj)->figuredimheight = x[1] ; 
  }
  figureSize[0] = pFIGURE_FEATURE(pobj)->figuredimwidth;
  figureSize[1] = pFIGURE_FEATURE(pobj)->figuredimheight;

  return sciReturnRowVector( figureSize, 2 ) ;

}
/*------------------------------------------------------------------------*/
