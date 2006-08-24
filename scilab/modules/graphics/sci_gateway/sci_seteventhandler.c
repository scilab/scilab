/*------------------------------------------------------------------------*/
/* file: sci_seteventhandler.c                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for seteventhandler routine                           */
/*------------------------------------------------------------------------*/

#include "sci_seteventhandler.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "Xcall1.h"
#include "Events.h"

/*-----------------------------------------------------------------------------------*/
int sci_seteventhandler( char *fname,unsigned long fname_len )
{
  integer m1,n1,l1,ierr;
  integer verb=0,win,na,v;
  double dv;
  SciWin();
  CheckRhs(1,1);
  CheckLhs(0,1);
  C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  if (Rhs == 1) 
  {
    GetRhsVar(1,"c",&m1,&n1,&l1);
    seteventhandler(&win,cstk(l1),&ierr);
  }
  else
  {
    seteventhandler(&win,"",&ierr);
  }
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
