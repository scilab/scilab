/*------------------------------------------------------------------------*/
/* file: sci_move.c                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for move routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_move.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "Interaction.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_move( char * fname, unsigned long fname_len )
{
  long hdl;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,n;
  BOOL opt;

  CheckRhs(1,3);
  /*  set or create a graphic window */
  opt = FALSE;
  if (Rhs ==3) {
    GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
    if (strcmp(cstk(l3),"alone") == 0) 
		{ 
			opt = TRUE; 
		}
    else 
		{
			Scierror(999,_("%s: Wrong third input argument: '%s' expected.\n"),fname,"alone"); 
			return 0;
		}
  }

  GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1); /* Gets the Handle passed as argument */    
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */
  n=m2*n2;
  if (n != 2&&n !=3)
  { 
    Scierror(999,_("%s: Wrong type for third input argument: vector [x y] or [x,y,z] expected.\n"),fname);
    return 0;
  }
  Objmove(&hdl,stk(l2),n,opt);

  LhsVar(1)=0;
  return 0;
}
/*--------------------------------------------------------------------------*/
