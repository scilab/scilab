#include "stack-c.h"
/******************************************
 * SCILAB function : calc, fin = 1
 ******************************************/

int intscalc(fname)
   char *fname;
{
 int m1,n1,l1,un=1,l2;
 CheckRhs(1,1);
 CheckLhs(1,1);
 /*  checking variable str */
 GetRhsVar(1,"c",&m1,&n1,&l1);
 /* cross variable size checking */
 CreateVar(2,"i",&un,&un,&l2);/* named: a */
 C2F(fcalc)(cstk(l1),istk(l2),&m1);
 LhsVar(1)= 2;
 return 0;
}
/******************************************
 * SCILAB function : som, fin = 2
 ******************************************/

int intssom(fname)
   char *fname;
{
 int m1,n1,l1,mn1,m2,n2,l2,mn2,un=1,mn3,l3;
 CheckRhs(2,2);
 CheckLhs(1,1);
 /*  checking variable a */
 GetRhsVar(1,"d",&m1,&n1,&l1);
 CheckRow(1,m1,n1);
 mn1=m1*n1;
 /*  checking variable b */
 GetRhsVar(2,"d",&m2,&n2,&l2);
 CheckRow(2,m2,n2);
 mn2=m2*n2;
 /* cross variable size checking */
 CheckDimProp(1,2,n1 != n2);
 CreateVar(3,"d",(mn3=n1,&mn3),(un=1,&un),&l3);/* named: c */
 C2F(csom)(&n1,stk(l1),stk(l2),stk(l3));
 LhsVar(1)= 3;
 return 0;
}
