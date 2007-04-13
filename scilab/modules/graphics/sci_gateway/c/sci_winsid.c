/*------------------------------------------------------------------------*/
/* file: sci_winsid.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for winsid routine                                    */
/*------------------------------------------------------------------------*/

#include "sci_winsid.h"
#include "stack-c.h"
#include "periScreen.h"
#undef Top
#include "WindowList.h"

/*-----------------------------------------------------------------------------------*/
int sci_winsid(char *fname,unsigned long fname_len)
{
  integer iflag =0,ids,num,un=1,l1;

  CheckRhs(-1,0) ;
  getWins(&num,&ids ,&iflag);
  CreateVar(Rhs+1,"i",&un,&num,&l1);
  iflag = 1; 
  getWins(&num,istk(l1),&iflag);
  LhsVar(1)=Rhs+1;

  return 0;
}
/*-----------------------------------------------------------------------------------*/
