#include <string.h>
#include "../stack-c.h"
 
int F2C(intsrealtimeinit)(fname)
   char *fname;
{
 int m1,n1,l1;
 double zer=0.0;
 CheckRhs(1,1);
 CheckLhs(1,1);
 /*  checking variable scale */
 GetRhsVar(1,"d",&m1,&n1,&l1);
 CheckScalar(1,m1,n1);
 /* cross variable size checking */
 C2F(realtimeinit)(&zer,stk(l1));
 LhsVar(1)=0;
 return 0;
}
 
int F2C(intsrealtime)(fname)
   char *fname;
{
 int m1,n1,l1;
 CheckRhs(1,1);
 CheckLhs(1,1);
 /*  checking variable t */
 GetRhsVar(1,"d",&m1,&n1,&l1);
 CheckScalar(1,m1,n1);
 /* cross variable size checking */
 C2F(realtime)(stk(l1));
 LhsVar(1)=0;
 return 0;
}               
