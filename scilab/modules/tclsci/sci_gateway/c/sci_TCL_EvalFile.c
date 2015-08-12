/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2005-2008 - INRIA - Allan CORNET
*  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "gw_tclsci.h"
#include "TCL_Command.h"
#include "TCL_Global.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"

#include "FileExist.h"
#include "TCL_getErrorLine.h"
#include "getshortpathname.h"

/*--------------------------------------------------------------------------*/
int sci_TCL_EvalFile(char *fname, unsigned long l)
{
    /* execute Tcl scripts */
    int m1, n1, l1;
    int m2, n2, l2;
    int RET;

    Tcl_Interp *TCLinterpreter = NULL;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (GetType(1) == sci_strings)
    {
        GetRhsVar(1, STRING_DATATYPE, &m1, &n1, &l1);
        /* Check if there is a global interpreter */
        TCLinterpreter = getTclInterp();
        releaseTclInterp();
        if (TCLinterpreter == NULL)
        {
            Scierror(999, _("%s: Error main TCL interpreter not initialized.\n"), fname);
            return 0;
        }

        /* Check if the file to load exists*/
        if (!FileExist(cstk(l1)))
        {
            Scierror(999, _("%s: File %s not found.\n"), fname, cstk(l1));
            return 0;
        }

        if (Rhs == 2)
        {
            /* two arguments given - get a pointer on the slave interpreter */
            if (GetType(2) == sci_strings)
            {
                GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2);
                TCLinterpreter = Tcl_GetSlave(getTclInterp(), cstk(l2));
                releaseTclInterp();
                if (TCLinterpreter == NULL)
                {
                    Scierror(999, _("%s: No such slave interpreter.\n"), fname);
                    return 0;
                }

                {
                    BOOL bOK = FALSE;
                    char *sz = cstk(l1);
                    char *szShort = getshortpathname(sz, &bOK);
                    RET = sendTclFileToSlave(szShort, cstk(l2));
                    FREE(szShort);
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
                return 0;
            }
        }
        else
        {
            BOOL bOK = FALSE;
            char *sz = cstk(l1);
            char *szShort = getshortpathname(sz, &bOK);
            RET = sendTclFile(szShort);
            FREE(szShort);
        }

        if (RET == TCL_ERROR)
        {
            const char *trace = Tcl_GetVar(TCLinterpreter, "errorInfo", TCL_GLOBAL_ONLY);
            if (Err > 0)
            {
                sciprint(_("%s, at line %i of file %s\n	%s.\n"), fname, TCL_getErrorLine(TCLinterpreter), cstk(l1), (char *)trace);
            }
            else
            {
                Scierror(999, _("%s, at line %i of file %s\n	%s.\n"), fname, TCL_getErrorLine(TCLinterpreter), cstk(l1), (char *)trace);
                return 0;
            }
        }
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
        return 0;
    }

    LhsVar(1) = 0;
    PutLhsVar();
    return 0;
}
/*--------------------------------------------------------------------------*/
