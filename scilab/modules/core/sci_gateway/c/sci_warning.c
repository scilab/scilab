/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include "gw_core.h"
#include "stack-c.h"
#include "sciprint.h"
#include "Scierror.h"
#include "warningmode.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_warning)(char *fname, unsigned long fname_len)
{
    static int n1 = 0, m1 = 0;

    CheckRhs(1, 1);
    CheckLhs(1, 1);

    if ( GetType(1) == sci_strings )
    {
        char **Input_Strings = NULL;

        GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Input_Strings);

        if ( (m1 * n1) == 1 )
        {
            if ( (strcmp(Input_Strings[0], "off") == 0) || (strcmp(Input_Strings[0], "on") == 0) )
            {
                if (strcmp(Input_Strings[0], "off") == 0)
                {
                    setWarningMode(FALSE);
                }
                else  /* on */
                {
                    setWarningMode(TRUE);
                }
                freeArrayOfString(Input_Strings, 1);
            }
            else
            {
                if (strcmp(Input_Strings[0], "query") == 0)
                {
                    char *Output = NULL;

                    if (getWarningMode())
                    {
                        Output = strdup("on");
                    }
                    else
                    {
                        Output = strdup("off");
                    }

                    n1 = 1;
                    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, (m1 = (int)strlen(Output), &m1), &n1, &Output);
                    LhsVar(1) = Rhs + 1;

                    if (Output)
                    {
                        FREE(Output);
                        Output = NULL;
                    }
                    freeArrayOfString(Input_Strings, n1);

                    PutLhsVar();
                    return 0;
                }
                else if ( getWarningMode() )
                {
                    if (strlen(Input_Strings[0]) > 0)
                    {
                        if (Input_Strings[0][strlen(Input_Strings[0]) - 1] == '\n')
                        {
                            sciprint(_("WARNING: %s"), Input_Strings[0]);
                        }
                        else
                        {
                            sciprint(_("WARNING: %s\n"), Input_Strings[0]);
                        }
                    }
                    else
                    {
                        sciprint(_("WARNING: %s\n"), "");
                    }
                    freeArrayOfString(Input_Strings, n1);
                }
            }
        }
        else
        {
            int i = 0;
            if ( getWarningMode() && m1 * n1 > 1)
            {
                int len = strlen(_("WARNING: %s\n")) - strlen("%s\n");
                char * whites = (char *)MALLOC(sizeof(char) * (len + 1));

                if (strlen(Input_Strings[0]) > 0)
                {
                    if (Input_Strings[i][strlen(Input_Strings[0]) - 1] == '\n')
                    {
                        sciprint(_("WARNING: %s"), Input_Strings[0]);
                    }
                    else
                    {
                        sciprint(_("WARNING: %s\n"), Input_Strings[0]);
                    }
                }
                else
                {
                    sciprint(_("WARNING: %s\n"), "");
                }

                memset(whites, ' ', len);
                whites[len] = '\0';

                for (i = 1; i < m1 * n1; i++)
                {
                    if (strlen(Input_Strings[i]) > 0)
                    {
                        if (Input_Strings[i][strlen(Input_Strings[i]) - 1] == '\n')
                        {
                            sciprint(_("%s%s"), whites, Input_Strings[i]);
                        }
                        else
                        {
                            sciprint(_("%s%s\n"), whites, Input_Strings[i]);
                        }
                    }
                    else
                    {
                        sciprint(_("%s%s\n"), whites, "");
                    }
                }
                FREE(whites);
                sciprint("\n");
            }
            freeArrayOfString(Input_Strings, m1 * n1);
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        return 0;
    }
    LhsVar(1) = 0;
    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
