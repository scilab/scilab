/* ============================================== */
#include "stack-c.h"
#include "sciprint.h"
/* ============================================== */
/*  Interfaces for c fonctions */
/* ============================================== */
void c_sum(double *b, double *c, double *a);
void c_sub(double *b, double *c, double *a);
/* ============================================== */
int c_intsum(char *fname) 
{
  int l1=0, m1=0, n1=0, l2=0, m2=0, n2=0, l3=0, m3=1, n3=1;
  int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

  /* Check number of inputs (rhs=2) and outputs (lhs=1) */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* Get Qdata (#1) and Param (#2) and create C (#3) as double ("d") matrices */
  GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);   /* b */
  GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);   /* c */

  CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);   /* Result : a = b+c */

  /* Check dimensions  */  
  if (!(m1==1)|!(n1==1)) 
  { 
    sciprint("%s: Wrong inputs \r\n", "c_fun");
    Error(999);
    return 0;
   }
  if (!(m2==1)|!(n2==1)) 
  {
    sciprint("%s: Wrong inputs \r\n", "c_fun");
    Error(999);
    return 0;
  }

  /* Call c_fun */
  c_sum(stk(l1),stk(l2),stk(l3));

  /*  Return result  */
  LhsVar(1) = Rhs+1;
  return 0;
}
/* ============================================== */
int c_intsub(char *fname) 
{
  int l1=0, m1=0, n1=0, l2=0, m2=0, n2=0, l3=0, m3=1, n3=1;
  int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

  /* Check number of inputs (rhs=2) and outputs (lhs=1) */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* Get Qdata (#1) and Param (#2) and create C (#3) as double ("d") matrices */
  GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);   /* b */
  GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);   /* c */

  CreateVar(Rhs+1, MATRIX_OF_DOUBLE_DATATYPE, &m3, &n3, &l3);   /* Result : a = b-c */

  /* Check dimensions  */  
  if (!(m1==1)|!(n1==1)) 
  { 
    sciprint("%s: Wrong inputs \r\n", "c_fun");
    Error(999);
    return 0;
   }
  if (!(m2==1)|!(n2==1)) 
  {
    sciprint("%s: Wrong inputs \r\n", "c_fun");
    Error(999);
    return 0;
  }

  /* Call c_fun */
  c_sub(stk(l1),stk(l2),stk(l3));

  /*  Return result  */
  LhsVar(1) = Rhs+1;
  return 0;
}
/* ============================================== */
void c_sum(double *b, double *c, double *a)
{
  *a = *b + *c;
}
/* ============================================== */
void c_sub(double *b, double *c, double *a)
{
  *a = *b -*c;
}
/* ============================================== */

