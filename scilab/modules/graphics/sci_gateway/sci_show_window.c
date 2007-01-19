/*------------------------------------------------------------------------*/
/* file: sci_show_window.c                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for show_window routine                               */
/*------------------------------------------------------------------------*/

#include "sci_show_window.h"
#include "Xcall1.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
int sci_show_window( char *fname,unsigned long fname_len )
{
  integer v;
  double dv;
  CheckRhs(-1,0);
  C2F(dr)("xselect","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
