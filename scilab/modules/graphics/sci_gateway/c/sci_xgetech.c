/*------------------------------------------------------------------------*/
/* file: sci_xgetech.c                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xgetech routine                                   */
/*------------------------------------------------------------------------*/

#include "sci_xgetech.h"
#include "stack-c.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "PloEch.h"

/*-----------------------------------------------------------------------------------*/
int sci_xgetech( char *fname, unsigned long fname_len )
{
  double WRect[4],FRect[4],ARect[4];
  double *W= WRect, *F = FRect, *A= ARect ;
  int un=1,deux=2,quatre=4,i,l1,l2,l3,l4;
  char logFlags[2], *L=logFlags ;

  CheckRhs(0,0);
  CheckLhs(1,4);
  if ( Lhs >=1 ) { CreateVar( 1,MATRIX_OF_DOUBLE_DATATYPE, &un, &quatre,&l1); W= stk(l1);}
  if ( Lhs >=2 ) { CreateVar( 2,MATRIX_OF_DOUBLE_DATATYPE, &un, &quatre,&l2); F= stk(l2);}
  if ( Lhs >=3 ) { CreateVar( 3,STRING_DATATYPE, &un, &deux,  &l3); L= cstk(l3);}
  if ( Lhs >=4 ) { CreateVar( 4,MATRIX_OF_DOUBLE_DATATYPE, &un, &quatre,&l4); A= stk(l4);}
  SciWin();
  getscale2d(W,F,L,A);
  for ( i = 1 ; i <= Lhs ; i++) LhsVar(i) = i;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
