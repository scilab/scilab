#include <string.h>
#include "stack-c.h"

typedef void (*voidf)();

typedef struct {
  char *name;
  voidf f;
} FTAB;

/*******************************************
 * Table of predefined functions f1c and f2c
 *******************************************/

#define ARGS_ex15c double,double, double *

typedef int (*funcex) __PARAMS((ARGS_ex15c));
extern int f1c __PARAMS((ARGS_ex15c));
extern int f2c __PARAMS((ARGS_ex15c));
extern int ex15c __PARAMS((double *,integer ,double *,integer,double *, funcex));
 
FTAB FTab_ex15c[] ={
  {"f1c", (voidf) f1c},
  {"f2c", (voidf) f2c},
  {(char *) 0, (voidf) 0}};

/***************************************************
 * deal with errors in scilab functions 
 ***************************************************/

#include <setjmp.h>
static  jmp_buf ex15cenv; 

/***************************************************
 * data for interface 
 ***************************************************/

static int sci_f, lhs_f, rhs_f;

/***************************************************
 * Functions 
 ***************************************************/

static int sciex15c __PARAMS((ARGS_ex15c));
static funcex Argex15c;

/***************************************************
 * intex15c interface for ex15c 
 ***************************************************/

int intex15c(fname) 
     char *fname;
{ 
  int returned_from_longjump ;
  int m_X,n_X,l_X,m_Y,n_Y,l_Y,m_Z,n_Z,l_Z;
  static int minlhs=1, minrhs=3, maxlhs=1, maxrhs=3;

  /*   Check rhs and lhs   */  
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /*   Variable #1 (X = real vector)   */
  GetRhsVar(1, "d", &m_X, &n_X, &l_X);

  /*   Variable #2 (Y = real vector)   */
  GetRhsVar(2, "d", &m_Y, &n_Y, &l_Y);

  /*   Variable #3 (f = "argument function")   */
  /*   ex15c(X,Y,f) 
   *   if f is a string we search for f in Tables and on succes 
   *         Argex15c will be the function to be called  
   *   if f is a macro then sciex15c will be called (see its definition bellow) 
   *         returned arguments are sci_f, lhs_f, rhs_f 
   *         function position in the stack sci_f required lhs and rhs in lhs_f, rhs_f 
   *         (we do not use lhs_f and rhs_f in this example)
   */

  Argex15c  = (funcex) GetFuncPtr("ex15c", 3, FTab_ex15c, (voidf) sciex15c, &sci_f, &lhs_f, &rhs_f);
  if ( Argex15c == (funcex) 0 ) return 0;
  
  m_Z= m_X*n_X;  n_Z= m_Y*n_Y;

  /*  Creating the output variable Z (#4) , real matrix variable with m_Z rows and n_Z columns    */

  CreateVar(4, "d", &m_Z, &n_Z, &l_Z);
  
  /* If an error occurs while Z is calculated ... */

  if (( returned_from_longjump = setjmp(ex15cenv)) != 0 )
    {
      Scierror(999,"%s: Internal error \r\n",fname);
      return 0;
    } 
  
  /*  Now matrix Z is populated i.e. appropriate values are 
   *  assigned to stk(l_Z)[0] ( = Z(1,1) ), stk(l_Z)[1]  ( = Z(2,1) ), ... 
   */

  ex15c(stk(l_X), m_X*n_X, stk(l_Y), m_Y*n_Y, stk(l_Z), Argex15c);

  /*  Variable #4 is returned to Scilab  */
  LhsVar(1) = 4;
  return 0;
}

/*---------------------------------------------------------------
 *  Computing z=f(x,y), f being the Scilab argument function 
 * C function emulating the Scilab function pointed to by sci_f 
 *---------------------------------------------------------------*/

static int sciex15c(x, y, z)
     double x, y, *z;
{
  static int lhs=1,rhs=2;
  int scilab_i,scilab_j, un=1;
  /* Inputs (x(i),y(j)) at positions  (5,6) */
  CreateVar(5, "d", &un, &un, &scilab_i);
  stk(scilab_i)[0] = x;

  CreateVar(6, "d", &un, &un, &scilab_j);
  stk(scilab_j)[0] = y;

  /* executes the Scilab function (f) pointed to by sci_f. 
   * we provide a rhs=2 and expect lhs = 1 
   */
  PExecSciFunction(5, &sci_f, &lhs, &rhs, "ArgFex", ex15cenv);
  /* One output at position of first input (5) */
  *z = *stk(scilab_i);
  return 0;
}


/*---------------------------------------------------------------
 * function which calculates Z=ex15c(X,Y,f) : given x, y and f, 
 *  the array z is such that   z[i,j]=f(x[i],y[j]) 
 *---------------------------------------------------------------*/

int ex15c(x, nx, y, ny, z, f)
     double *x,*y,*z;
     int nx,ny;
     funcex f;
{
  int i,j;
  for ( i = 0 ; i < nx ; i++ ) 
    for ( j = 0 ; j < ny ; j++) 
	(*f)(x[i], y[j], &z[i+nx*j]);
  return 0;
}

/*---------------------------------------------------------------
 * Two examples of argument functions f. Scilab usage:
 *  Z=ex15c(X,Y,"f1c") 
 *  or
 *  Z=ex15c(X,Y,"f2c")     
 *---------------------------------------------------------------*/

int f1c(x,y,z) 
     double x,y,*z;
{
  *z= x+y;
  return 0;
}

int f2c(x,y,z) 
     double x,y,*z;
{
  *z= x*x+y*y;
  return 0;
}




