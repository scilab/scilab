#include  "stack-c.h"

int intex18c(char* fname)
     /* [p,q,r,s]=ex18c(A)  returns p (#2) = a reference to param (must exists),
      *  q (#3) = a 2x3 matrix X with X(i,j)=ij, variable number 3
      *  r (#1) = A,
      *  s (#4) a reference to X  */
{ 
  int l1,m1,n1;  int l3,m3,n3;

  CheckRhs(1,1);    CheckLhs(1,4);

  GetRhsVar(1,"d", &m1, &n1, &l1);
  CreateRefFromName(2,"param");
  CreateVar(3, "d",(m3=2, &m3), (n3=3, &n3), &l3);
  stk(l3)[0]=11;stk(l3)[1]=21;stk(l3)[2]=12;
  stk(l3)[3]=22;stk(l3)[4]=13;stk(l3)[5]=23;
  CreateRef(4,3);

  LhsVar(1)=2;  LhsVar(2)=3;  LhsVar(3)=1;  LhsVar(4)=4;
  return 0;
}
