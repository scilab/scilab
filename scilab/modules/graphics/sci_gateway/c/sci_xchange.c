/*------------------------------------------------------------------------*/
/* file: sci_xchange.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xchange routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xchange.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "PloEch.h"

/*-----------------------------------------------------------------------------------*/
int sci_xchange( char * fname, unsigned long fname_len )
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,l4,l5,i;
  integer four = 4 ;
  integer one  = 1 ;
  CheckRhs(3,3);
  CheckLhs(1,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  GetRhsVar(3,"c",&m3,&n3,&l3);
  CheckSameDims(1,2,m1,n1,m2,n2);

  SciWin();
  if ( strcmp(cstk(l3),"i2f") == 0) 
  {
    GetRhsVar(1,"i",&m1,&n1,&l1);
    GetRhsVar(2,"i",&m2,&n2,&l2);

    CreateVar(Rhs+3,"d",&m1,&n1,&l3);
    CreateVar(Rhs+4,"d",&m1,&n1,&l4);

    C2F(echelle2d)(stk(l3),stk(l4),istk(l1),istk(l2),&m1,&n1,"i2f",3L);
  }
  else 
  {
    CreateVar(Rhs+3,"i",&m1,&n1,&l3);
    CreateVar(Rhs+4,"i",&m1,&n1,&l4);

    C2F(echelle2d)(stk(l1),stk(l2),istk(l3),istk(l4),&m1,&n1,"f2i",3L);
  }

  CreateVar(Rhs+5,"d",&one,&four,&l5);

  for (i=0; i < four ; i++) {*stk(l5+i) =  Cscale.WIRect1[i]; }
  LhsVar(1)=Rhs+3;
  LhsVar(2)=Rhs+4;
  LhsVar(3)=Rhs+5;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
