/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * Copyright (C) 2002 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* PVM functions interfaces */
/*--------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "MALLOC.h"
#include "sci_pvm.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_tasks, fin = 8
 ******************************************/
int intspvm_tasks (char *fname,unsigned long fname_len)
{
  int un=1,m1,n1,l1,where=0; 
  char  **pname;
  int ntask, *ptid,*pptid,*pdtid,*pflag,info,ne3,*work; 

  CheckRhs(0,1); 
  CheckLhs(1,1);
  if ( Rhs == 1) { 
    GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1); 
    CheckScalar(1,m1,n1);
    where = *istk(l1);
  }
  
  C2F(scipvmtasks)(&where,&ntask,&ptid,&pptid,&pdtid,&pflag,&pname,&ne3,&info); 
  if (info != 0 ) 
    {
      Scierror(999,_("%s: An error occurred: %s\n"),fname,scipvm_error_msg(info));
      return 0;
    }
  /* Creation of output list of length 7*/
  Createlist(Rhs+1,7);
  CreateListVarFromPtr(Rhs+1,1,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&ptid);
  FREE(ptid);
  CreateListVarFromPtr(Rhs+1,2,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pptid);
  FREE(pptid);
  CreateListVarFromPtr(Rhs+1,3,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pdtid);
  FREE(pdtid);
  CreateListVarFromPtr(Rhs+1,4,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pflag);
  FREE(pflag);
  CreateListVarFromPtr(Rhs+1,5,MATRIX_OF_STRING_DATATYPE,&ne3,&un,pname);
  if (pname)
  {
	  int i=0;
	  for(i=0;i<ne3;i++)
	  {
		  if (pname[i])
		  {
			  FREE(pname[i]);
			  pname[i]=NULL;
		  }
	  }
	  FREE(pname);
	  pname=NULL;
  }

  CreateListVarFromPtr(Rhs+1,6,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&ntask,&work));
  CreateListVarFromPtr(Rhs+1,7,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&info,&work));
  LhsVar(1)= Rhs+1;
  pvm_error_check(fname,info,fname_len);

  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/ 
