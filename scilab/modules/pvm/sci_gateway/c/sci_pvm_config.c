/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_pvm.h"
#include "MALLOC.h"
#include "sci_pvm.h"
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_config, fin = 9
 ******************************************/
int intspvm_config _PARAMS((char *fname,unsigned long fname_len))
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
      Scierror(999,_("%s: Error\n"),fname);
      return 0;
    }
  Createlist(1,7);
  CreateListVarFromPtr(1,1,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&nhost,&work));
  CreateListVarFromPtr(1,2,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&narch,&work));
  CreateListVarFromPtr(1,3,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pdtid);
  FREE(pdtid);

  CreateListVarFromPtr(1,4,MATRIX_OF_STRING_DATATYPE,&ne3,&un,pname);
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

  CreateListVarFromPtr(1,5,MATRIX_OF_STRING_DATATYPE,&ne3,&un,parch);
  if (parch)
  {
	  int i=0;
	  for(i=0;i<ne3;i++)
	  {
		  if (parch[i])
		  {
			  FREE(parch[i]);
			  parch[i]=NULL;
		  }
	  }
	  FREE(parch);
	  parch=NULL;
  }

  CreateListVarFromPtr(1,6,MATRIX_OF_INTEGER_DATATYPE,&un,&ne3,&pspeed);
  FREE(pspeed);
  CreateListVarFromPtr(1,7,MATRIX_OF_INTEGER_DATATYPE,&un,&un,(work=&info,&work));
  LhsVar(1)= 1;
  
  pvm_error_check(fname,info,fname_len);

  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
