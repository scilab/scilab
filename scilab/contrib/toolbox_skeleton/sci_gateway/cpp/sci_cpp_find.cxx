/* ==================================================================== */
/* Allan CORNET */
/* DIGITEO 2009 */
/* Template toolbox_skeleton */
/* This file is released into the public domain */
/* ==================================================================== */
#include <string>
/* ==================================================================== */
extern "C" 
{
/* ==================================================================== */	
  #include "stack-c.h"
  #include "api_scilab.h"
  #include "Scierror.h"
  #include "MALLOC.h"
/* ==================================================================== */
  int sci_cpp_find(char *fname) 
  {
    StrErr strErr;
    
    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    int lenStVarOne = 0;
    int iType1 = 0;
    
    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    char *pStVarTwo = NULL;
    int lenStVarTwo = 0;
    int iType2 = 0;
    
    /* Check the number of input argument */
    CheckRhs(2,2); 
    
    /* Check the number of output argument */
    CheckLhs(1,1);
    
    /* get Address of inputs */
    strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    }     
    strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    /* checks types */
    strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    if ( iType1 != sci_strings )
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A string expected.\n",fname,1);
      return 0;
    }
  
    strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    if ( iType2 != sci_strings )
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A string expected.\n",fname,2);
      return 0;
    }
    
    /* get strings */
    
    strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    /* check size */
    if ( (m1 != n1) && (n1 != 1) ) 
    {
      Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n",fname,1);
      return 0;
    }
    /* alloc string */
    pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
    /* get string One */
    strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    if ( (m2 != n2) && (n2 != 1) ) 
    {
      Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n",fname,2);
      return 0;
    }
    /* alloc string */
    pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
    /* get string Two */
    strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
    if(strErr.iErr)
    {
      printError(&strErr, 0);
      return 0;
    } 
    
    
    std::string myMessage (pStVarOne);
    if (pStVarOne) {FREE(pStVarOne); pStVarOne = NULL;}
    	
    std::string search(pStVarTwo);
    if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}

    /* Where we will store the position */
    double dOut = 0.0;
    if (myMessage.find(search) != std::string::npos) 
    {
      /* The actual operation */
      dOut = myMessage.find(search); 
    } 
    else 
    {
      dOut = -1; /* Substring not found */
    }
    
    /* create result on stack */
    int m_out = 1, n_out = 1;
    
    createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, &dOut);
    
    LhsVar(1) = Rhs + 1; 

    return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */