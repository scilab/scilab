/*------------------------------------------------------------------------*/
/* file: sci_xtring.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xstring.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "dr1Call.h"
#include "BuildObjects.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
int sci_xstring( char *fname, unsigned long fname_len )
{
  double rect[4],wc,x,y,yi,angle=0.0;
  integer i,j,iv=0,flagx=0;
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str;
  long hdlstr ;

  CheckRhs(3,5);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4)
  {
    GetRhsVar(4,"d",&m4,&n4,&l4);
    CheckScalar(4,m4,n4);
    angle = *stk(l4 );
  }
  if (Rhs >= 5)
  { 
    GetRhsVar(5,"d",&m5,&n5,&l5);
    CheckScalar(5,m5,n5);
    flagx = (integer) *stk(l5);
  }

  SciWin();
  wc = 0.;/* to keep the size of the largest line */

  if ( version_flag() == 0 )
  {
    BOOL isboxed = FALSE;

    if ( (flagx == 1) && (*stk(l4) == 0))
    {
      isboxed = TRUE;
    }

    /* create the object */
    Objstring ( Str,m3,n3,x,y,&angle,rect,TRUE,NULL,&hdlstr,FALSE,NULL,NULL,isboxed,TRUE,FALSE, ALIGN_LEFT ) ;

  }
  else {
    for (i = m3 -1 ; i >= 0; --i) {
      int ib = 0;
      for (j = 0 ; j < n3 ; ++j) {
        strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
        ib += strlen(Str[i+ m3*j]);
        if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
      Xstring (C2F(cha1).buf,bsiz,iv,x,y,angle,rect);
      wc = Max(wc,rect[2]);
      if (i != 0 )
      {
        y += rect[3] * 1.2;
      }
      else
      {
        y += rect[3];
      }
    }
    if (flagx == 1)
    {
      double dx1 = y - yi;
      Xrect ("xrect",6L,&x,&y,&wc,&dx1); 
    }
  } 

  /* we must free Str memory */ 
  FreeRhsSVar(Str);

  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/