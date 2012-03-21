/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "predef.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_predef)(char *fname,unsigned long fname_len)
{
    int previous_n_var_protected = 0;

    Rhs = Max(0, Rhs);

    CheckRhs(0,1);
    CheckLhs(0,1);

    previous_n_var_protected = getNumberPredefVariablesProtected();

    if (Rhs == 0)
    {
        int one = 1 ,l = 0;

        CreateVar(Rhs+1, MATRIX_OF_INTEGER_DATATYPE, &one, &one,&l);
        *istk(l) = (int) previous_n_var_protected;

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else /* Rhs == 1 */
    {
        int *out_values = NULL;
        int nout = 0 , mout = 0;
        int new_n_var_protected = 0;

        if ( VarType(1) == sci_matrix )
        {
            int m1 = 0, n1 = 0, l1 = 0;
            GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
            if ( (m1 == n1) && (n1 == 1) )
            {
                double dn_var = *stk(l1);
                int n_var = (int) dn_var;

                if (dn_var != (double)n_var)
                {
                    Scierror(999,_("%s: Wrong value for input argument #%d: A int expected.\n"),fname,1);
                    return 0;
                }

                setNumberPredefVariablesProtected(n_var);
            }
            else
            {
                Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
                return 0;
            }
        }
        else if ( VarType(1) == sci_strings )
        {	
            int m1 = 0, n1 = 0, l1 = 0;
            char *protectMode = NULL;

            GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
            protectMode = cstk(l1);
            if (protectMode)
            {
                if ( ((strlen(protectMode) == 1 ) && (protectMode[0] == 'c')) ||
                    (strcmp(protectMode,"clear") == 0) )
                {
                    clearPredef();
                }
                else if ( ((strlen(protectMode) == 1 ) && (protectMode[0] == 'a')) ||
                    (strcmp(protectMode,"all") == 0) )
                {
                    predefAll();
                }
                else if (strcmp(protectMode,"names") == 0)
                {
                    int nbElements = 0;
                    char **variablesPredef = getPredefinedVariablesName(&nbElements);
                    if (variablesPredef && (nbElements > 0))
                    {
                        SciErr sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, nbElements, 1, variablesPredef);
                        freeArrayOfString(variablesPredef, nbElements);
                        variablesPredef = NULL;
                        if(sciErr.iErr)
                        {
                            printError(&sciErr, 0);
                            Scierror(999,_("%s: Memory allocation error.\n"), fname);
                        }
                        else
                        {
                            LhsVar(1) = Rhs + 1;
                            PutLhsVar();
                        }
                        return 0;
                    }
                    else
                    {
                        createEmptyMatrix(pvApiCtx, Rhs + 1);
                        LhsVar(1) = Rhs + 1;
                        PutLhsVar();
                        return 0;
                    }
                }
                else
                {
                    Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,1,"clear","all");
                    return 0;
                }
            }
        }
        else
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: A scalar or a string expected.\n"),fname,1);
            return 0;
        }

        new_n_var_protected = getNumberPredefVariablesProtected();

        out_values = (int*)MALLOC(sizeof(int)*2);
        out_values[0] = previous_n_var_protected;
        out_values[1] = new_n_var_protected;

        nout = 1 ; mout = 2;
        CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &nout, &mout, &out_values);
        if (out_values) {FREE(out_values); out_values = NULL;}

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
