#include <string.h>

#include "stack-c.h"

typedef void (*voidf)();

typedef struct {
  char *name;
  voidf f;
} FTAB;

/***********************************
 * Table of predefined functions f1f and f2f
 ***********************************/

#if defined(__STDC__)
#define ARGS_ex17f double *,double*, double *
#else
#define ARGS_ex17f 
#endif 

typedef int (*funcex)(ARGS_ex17f);
extern int C2F(f1f)(ARGS_ex17f);
extern int C2F(f2f)(ARGS_ex17f);

extern int C2F(ex17f) __PARAMS((double *,integer *,double *,integer *,double *, funcex));
 
FTAB FTab_ex17f[] ={
  {"f1f", (voidf) C2F(f1f)},
  {"f2f", (voidf) C2F(f2f)},
  {(char *) 0, (voidf) 0}};

/***************************************************
 * deal with errors in scilab functions 
 ***************************************************/

#include <setjmp.h>
static  jmp_buf ex17fenv; 

/***************************************************
 * data for interface 
 ***************************************************/

static int sci_f, lhs_f, rhs_f;

/***************************************************
 * Functions 
 ***************************************************/

static int sciex17f (ARGS_ex17f);
static funcex Argex17f;

/***************************************************
 * intex17f : interface for ex17f 
 ***************************************************/

int intex17f(fname) 
     char *fname;
{ 
  int returned_from_longjump ;
  int m_X,n_X,l_X,m_Y,n_Y,l_y,m_Z,n_Z,l_Z;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;

  /*   Check rhs and lhs   */  
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /*   Variable #1 (X = real vector)   */
  GetRhsVar(1, "d", &m_X, &n_X, &l_X);

  /*   Variable #2 (Y = real vector)   */
  GetRhsVar(2, "d", &m_Y, &n_Y, &l_y);

  /*   ex17c(X,Y,f) 
   *   if f is a string we search for f in Tables and on succes 
   *         Argex17c will be the function to be called  
   *   if f is a macro then sciex17f will be called (see its definition bellow) 
   *         returned arguments are sci_f, lhs_f, rhs_f 
   *         function position in the stack sci_f required lhs and rhs in lhs_f, rhs_f 
   *         (we do not use lhs_f and rhs_f in this example)
   */

  Argex17f  = (funcex) GetFuncPtr("ex17f", 3, FTab_ex17f,
				  (voidf) sciex17f, &sci_f, &lhs_f, &rhs_f);
  if ( Argex17f == (funcex) 0 ) return 0;
  
  m_Z= m_X*n_X;  n_Z= m_Y*n_Y;
  /*  Creating the output variable Z (#4) , real matrix variable with m_Z rows and n_Z columns    */
  CreateVar(4, "d", &m_Z, &n_Z, &l_Z);

  /* If an error occurs while Z is calculated ... */
  if (( returned_from_longjump = setjmp(ex17fenv)) != 0 )
    {
      Scierror(999,"%s: Internal error \r\n",fname);
      return 0;
    }
  
  /*  Now matrix Z is populated i.e. appropriate values are 
      assigned to stk(l_Z)[0] ( = Z(1,1) ), stk(l_Z)[1]  ( = Z(2,1) ), ... */
  C2F(ex17f)(stk(l_X), &m_Z,stk(l_y), &n_Z, stk(l_Z), Argex17f);

  /*  Variable #4 is returned to Scilab  */
  LhsVar(1) = 4;
  return 0;
}

static int sciex17f(x,y,z)
     double *x,*y,*z;
     /* Computing z=f(x,y), f being the Scilab argument function */
     /* C function emulating the Scilab function pointed to by sci_f  */
{
  static int lhs=1,rhs=2;
  int scilab_i,scilab_j, un=1;
  /* Inputs (x(i),y(j)) at positions  (5,6) */
  CreateVar(5, "d", &un, &un, &scilab_i);
  stk(scilab_i)[0] = *x;

  CreateVar(6, "d", &un, &un, &scilab_j);
  stk(scilab_j)[0] = *y;

  /* executes the Scilab function (f) pointed to by sci_f. 
   * we provide a rhs=2 and expect lhs = 1 
   */

  PExecSciFunction(5, &sci_f, &lhs, &rhs, "ArgFex", ex17fenv);
  /* One output at position of first input (5) */
  *z = *stk(scilab_i);
  return 0;
}

