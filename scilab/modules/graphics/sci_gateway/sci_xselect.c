/*------------------------------------------------------------------------*/
/* file: sci_xselect.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xselect routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xselect.h"
#include "Graphics.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
int sci_xselect( char *fname,unsigned long fname_len )
{
  integer v;
  double dv;
  CheckRhs(-1,0);
  C2F(dr1)("xselect","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
