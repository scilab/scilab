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
#include "sci_pvm.h"
#include "gw_pvm.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_config, fin = 9
 ******************************************/
int sci_pvm_config(char *fname,unsigned long fname_len)
{
  int un=1;
  int nhost,narch,*pdtid,*pspeed,ne3,info,*work;
  char  **pname,**parch;
  CheckRhs(0,0);
  CheckLhs(1,1);
  C2F(scipvmconfig)(&nhost,&narch,&pdtid,&pname,&parch,&pspeed,&ne3,&info);
  /* Creation of output list of length 7*/
  if (info != 0 ) 
    {
      Scierror(999,_("%s: An error occurred: %s\n"),fname,scipvm_error_msg(info));
      return 0;
    }
  Createlist(1,7);
  CreateListVarFromPtr(1,1,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&nhost,&work));
  CreateListVarFromPtr(1,2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&narch,&work));
  CreateListVarFromPtr(1,3,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pdtid);
  FREE(pdtid);

  CreateListVarFromPtr(1,4,MATRIX_OF_STRING_DATATYPE,&ne3,&un,pname);
  freeArrayOfString(pname, ne3);

  CreateListVarFromPtr(1,5,MATRIX_OF_STRING_DATATYPE,&ne3,&un,parch);
  freeArrayOfString(parch, ne3);

  CreateListVarFromPtr(1,6,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pspeed);
  FREE(pspeed);
  CreateListVarFromPtr(1,7,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&info,&work));
  LhsVar(1)= 1;
  
  pvm_error_check(fname,info,fname_len);

  C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/ 
