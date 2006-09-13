/*------------------------------------------------------------------------*/
/* file: get_figures_id_property                                          */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : function to retrieve in Scilab the figures_id field of a handle */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "stack-c.h"
#include "GetProperty.h"

/*------------------------------------------------------------------------*/

int get_figures_id_property( sciPointObj * pobj )
{
  int numRow   = 1 ;
  int numCol   = 0 ;
  int iflag    = 0 ;
  int outIndex = 0 ;
  int ids      = 0 ;

  sciGetIdFigure (&ids,&numCol,&iflag);
  CreateVar(Rhs+1,"i",&numRow,&numCol,&outIndex);
  iflag = 1; 
  sciGetIdFigure (istk(outIndex),&numCol,&iflag) ;

  return 0 ;

}

/*------------------------------------------------------------------------*/