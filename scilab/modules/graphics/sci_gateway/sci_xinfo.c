/*------------------------------------------------------------------------*/
/* file: sci_xinfo.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xinfo routine                                     */
/*------------------------------------------------------------------------*/

#include "sci_xinfo.h"
#include "Xcall1.h"
#include "stack-c.h"

/*-----------------------------------------------------------------------------------*/
int sci_xinfo( char * fname,unsigned long fname_len )
{
  double dv;
  integer m1,n1,l1,v;
  CheckRhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  C2F(dr)("xinfo",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,m1);
  LhsVar(1) =0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
