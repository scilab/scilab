#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 * Shows how to pass 
 *       - Scilab complex scalar matrices 
 *       - Scilab boolean matrices 
 *       how to create and return new complex matrices
 **************************************************/

static void f99 __PARAMS((double *,double *,int*, int *,int *));

/*------------------------------------------------------
 * A complex matrix as argument which is transmited to 
 * the routine f99, modified and returned back to Scilab.
 * When using GetRhsCVar: the returned argument it is 
 * a flag which indicated the type of the scalar matrix 
 * it==1 the matrix is complex, it==0 the matrix is real. 
 * when the matrix is complex on can access the complex part 
 * through the lc parameter. 
 * according to the second argument which can be 'i','r','d' 
 * one access data throug istk, sstk or stk 
 *------------------------------------------------------*/

int intex2c_1(fname) 
     char *fname;
{
  static int lr1,lc1,it1,m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsCVar(1, "d", &it1, &m1, &n1, &lr1,&lc1);

  f99(stk(lr1), stk(lc1), &it1, &m1, &n1);

  LhsVar(1) = 1;
  return 0;
}

static void f99( ar,ac, ita,ma,na) 
     int *ma,*na,*ita;
     double *ar,*ac;
{
  int i;
  for ( i= 0 ; i < (*ma)*(*na) ; i++) ar[i] = 2*ar[i] ;
  if ( *ita == 1) 
    for ( i= 0 ; i < (*ma)*(*na) ; i++) ac[i] = 3*ac[i] ;
}

/*------------------------------------------------------
 * A complex matrix as argument. 
 * Here we want to convert the arrays (real,imag) as 
 * a complex array. This can be done by using 'z' as 
 * the type-flag of GetRhsCVar or GetRhsVar 
 * but note that the argument here must be complex. 
 *------------------------------------------------------*/

static void f99z __PARAMS((doublecomplex *,int m,int n));

int intex2c_2(fname) 
     char *fname;
{
  static int lr1,m1, n1;
  static int minlhs=1, minrhs=1, maxlhs=1, maxrhs=1;
  doublecomplex *z;

  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  GetRhsVar(1, "z", &m1, &n1, &lr1); /* expecting a complex matrix */

  z = zstk(lr1); 
    f99z(z,m1,n1) ; 
  LhsVar(1) = 1;
  return 0;
}

static void f99z( Z, m,n )
     int m,n;
     doublecomplex *Z;
{
  int i;
  for ( i= 0 ; i < m*n ; i++) 
    {
      Z[i].r *= 2.0 ;
      Z[i].i *= 3.0 ;
    }
}

