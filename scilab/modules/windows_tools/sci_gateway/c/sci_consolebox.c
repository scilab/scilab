/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "scilabmode.h"
#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
#include "sciprint.h"
#include "WinConsole.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int sci_consolebox(char *fname, unsigned long l)
{
    static int l1, n1, m1;
    char *Output = NULL;

    Rhs = Max(Rhs, 0);
    CheckRhs(0, 1);
    CheckLhs(0, 1);

    Output = (char*)MALLOC(4 * sizeof(char));

    if (Rhs == 0)
    {
        if (getScilabMode() == SCILAB_STD)
        {
            if (GetConsoleState())
            {
                strcpy(Output, "on");
            }
            else
            {
                strcpy(Output, "off");
            }

        }
        else
        {
            sciprint(_("Only on Windows Mode, not in Console Mode.\n"));
            strcpy(Output, "off");
        }
    }
    else if (GetType(1) == sci_strings)
    {
        char *param = NULL;

        GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
        param = cstk(l1);

        if ( (strcmp(param, "off") == 0) || (strcmp(param, "on") == 0) || (strcmp(param, "toggle") == 0) )
        {
            if (getScilabMode() == SCILAB_STD)
            {
                if (strcmp(param, "on") == 0)
                {
                    SetConsoleState(1);
                    ShowScilex();
                    strcpy(Output, "on");
                }
                else if (strcmp(param, "off") == 0)
                {
                    SetConsoleState(0);
                    HideScilex();
                    strcpy(Output, "off");
                }
                else /* toggle */
                {
                    SwitchConsole();
                    if (GetConsoleState())
                    {
                        strcpy(Output, "on");
                    }
                    else
                    {
                        strcpy(Output, "off");
                    }
                }
            }
            else
            {
                sciprint(_("Only on Windows Mode, not in Console Mode.\n"));
                strcpy(Output, "off");
            }
        }
        else
        {
            Scierror(999, ("%s: Wrong input argument: '%s', '%s' or '%s' expected.\n"), fname, "on", "off", "toggle");
            return 0;
        }
    }
    else
    {
        Scierror(999, ("%s: Wrong input argument: '%s', '%s' or '%s' expected.\n"), fname, "on", "off", "toggle");
        return 0;
    }

    n1 = 1;
    CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, (m1 = (int)strlen(Output), &m1), &n1, &Output);
    if (Output)
    {
        FREE(Output);
        Output = NULL;
    }
    LhsVar(1) = Rhs + 1;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
