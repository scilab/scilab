/*------------------------------------------------------------------------*/
/* file: sci_xtring.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xstring routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xstring.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "sciCall.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "MALLOC.h"

/*-----------------------------------------------------------------------------------*/
int sci_xstring( char *fname, unsigned long fname_len )
{
  double rect[4],wc,x,y,yi,angle=0.0;
  integer flagx=0;
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str;
  long hdlstr ;
  BOOL isboxed = FALSE;

  CheckRhs(3,5);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
  CheckScalar(1,m1,n1);
  x = *stk(l1);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  CheckScalar(2,m2,n2);
  yi = y = *stk(l2);
  GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4)
  {
    GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE,&m4,&n4,&l4);
    CheckScalar(4,m4,n4);
    angle = *stk(l4 );
  }
  if (Rhs >= 5)
  { 
    GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE,&m5,&n5,&l5);
    CheckScalar(5,m5,n5);
    flagx = (integer) *stk(l5);
  }

  SciWin();
  wc = 0.;/* to keep the size of the largest line */


  if ( (flagx == 1) && (*stk(l4) == 0))
  {
    isboxed = TRUE;
  }

  /* create the object */
  Objstring ( Str,m3,n3,x,y,&angle,rect,TRUE,NULL,&hdlstr,FALSE,NULL,NULL,isboxed,TRUE,FALSE, ALIGN_LEFT ) ;


  /* we must free Str memory */ 
  if (Str)
  {
	  int i=0;
	  for(i=0;i<m3*n3;i++)
	  {
		  if (Str[i])
		  {
			  FREE(Str[i]);
			  Str[i]=NULL;
		  }
	  }
	  FREE(Str);
	  Str=NULL;
  }

  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
