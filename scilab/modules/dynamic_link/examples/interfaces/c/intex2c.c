/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file is released into the public domain
*/

#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

int ex2c (double *, int *,int *,double *, int *,int *,int *);

/**************************************************
 * An example of an hand written interface 
 **************************************************/

int intex2c(char* fname)
{ 
  int m1,n1,l1,ierr=0;
  /** optional names must be stored in alphabetical order in opts **/
  static rhs_opts opts[]= { {-1,"v1","d",0,0,0},
		            {-1,"v2","d",0,0,0},
		            {-1,NULL,NULL,0,0}
  };
  int minrhs = 1,maxrhs = 1,minlhs=1,maxlhs=3,nopt,iopos;

  nopt = NumOpt();

  CheckRhs(minrhs,maxrhs+nopt) ;
  CheckLhs(minlhs,maxlhs) ;

  /** first non optional argument **/

  GetRhsVar( 1,STRING_DATATYPE, &m1, &n1, &l1);
  
  if ( get_optionals(fname,opts) == 0) return 0;

  /** default values if optional arguments are not given :  v1=[99] and v2=[3] **/
  iopos=Rhs ;
  if ( opts[0].position  == -1 ) {
    iopos++ ; opts[0].position = iopos;
    opts[0].m = opts[0].n = 1; opts[0].type = "d";
    CreateVar(opts[0].position,opts[0].type,&opts[0].m,&opts[0].n,&opts[0].l);
    *stk(opts[0].l)=99.0;
  }
  if ( opts[1].position  == -1 ) {
    iopos++ ; opts[1].position = iopos;
    opts[1].m = opts[1].n = 1; opts[1].type = "d";
    CreateVar(opts[1].position,opts[1].type,&opts[1].m,&opts[1].n,&opts[1].l);
    *stk(opts[1].l)= 3;
  }

  ex2c(stk(opts[0].l),&opts[0].m,&opts[0].n,
       stk(opts[1].l),&opts[1].m,&opts[1].n,&ierr);

  if (ierr > 0) 
    {
      Scierror(999,"%s: Internal error\n",fname);
      return 0;
    }
  /** return the first argument (unchanged ) then v1 and v2 **/
  LhsVar(1) = 1;
  LhsVar(2) = opts[0].position;
  LhsVar(3) = opts[1].position;
  return 0;
}

int ex2c( double *a,int *ma,int *na, double *b,int *mb,int *nb,int *err) 
{
  int i;
  *err=0;
  for ( i= 0 ; i < (*ma)*(*na) ; i++) a[i] = 2*a[i] ;
  for ( i= 0 ; i < (*mb)*(*nb) ; i++) b[i] = 3*b[i] ;
  return(0);
}

