/*------------------------------------------------------------------------*/
/* file: sci_xstringl.c                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstringl routine                                  */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xstringl.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "Xcall1.h"
#include "MALLOC.h"
#include "DrawObjects.h"
#include "Axes.h"
#include "GetProperty.h"
#include "CurrentObjectsManagement.h"

/*-----------------------------------------------------------------------------------*/
int sci_xstringl( char *fname, unsigned long fname_len )
{
  double rect[4],wc,dv,x,y,yi;
  integer i,j,v,verb=0,un=1,quatre=4,m4,n4,l4,m1,n1,l1,m2,n2,l2,m3,n3;
  integer m5,n5,l5;
  int font_[2], cur_font_[2];
  char **Str;

  CheckRhs(3,5);
  CheckLhs(0,1);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);

  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 
  SciWin();

  C2F(dr1)("xget","font",&verb,font_,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

  cur_font_[0] = font_[0];
  cur_font_[1] = font_[1];

  if (Rhs >= 4) { GetRhsVar(4,"i",&m4,&n4,&l4); CheckScalar(4,m4,n4); font_[0] = *istk(l4);}
  if (Rhs >= 5) { GetRhsVar(5,"i",&m5,&n5,&l5); CheckScalar(5,m5,n5); font_[1] = *istk(l5);}

  C2F(dr1)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  updateScaleIfRequired( sciGetCurrentSubWin() ) ;


  wc = 0.;
  for (i = m3 -1 ; i >= 0; --i) 
  {
    int ib = 0;
    for (j = 0 ; j < n3 ; ++j) 
    {
      strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
      ib +=  (int)strlen(Str[i+ m3*j]);
      if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
    }
    C2F(dr1)("xstringl",C2F(cha1).buf,&v,&v,&v,&v,&v,&v,&x,&y,rect,&dv,9L,bsiz);
    wc = Max(wc,rect[2]);
    if (i != 0 ) 
      y += rect[3] * 1.2;
    else 
      y += rect[3];
  }
  if (Str)
  {
	  int li=0;
	  for(li=0;li<m3*n3;li++)
	  {
		  if (Str[li])
		  {
			  FREE(Str[li]);
			  Str[li]=NULL;
		  }
	  }
	  FREE(Str);
	  Str=NULL;
  }
  CreateVar(Rhs+1,"d",&un,&quatre,&l4);
  *stk(l4) = x; 
  *stk(l4+1) = y;
  *stk(l4+2) = wc ;
  *stk(l4+3) = y-yi;

  C2F(dr1)("xset","font",&cur_font_[0],&cur_font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  LhsVar(1)=Rhs+1;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
