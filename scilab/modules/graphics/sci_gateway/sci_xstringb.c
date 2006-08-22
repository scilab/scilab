/*------------------------------------------------------------------------*/
/* file: sci_xstringb.c                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstringb routine                                  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xstringb.h"
#include "stack-c.h"
#include "sci_demo.h"
#include "BuildObjects.h"
#include "Scierror.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "sciCall.h"
#include "Graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xstringb(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5,m6,n6,l6;
  BOOL autoSize = TRUE ;
  double x,y,w,hx;
  char **Str;
  double rect[4],angle=0;
  long hdlstr;

  SciWin();

  if ( Rhs <= 0 )
  {
    /* demo */
    int one = 1 ;
    char demo[] = "scf(); axes = gca() ; axes.axes_visible = 'on' ; str = ['Scilab','is';'not','Esilab']; xstringb(0.1,0.1,str,0.5,0.5,'fill') ; txt = gce() ; txt.box = 'on' ;" ;
    sci_demo( fname, demo, &one ) ;
    return 0 ;
  }

  CheckRhs(5,6);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);  w = *stk(l4);
  GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5);  hx = *stk(l5);

  if (Rhs == 6)
  {
    GetRhsVar(6,"c",&m6,&n6,&l6) ;
    if ( m6*n6 !=0 && strcmp(cstk(l6),"fill") == 0 )
    {
      autoSize = FALSE ;
    } 
    else
    {
      Scierror(999,"%s: optional argument has a wrong value 'fill' expected\r\n",
        fname);
      return 0;
    }
  }

  if ( version_flag() == 0 )
  {
    double userSize[2] = { w, hx } ;
    Objstring (Str,m3,n3,x,y,&angle,rect,autoSize,userSize,&hdlstr,TRUE,NULL,NULL,FALSE,TRUE,FALSE,ALIGN_CENTER);
  }
  else 
  { /* NG end */
    int v ;
    int i ;
    int j ;
    int ib = 0;
    for (i = 0 ; i < m3 ; ++i) {
      for (j = 0 ; j < n3; ++j) 
      {
        strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
        ib += strlen(Str[i+ m3*j]);
        if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
      C2F(cha1).buf[ib]= '\n'; ib++;
    }
    C2F(cha1).buf[ib-1]='\0';
    C2F(dr1)("xstringb",C2F(cha1).buf,&autoSize,&v,&v,&v,&v,&v,&x,&y,&w,&hx,9L,bsiz);
  }

  LhsVar(1)=0;

  FreeRhsSVar(Str); 

  return 0;

}
/*-----------------------------------------------------------------------------------*/