/*------------------------------------------------------------------------
 *    Copyright INRIA
 --------------------------------------------------------------------------*/

#include "../stack-c.h"
#include <string.h>

void vect_or __PARAMS((int *v,int m,int n,int *r,int opt));
void vect_and __PARAMS((int *v,int m,int n,int *r,int opt));
/******************************************
 * SCILAB function : or
 ******************************************/

int C2F(intor)(fname,l)
   char *fname;
   long l;
{
 int m1,n1,l1,mm2,l2,lo,nopt=0,lw;
 int opt=0,errn=44;
 CheckRhs(1,2);
 CheckLhs(1,1);
 if (VarType(1)!=4) {
   lw = 1 + Top - Rhs;
   C2F(overload)(&lw,"or",2L);
   return 0;}

 if (Rhs==2) {
   nopt=1;
   if (VarType(2)==10) {
     GetRhsVar(2,"c",&m1,&n1,&lo);
     if (*cstk(lo)=='r')
       opt=1;
     else if (*cstk(lo)=='*')
       opt=0;
     else if (*cstk(lo)=='c')
       opt=2;
     else {
       Err=2;
       C2F(error)(&errn);
     }
 
   }
   else {
     GetRhsVar(2,"d",&m1,&n1,&lo);
     opt=*stk(lo);
     if (opt>2||opt<1) {
       Err=2;
       C2F(error)(&errn);
     }
   }
 }  /*(Rhs==2) */

 /*  checking variable a */
 GetRhsVar(1,"b",&m1,&n1,&l1);

 if (Rhs==2 && m1*n1==0) {
   LhsVar(1)= 1;
   C2F(putlhsvar)();
   return 0;
 }

 /* cross variable size checking */
 mm2=1;
 if (opt==0) {
   CreateVar(2+nopt,"b",&mm2,&mm2,&l2);}/* named: x */
 else if (opt==1) {
   CreateVar(2+nopt,"b",&mm2,&n1,&l2);}/* named: x */
 else if (opt==2) {
   CreateVar(2+nopt,"b",&m1,&mm2,&l2);}/* named: x */
 vect_or(istk(l1),m1,n1,istk(l2),opt);
 LhsVar(1)= 2+nopt;
 C2F(putlhsvar)();
 return 0;
}


/******************************************
 * SCILAB function : and
 ******************************************/

int C2F(intand)(fname,l)
   char *fname;
   long l;
{
 int m1,n1,l1,mm2,l2,lo,nopt=0,lw;
 int opt=0,errn=44;

 CheckRhs(1,2);
 CheckLhs(1,1);
 if (VarType(1)!=4) {
   lw = 1 + Top - Rhs;
   C2F(overload)(&lw,"and",3L);
   return 0;}

 if (Rhs==2) {
   nopt=1;
   if (VarType(2)==10) {
     GetRhsVar(2,"c",&m1,&n1,&lo);
     if (*cstk(lo)=='r')
       opt=1;
     else if (*cstk(lo)=='*')
       opt=0;
     else if (*cstk(lo)=='c')
       opt=2;
     else {
       Err=2;
       C2F(error)(&errn);
     }
 
   }
   else {
     GetRhsVar(2,"d",&m1,&n1,&lo);
     opt=*stk(lo);
     if (opt>2||opt<1) {
       Err=2;
       C2F(error)(&errn);
     }
   }
 }  /*(Rhs==2) */

 /*  checking variable a */
 GetRhsVar(1,"b",&m1,&n1,&l1);

 if (Rhs==2 && m1*n1==0) {
   LhsVar(1)= 1;
   C2F(putlhsvar)();
   return 0;
 }

 /* cross variable size checking */
 mm2=1;
 if (opt==0) {
   CreateVar(2+nopt,"b",&mm2,&mm2,&l2);}/* named: x */
 else if (opt==1) {
   CreateVar(2+nopt,"b",&mm2,&n1,&l2);}/* named: x */
 else if (opt==2) {
   CreateVar(2+nopt,"b",&m1,&mm2,&l2);}/* named: x */
 vect_and(istk(l1),m1,n1,istk(l2),opt);
 LhsVar(1)= 2+nopt;
 C2F(putlhsvar)();
 return 0;
}


void vect_or(int *v,int m,int n,int *r,int opt)
{
  int k,l,i;
  
  if (opt==0) {
    r[0]=0;
    for (k=0;k<m*n;k++)
      if (v[k]) {
	r[0]=1;
	break;
      }
  }
  else if (opt==1) {

    for (k=0;k<n;k++) {
      r[k]=0;i=k*m;
      for (l=0;l<m;l++)
	if (v[i++]) {
	  r[k]=1;
	  break;
	}
    }
  }
  else if (opt==2) {
    for (l=0;l<m;l++) {
      r[l]=0;i=l;
      for (k=0;k<n;k++) {
	if (v[i]) {
	  r[l]=1;
	  break;
	}
	i+=m;
      }
    }
  }
}

void vect_and(int *v,int m, int n,int *r,int opt)
{
  int k,l,i;
  
  if (opt==0) {
    r[0]=1;
    for (k=0;k<m*n;k++)
      if (!v[k]) {
	r[0]=0;
	break;
      }
  }
  else if (opt==1) {

    for (k=0;k<n;k++) {
      r[k]=1;i=k*m;
      for (l=0;l<m;l++)
	if (!v[i++]) {
	  r[k]=0;
	  break;
	}
    }
  }
  else if (opt==2) {
    for (l=0;l<m;l++) {
      r[l]=1;i=l;
      for (k=0;k<n;k++) {
	if (!v[i]) {
	  r[l]=0;
	  break;
	}
	i+=m;
      }
    }
  }
}
