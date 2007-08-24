/*------------------------------------------------------------------------*/
/* file: sci_zoom_rect.c                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for zoom_rect routine                                 */
/*------------------------------------------------------------------------*/

#include "sci_zoom_rect.h"
#include "stack-c.h"
#include "PloEch.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_zoom_rect(char *fname,unsigned long fname_len)
{
  int m,n,l;
  int x_pixel=0,y_pixel=0;
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  if (Rhs <= 0) 
  {
    zoom();
  }
  else
  {
    GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l); 
    CheckLength(1,4,m*n);
    zoom_box(stk(l),&x_pixel,&y_pixel);
  }

  LhsVar(1)=0; 
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
