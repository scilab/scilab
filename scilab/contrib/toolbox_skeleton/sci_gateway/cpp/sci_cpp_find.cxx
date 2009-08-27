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
  #include "api_common.h"
  #include "api_string.h"
  #include "api_double.h"
  #include "Scierror.h"
  #include "MALLOC.h"
/* ==================================================================== */
  int sci_cpp_find(char *fname) 
  {
    int m1 = 0, n1 = 0;
    int *piAddressVarOne = NULL;
    char *pStVarOne = NULL;
    int lenStVarOne = 0;
    
    int m2 = 0, n2 = 0;
    int *piAddressVarTwo = NULL;
    char *pStVarTwo = NULL;
    int lenStVarTwo = 0;
    
    /* Check the number of input argument */
    CheckRhs(2,2); 
    
    /* Check the number of output argument */
    CheckLhs(1,1);
    
    /* get Address of inputs */
    getVarAddressFromPosition(1, &piAddressVarOne);
    getVarAddressFromPosition(2, &piAddressVarTwo);
    
    if ( getVarType(piAddressVarOne) != sci_strings )
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A string expected.\n",fname,1);
      return 0;
    }
  
    if ( getVarType(piAddressVarTwo) != sci_strings )
    {
      Scierror(999,"%s: Wrong type for input argument #%d: A string expected.\n",fname,2);
      return 0;
    }
    
    /* get strings */
    
    getMatrixOfString(piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
    /* check size */
    if ( (m1 != n1) && (n1 != 1) ) 
    {
      Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n",fname,1);
      return 0;
    }
    /* alloc string */
    pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
    /* get string One */
    getMatrixOfString(piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
    
    getMatrixOfString(piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
    if ( (m2 != n2) && (n2 != 1) ) 
    {
      Scierror(999,"%s: Wrong size for input argument #%d: A string expected.\n",fname,2);
      return 0;
    }
    /* alloc string */
    pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
    /* get string Two */
    getMatrixOfString(piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
    
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
    
    createMatrixOfDouble(Rhs + 1, m_out, n_out, &dOut);
    LhsVar(1) = Rhs + 1; 

    return 0;
	}
/* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */