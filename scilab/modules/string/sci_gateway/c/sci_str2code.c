/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/* desc : This function return scilab int codes associated with a
          character string                                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "code2str.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "warningmode.h"
#include "sciprint.h"
/*----------------------------------------------------------------------------*/
int sci_str2code(char *fname, unsigned long fname_len)
{
    CheckRhs(1, 1);
    CheckLhs(1, 1);

    if (strcmp(fname, "str2code") == 0)
    {
        if (getWarningMode())
        {
            sciprint(_("%s: Feature %s is obsolete.\n"), _("Warning"), fname);
            sciprint(_("%s: Please use %s instead.\n"), _("Warning"), "ascii");
            sciprint(_("%s: This feature will be permanently removed in Scilab %s\n\n"), _("Warning"), "6.0.0");
        }
    }

    if (VarType(1) == sci_strings)
    {
        char **Input_String = NULL;
        int m1 = 0, n1 = 0;
        int *Output_Matrix = NULL;
        int nbOutput_Matrix = 0;

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Input_String);

        if ( ( strcmp(Input_String[0], "") == 0 ) || (Input_String[0] == NULL) )
        {
            /* str2code('') returns [] */
            int l = 0;

            freeArrayOfString(Input_String, m1 * n1);

            m1 = 0;
            n1 = 0;
            CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l);
            LhsVar(1) = Rhs + 1 ;
            PutLhsVar();

            return 0;
        }
        else
        {
            if (strlen(Input_String[0]))
            {
                nbOutput_Matrix = (int)strlen(Input_String[0]);
            }
            Output_Matrix = (int*)MALLOC( sizeof(int) * nbOutput_Matrix );
            if (Output_Matrix)
            {
                int i = 0;
                int numRow   = 1 ;
                int outIndex = 0;

                str2code(Output_Matrix, Input_String);
                freeArrayOfString(Input_String, m1 * n1);

                /* put on scilab stack */
                CreateVar(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &nbOutput_Matrix, &numRow, &outIndex); /*Output*/
                for ( i = 0 ; i < nbOutput_Matrix ; i++ )
                {
                    stk(outIndex)[i] = (double)Output_Matrix[i] ;
                }

                /* free pointers */
                FREE(Output_Matrix);
                Output_Matrix = NULL;

                LhsVar(1) = Rhs + 1 ;
                PutLhsVar();
            }
            else
            {
                freeArrayOfString(Input_String, m1 * n1);
                Scierror(999, _("%s: No more memory.\n"), fname);
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), fname, 1);
    }
    return 0;
}
/*----------------------------------------------------------------------------*/
