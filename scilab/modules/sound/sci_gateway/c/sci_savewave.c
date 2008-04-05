/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA - Scilab 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_sound.h"
#include "machine.h"
#include "stack-c.h"
#include "sox.h"
#include "cluni0.h"
/*--------------------------------------------------------------------------*/
static char filename[PATH_MAX];
static int out_n;
static long int lout;
/*--------------------------------------------------------------------------*/
/* SCILAB function : savewave */
/*--------------------------------------------------------------------------*/
int sci_savewave(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,mn2,l2,m3,n3,l3,l4,err,rate=22050,channels;
  int un=1;
  Nbvars=0;
  CheckRhs(2,3);
  CheckLhs(1,1);
  /*  checking variable file */
  GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
  /*  checking variable res */
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
  mn2=m2*n2;
  /* Checking variable rate */
  if ( Rhs == 3 )
  {
     GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
     rate =(int)(*stk(l3));
  }
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &un,&un, &l4);
  lout=PATH_MAX;
  C2F(cluni0)(cstk(l1), filename, &out_n,m1*n1,lout);
  channels = m2;
  C2F(savewave)(filename,stk(l2),&rate,&mn2,&channels,&err);
  if (err >  0)
	{
    /*sciprint("%s: Internal Error \n",fname);*/
    Error(10000);
    return 0;
  };
  *stk(l4) = *stk(l2);
  LhsVar(1)= Rhs+1;
  PutLhsVar();
  return 0;
}
/*--------------------------------------------------------------------------*/
