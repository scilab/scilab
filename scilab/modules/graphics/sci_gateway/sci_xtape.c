/*------------------------------------------------------------------------*/
/* file: sci_xtape.c                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xtape routine                                     */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xtape.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "Xcall1.h"

/*-----------------------------------------------------------------------------------*/
int sci_xtape( char *fname,unsigned long fname_len )
{
  static double  rect_def[4] = { 0,0,10,10}, ebox_def[6] = {0,1,0,1,0,1};
  static integer iflag_def[4] = { 0,0,0,0 };
  static integer aint_def[4] = { 0,0,0,0 };
  static integer iscflag_def[2] = { 1,0 };
  static integer flagx_def[3] = { 1,1,1} ;
  integer *iflag = iflag_def,*aint = aint_def,*iscflag = iscflag_def, *flagx= flagx_def,num,v;
  double alpha = 35.0 ,theta = 45.0,  *rect = rect_def ,*ebox = ebox_def , dv;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5,m6,n6,l6,m7,n7,l7;

  CheckRhs(1,7);

  /* NG beg */
  if (version_flag() == 0) {
    Scierror(999,"%s: not implemented with new figure style\r\n",fname);
    return 0;
    /* call a Scilab function to handle compatibility */
  }/* NG end */

  /* first argument is a string */
  GetRhsVar(1,"c",&m1,&n1,&l1);

  if ( strcmp(cstk(l1),"on") == 0) 
  {
    CheckRhs(1,1);
    C2F(dr)("xsetdr","Rec",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,4L);
  }
  else if ( strcmp(cstk(l1),"clear") == 0)
  {
    CheckRhs(2,2);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
    C2F(dr)("xstart","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
  }
  else if (strcmp(cstk(l1),"replay") == 0) 
  {
    CheckRhs(2,2);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
    C2F(dr)("xreplay","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  }
  else if (strcmp(cstk(l1),"replaysh") == 0) 
  {
    CheckRhs(2,2);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
    C2F(dr)("xreplaysh","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  }
  else if (strcmp(cstk(l1),"replaysc") == 0) 
  {
    CheckRhs(2,5);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
    /*     s'il n'y a que trois argument le 3ieme est rect[4] */
    if (Rhs == 3) { GetRhsVar(3,"d",&m3,&n3,&l3); CheckLength(3,m3*n3,4); rect = stk(l3); }
    else if ( Rhs > 3 ) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3); CheckLength(3,m3*n3,2); iscflag = istk(l3);
      if ( Rhs >=4 ) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckLength(4,m4*n4,4); rect = stk(l4); }
      if ( Rhs >=5 ) { GetRhsVar(5,"i",&m5,&n5,&l5); CheckLength(5,m5*n5,4); aint = istk(l5);}       
    }
    C2F(dr)("xreplaysc","v",&num,iscflag,&v,aint,&v,&v,rect,&dv,&dv,&dv,10L,2L);
  }
  else if (strcmp(cstk(l1),"replayna") == 0) 
  {
    CheckRhs(2,5);
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
    if ( Rhs >= 3 ) {GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3); theta = *stk(l3);}
    if ( Rhs >= 4 ) {GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); alpha = *stk(l4);}
    if ( Rhs >= 5 ) {GetRhsVar(5,"i",&m5,&n5,&l5); CheckLength(5,m5*n5,4); iflag = istk(l5);}
    if ( Rhs >= 6 ) {GetRhsVar(6,"i",&m6,&n6,&l6); CheckLength(6,m6*n6,3); flagx = istk(l6);}
    if ( Rhs >= 7 ) {GetRhsVar(7,"d",&m7,&n7,&l7); CheckLength(7,m7*n7,6); ebox = stk(l7);}
    C2F(dr)("xreplayna","v",&num,&v,&v,iflag,flagx,&v,&theta,&alpha,ebox,&dv,10L,2L);
  }
  else 
  {
    Scierror(999,"%s: first argument has a wrong value %s\r\n",fname,cstk(l1));
    return 0;
  }
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/