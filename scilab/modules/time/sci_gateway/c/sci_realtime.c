/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/ 
#include "sci_realtime.h"
/*--------------------------------------------------------------------------*/ 
extern int C2F(realtime)(double *t);
extern int C2F(realtimeinit)(double *t,double *scale); 
/*--------------------------------------------------------------------------*/ 
int C2F(sci_realtimeinit)(char *fname,unsigned long fname_len)
{
 int m1,n1,l1;
 double zer=0.0;
 CheckRhs(1,1);
 CheckLhs(1,1);
 /*  checking variable scale */
 GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
 CheckScalar(1,m1,n1);
 /* cross variable size checking */
 C2F(realtimeinit)(&zer,stk(l1));
 LhsVar(1)=0;
 return 0;
}
/*--------------------------------------------------------------------------*/  
int C2F(sci_realtime)(char *fname,unsigned long fname_len)
{
 int m1,n1,l1;
 CheckRhs(1,1);
 CheckLhs(1,1);
 /*  checking variable t */
 GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
 CheckScalar(1,m1,n1);
 /* cross variable size checking */
 C2F(realtime)(stk(l1));
 LhsVar(1)=0;
 return 0;
}               
/*--------------------------------------------------------------------------*/ 
