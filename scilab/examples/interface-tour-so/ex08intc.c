#include <string.h> 
#include <stdio.h>
#include "stack-c.h"

/**************************************************
 * examples of an hand written interface 
 *   utilities for testing dimensions constraints 
 *   test of type of arguments (VarType) 
 **************************************************/

int intex8c_1(fname)
     char *fname;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5;

  CheckRhs(5,5);
  CheckLhs(0,1) ;

  GetRhsVar(1, "d", &m1, &n1, &l1);
  /* Check that first argument is a vector */
  CheckVector(1,m1,n1);                  
  /* Check vector length */
  CheckLength(1,m1*n1,4);
  
  GetRhsVar(2, "d", &m2, &n2, &l2);
  /* Checks that second argument is a row vector */
  CheckRow(2,m2,n2);
  /* CheckColumn can also be used */

  /* Check compatibility beetween arg 1 and arg 2 
   * we want m1*n1 == n2 
   */
  CheckDimProp(1,2, m1 * n1 != n2); 

  GetRhsVar(3, "d", &m3, &n3, &l3);
  /* Checks that arg 1 and arg3 have same dimensions */
  CheckSameDims(1,3,m1,n1,m3,n3);

  GetRhsVar(4,"d",&m4,&n4,&l4);
  /* arg 4 must be scalar */
  CheckScalar(4,m4,n4);

  GetRhsVar(5,"d",&m5,&n5,&l5);
  /* square matrix */
  CheckSquare(5,m5,n5);
  /* check dimensions */
  CheckDims(5,m5,m5,5,5);
  LhsVar(1)=0;
  return 0;
}

/* use help type in scilab to get a list of types */

int intex8c_2(fname)
     char *fname;
{
  integer m1,n1,l1;

  CheckRhs(1,1);
  CheckLhs(0,1) ;

  switch (  VarType(1) ) 
    {
    case 1 : 
      GetRhsVar(1, "d", &m1, &n1, &l1);
      sciprint("1 is a scalar matrix\r\n");
      break;
    case 10:
      GetRhsVar(1, "c", &m1, &n1, &l1);
      sciprint("1 is a string\r\n");
      break;
    case 5 :
      sciprint("1 is a sparse trying to overload\r\n");
      OverLoad(1);
    }
  LhsVar(1)=0;
  return 0;
}






