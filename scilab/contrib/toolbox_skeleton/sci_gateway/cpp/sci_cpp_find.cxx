/* ==================================================================== */
/* Template toolbox_skeleton */
/* This file is released under the 3-clause BSD license. See COPYING-BSD. */
/* ==================================================================== */
#include <string>
/* ==================================================================== */
extern "C" 
{
    /* ==================================================================== */	
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include <localization.h>

    /* ==================================================================== */
    int sci_cpp_find(char *fname, void* pvApiCtx) 
    {
        SciErr sciErr;

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
        CheckInputArgument(pvApiCtx, 2,2); 

        /* Check the number of output argument */
        CheckOutputArgument(pvApiCtx, 1,1);

        /* get Address of inputs */
        sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }     
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        /* checks types */
        sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        if ( iType1 != sci_strings )
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: string expected.\n"),fname,1);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        if ( iType2 != sci_strings )
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: string expected.\n"),fname,2);
            return 0;
        }

        /* get strings */

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        /* check size */
        if ( (m1 != n1) && (n1 != 1) ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: string expected.\n"),fname,1);
            return 0;
        }
        /* alloc string */
        pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
        /* get string One */
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 

        if ( (m2 != n2) && (n2 != 1) ) 
        {
            Scierror(999,_("%s: Wrong size for input argument #%d: string expected.\n"),fname,2);
            return 0;
        }
        /* alloc string */
        pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
        /* get string Two */
        sciErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        } 


        std::string myMessage (pStVarOne);
        if (pStVarOne)
        {
            FREE(pStVarOne); 
            pStVarOne = NULL;
        }

        std::string search(pStVarTwo);
        if (pStVarTwo)
        {
            FREE(pStVarTwo);
            pStVarTwo = NULL;
        }

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
        int m_out = 1;
        int n_out = 1;

        createMatrixOfDouble(pvApiCtx, nbInputArgument(pvApiCtx) + 1, m_out, n_out, &dOut);

        AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1; 

        return 0;
    }
    /* ==================================================================== */	
} /* extern "C" */
/* ==================================================================== */
