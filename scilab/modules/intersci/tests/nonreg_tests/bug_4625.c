#include "stack-c.h"
/******************************************
 * SCILAB function : ext1c, fin = 1
 ******************************************/

int intsext1c(char *fname)
{
 int m1,n1,l1,mn1,m2,n2,l2,mn2,un=1,mn3,l3;
 CheckRhs(2,2);
 CheckLhs(1,1);
 /*  checking variable a */
 GetRhsVar(1,"d",&m1,&n1,&l1);
 CheckVector(1,m1,n1);
 mn1=m1*n1;
 /*  checking variable b */
 GetRhsVar(2,"d",&m2,&n2,&l2);
 CheckVector(2,m2,n2);
 mn2=m2*n2;
 /* cross variable size checking */
 CheckDimProp(1,2,m1*n1 != m2*n2);
 CreateVar(3,"d",(un=1,&un),(mn3=mn1,&mn3),&l3);/* named: c */
 C2F(ext1c)(&mn1,stk(l1),stk(l2),stk(l3));
 LhsVar(1)= 3;
 return 0;
}
