#include "stack-c.h"

/*--------------------------------------------------------
 * intmatmul: interface for matmul C-function 
 *     should provide C=matmul(A,B) at Scilab level 
 *--------------------------------------------------------*/

void matmul __PARAMS((double *a, int n, int m, double *b, int l, double *c));

int intmatmul(fname) 
     char *fname;
{
  static int l1, m1, n1, l2, m2, n2, l3;
  static int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;

  /* Check number of inputs (rhs=2) and outputs (lhs=1) */
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* Get A (#1) and B (#2) and create C (#3) as double ("d") matrices */
  GetRhsVar(1, "d", &m1, &n1, &l1);   /* m1, n1 (and l1) are output parameters */
  GetRhsVar(2, "d", &m2, &n2, &l2);   /* m1, n1 (and l1) are output parameters */
  CreateVar(3, "d", &m1, &n2, &l3);   /* m1 and n2 are input parameters */

  /* Check dimensions    */
  if (!(n1==m2)) { sciprint("%s: Incompatible inputs \r\n", "matmul");
    Error(999);
    return 0;}

  /* Call multiplication function 
   * inputs:stk(l1)->A, stk(l2)->B 
   *  output:stk(l3)->C               
   */
  matmul(stk(l1), m1, n1, stk(l2), n2, stk(l3));

  /*  Return C (3)  */
  LhsVar(1) = 3;
  return 0;
}

/*--------------------------------------------------------
 *  Matrix multiplication C= A*B, (A,B,C stored columnwise) 
 *  C function
 *--------------------------------------------------------*/


#define A(i,k) a[i + k*n]
#define B(k,j) b[k + j*m]
#define C(i,j) c[i + j*n]

void matmul(a,n,m,b,l,c)
     double a[],b[],c[];
     int n,m,l;
{
  int i,j,k;
  double s;
  for( i=0 ; i < n; i++){
    for( j=0; j < l; j++){
      s = 0.;
      for( k=0; k< m; k++){
	s += A(i,k)*B(k,j);
      }
      C(i,j) = s;
    }
  }
}





