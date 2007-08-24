/*------------------------------------------------------------------------*/
/* file: sci_xclick.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xclick routine                                    */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_xclick.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"

/*-----------------------------------------------------------------------------------*/
int sci_xclick(char *fname,unsigned long fname_len)
{
  int un=1,trois=3,rep;
  int iw = 0;
  integer istr;
  int ix = 0;
  int iflag = 0;
  double x = 0.0;
  double y = 0.0;

  CheckRhs(-1,1) ;
  CheckLhs(1,5) ;

  iflag = ( Rhs >= 1) ? 1 :0;


  switch (Lhs) {
  case 4 : 
    /* C2F(dr1)("xclickany","xv",&ix,&iw,&iflag,&v,&v,&v,&x,&y,&dv,&dv,10L,3L); */
    break;
  case 5 :
    istr = 1;
    /* C2F(dr1)("xclickany",C2F(cha1).buf,&ix,&iw,&iflag,&v,&v,&istr,&x,&y,&dv,&dv,10L,bsiz); */
    break;
  default :
    istr = 0;
    /* C2F(dr1)("xclick","xv",&ix,&iflag,&istr,&v,&v,&v,&x,&y,&dv,&dv,7L,3L); */
  }

  if ( ix < 0 ) {
    x=y=-1; /* to better manage string conversion of the output */ /* F.Leray 20.04.05 */
  }

  if ( Lhs == 1 )
  {
    LhsVar(1) = Rhs+1;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&trois,&rep);
    *stk(rep) = (double) ix; *stk(rep + 1) = x; *stk(rep + 2) = y;
  }
  else 
  {
    LhsVar(1) = Rhs+1; CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&rep); *stk(rep) = (double) ix ;
  }
  if ( Lhs >= 2) 
  { LhsVar(2) = Rhs+2; CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&rep); *stk(rep) = x ;} 
  if ( Lhs >= 3)
  { LhsVar(3) = Rhs+3; CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&rep); *stk(rep) = y ;}
  if ( Lhs >=4 ) 
  { LhsVar(4) = Rhs+4; CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE,&un,&un,&rep); *stk(rep) = (double) iw ;}
  if ( Lhs >= 5) 
  {
    if (istr == 0) { istr = 4; strcpy(C2F(cha1).buf,"void");}
    LhsVar(5) = Rhs+5; CreateVar(Rhs+5,STRING_DATATYPE,&istr,&un,&rep); 
    strncpy(cstk(rep),C2F(cha1).buf,istr);
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
