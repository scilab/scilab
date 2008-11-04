/* ==================================================================== */
/* Allan CORNET */
/* INRIA 2008 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */
#include "stack-c.h" 
/* ==================================================================== */
extern int csub(double *a,double *b,double *c);
/* ==================================================================== */
int sci_csub(char *fname)
{
  int l1, m1, n1, l2, m2, n2, m3, n3,l3;

  double a,b,c;
  
  a = 0;
  b = 0;
  c = 0;

  /* --> result = csum(3,8)
  /* check that we have only 2 parameters input */
  /* check that we have only 1 parameters output */
  CheckRhs(2,2) ;
  CheckLhs(1,1) ;   

  /* get first parameter and put in 'a' */
  GetRhsVar(1, "d", &m1, &n1, &l1);
  a = *stk(l1);
  
  /* get second parameter and put in 'a' */
  GetRhsVar(2, "d", &m2, &n2, &l2);
  b= *stk(l2) ;
 
  /* call fortran fsum subroutine */
  csub(&a,&b,&c);
  
  /* create a variable on scilab's stack */
  m3=1;
  n3=1;
  CreateVar(Rhs+1,"d",&m3,&n3,&l3);
  *stk(l3) = c;

  LhsVar(1) = Rhs+1; 
  return 0;
}
/* ==================================================================== */

