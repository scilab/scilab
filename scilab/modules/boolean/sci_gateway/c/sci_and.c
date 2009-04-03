/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "stack-c.h"
#include "Scierror.h"
#include "vect_and.h"
/*--------------------------------------------------------------------------*/
int C2F(intand)(char *fname,unsigned long l);
/*--------------------------------------------------------------------------*/
/* SCILAB function : and */
/*--------------------------------------------------------------------------*/
int C2F(intand)(char *fname,unsigned long l)
{
 int m1,n1,l1,mm2,l2,lo,nopt=0,lw;
 int opt=0,errn=44;

 CheckRhs(1,2);
 CheckLhs(1,1);
 if (VarType(1)!=sci_boolean) {
   lw = 1 + Top - Rhs;
   C2F(overload)(&lw,"and",3L);
   return 0;}

 if (Rhs==2) {
   nopt=1;
   if (VarType(2)==sci_strings) {
     GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&lo);
     if (*cstk(lo)=='r')
       opt=1;
     else if (*cstk(lo)=='*')
       opt=0;
     else if (*cstk(lo)=='c')
       opt=2;
     else {
       Err=2;
       Error(errn);
     }
 
   }
   else {
     GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&lo);
     opt= (int) *stk(lo);
     if (opt>2||opt<1) {
       Err=2;
       Error(errn);
     }
   }
 }  /*(Rhs==2) */

 /*  checking variable a */
 GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);

 if (Rhs==2 && m1*n1==0) {
   LhsVar(1)= 1;
   C2F(putlhsvar)();
   return 0;
 }

 /* cross variable size checking */
 mm2=1;
 l2 = 0;
 if (opt==0) 
 {
   CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&mm2,&mm2,&l2);
 }/* named: x */
 else if (opt==1) 
 {
   CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&mm2,&n1,&l2);
 }/* named: x */
 else if (opt==2) 
 {
   CreateVar(2+nopt,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&mm2,&l2);
 }/* named: x */
 vect_and(istk(l1),m1,n1,istk(l2),opt);
 LhsVar(1)= 2+nopt;
 C2F(putlhsvar)();
 return 0;
}
/*--------------------------------------------------------------------------*/
