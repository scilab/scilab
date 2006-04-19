#include "stack-c.h"

extern void fun2(double * , int, int *);
extern void fun1(double * ,  int, int *, double **, double *);

int intfun1(char *fname)
{
  int l1, m1, n1, m=1, nX, i, l2, l3;
  double * X, S;
 
  /* 1 - Check the number of inputs and outputs arguments */
  /* You can use the variables: Lhs and Rhs */
  int minlhs=1, maxlhs=2, minrhs=1, maxrhs=1;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* 2 - Check rhs type, get the rows number(m1) and the columns number(n1) of rhs, and its adress(l1) in the Scilab stack */
  GetRhsVar(1, "d", &m1, &n1, &l1);
 
  if(m1!=0 && n1!=0 )
  {
    if(m1!=1 && n1!=1)
    {
      cerro("input argument must be a vector");
      return(0);
    }
  }
 
  fun1(stk(l1), n1*m1, &nX, &X, &S);
 
  /* 4 - Create the place for the firts output X ( a vector of doubles, size: m*n ) to the adress l2 in the Scilab stack */
  CreateVar(2, "d", &m, &nX, &l2);
 
  /* if there are two outputs variable then: Create the place for the second output S ( a double, size 1*1) to the adress l3 in the Scilab stack */
  /* get the value of S, and put it in the Scilab stack */
  if(Lhs==2)
  {
  CreateVar(3, "d", &m, &m, &l3);
  *stk(l3)=S;
 }
 
  /* get the components of  X, and put them in the Scilab stack */
  for(i=0;i<nX;i++)
  stk(l2)[i]=X[i];
 
  /* free memory */
  free(X);
 
  /* 5 - Specification of outputs variables */
  LhsVar(1) = 2;
 
  if(Lhs==2)
  LhsVar(2) = 3;
 
  return 0;
}