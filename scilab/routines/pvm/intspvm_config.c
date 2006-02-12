/*-----------------------------------------------------------------------------------*/
/* PVM functions interfaces */
/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */
/* Jean-Philippe Chancelier July 2002  */
/* Modified by Allan Cornet  INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "intspvm_config.h"
/*-----------------------------------------------------------------------------------*/ 
extern void pvm_error_check(char *fname,int err);
/*-----------------------------------------------------------------------------------*/ 
/******************************************
 * SCILAB function : pvm_config, fin = 9
 ******************************************/
int intspvm_config(char *fname)
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
      Scierror(999,"%s: Error \r\n",fname);
      return 0;
    }
  Createlist(1,7);
  CreateListVarFromPtr(1,1,"i",&un,&un,(work=&nhost,&work));
  CreateListVarFromPtr(1,2,"i",&un,&un,(work=&narch,&work));
  CreateListVarFromPtr(1,3,"i",&un,&ne3,&pdtid);
  FREE(pdtid);
  CreateListVarFromPtr(1,4,"S",&ne3,&un,pname);
  FreeRhsSVar(pname); 
  CreateListVarFromPtr(1,5,"S",&ne3,&un,parch);
  FreeRhsSVar(parch); 
  CreateListVarFromPtr(1,6,"i",&un,&ne3,&pspeed);
  FREE(pspeed);
  CreateListVarFromPtr(1,7,"i",&un,&un,(work=&info,&work));
  LhsVar(1)= 1;
  pvm_error_check(fname,info);
  return 0;
}
/*-----------------------------------------------------------------------------------*/ 
