#include "stack-c.h"

extern int sumab(int n, double * a, double * b, double * y); 

void intsumab(char *fname)
{
  int l1, m1, n1, l2, m2, n2, l3, n;
  
  /* 1 - Check rhs and lhs number  */
  int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  /* 2 - Check rhs type and get rhs adress in the Scilab stack */ 
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  
  n=m2*n2;
  if( n1!=n2 || m1!=m2) 
    {
      cerro("input arguments must have the same size");
      return 0;
    }	
  
  if(n1!=0 && m1!=0)
    if(n1!=1 && m1!=1)
    {
      cerro("input arguments must be vectors");
      return(0);
    }

  /* use chekdims and getscalar functions: a completer ................*/ 

  /* 4 - Create the output argument */
  CreateVar(3,"d",&m2,&n2,&l3); 
  sumab(n,stk(l1),stk(l2),stk(l3));
  
  /* 5 - Specif the ouput argument */
  LhsVar(1) = 3;
  return 0;
}