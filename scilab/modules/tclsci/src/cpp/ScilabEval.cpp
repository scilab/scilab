/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab-Enterprises - Cedric Delamarre
  *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "configvariable.hxx"
#include "threadmanagement.hxx"
#include "runner.hxx"

extern "C"
{
#include "TCL_Global.h"
#include "ScilabEval.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "storeCommand.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/
int TCL_EvalScilabCmd(ClientData clientData, Tcl_Interp * theinterp, int objc, CONST char ** argv)
{
    int ierr = 0;
    int isInterruptible = 1;
    char *pstCommand = NULL;

    if (argv[1] != (char *)0)
    {
        int ind = 0;
        pstCommand = os_strdup(argv[1]);
        if (pstCommand == NULL)
        {
            sciprint(_("%s: No more memory.\n"), "TCL_EvalScilabCmd");
            return TCL_ERROR;
        }

        std::string str(pstCommand);

        size_t pos = str.find('\n');
        while (pos != std::string::npos)
        {
            str.replace(pos, 1, "\\n");
            pos = str.find('\n');
        }

        FREE(pstCommand);
        pstCommand =  strdup(str.c_str());

        if (strncmp(pstCommand, "flush", 5) == 0)
        {
            // execute all commands before return to scilab prompt
            while (isEmptyCommandQueue() == false)
            {
                // Wake up Runner waiting for my execution ends
                ThreadManagement::SendAwakeRunnerSignal();
                ThreadManagement::WaitForRunMeSignal();
                try
                {
                    StaticRunner_launch();
                }
                catch (const ast::InternalAbort& /*ia*/)
                {
                    return TCL_OK;
                }
                catch (const ast::ScilabException& /*e*/)
                {
                    return TCL_ERROR;
                }
            }
        }
        else if ( (argv[2] != (char *)0) && (strncmp(argv[2], "sync", 4) == 0) )
        {
            command_origin_t iCmdOrigin = NONE;
            isInterruptible = ( (argv[3] != (char *)0) && (strncmp(argv[3], "seq", 3) == 0) ) ? 0 : 1;
            StoreCommandWithFlags(pstCommand, 1/*is prioritary*/, isInterruptible, TCLSCI/*command origin*/);

            // execute all commands stored before my TCL command
            // then execute the TCL command before return to the scilab prompt
            do
            {
                ThreadManagement::WaitForRunMeSignal();

                // get the origin of the command
                iCmdOrigin = StaticRunner_getCommandOrigin();
                try
                {
                    StaticRunner_launch();
                }
                catch (const ast::InternalAbort& /*ia*/)
                {
                    return TCL_OK;
                }
                catch (const ast::ScilabException& /*e*/)
                {
                    return TCL_ERROR;
                }

                // Wake up Runner waiting for my execution ends
                ThreadManagement::SendAwakeRunnerSignal();
            }
            // if the last command executed is not the TCL command, execute the next command.
            while (iCmdOrigin != TCLSCI);
        }
        else
        {
            isInterruptible = ( (argv[2] != (char *)0) && (strncmp(argv[2], "seq", 3) == 0) ) ? 0 : 1;
            StoreCommandWithFlags(pstCommand, 0/*is prioritary*/, isInterruptible, NONE/*command origin*/);
            if (isInterruptible)
            {
                Tcl_SetResult(theinterp, NULL, NULL);
            }
        }

        FREE(pstCommand);
    }
    else
    {
        /* ScilabEval called without argument */
        Scierror(999, _("%s: Wrong number of input argument(s): at least one expected.\n"), "TCL_EvalScilabCmd");
        return TCL_ERROR;
    }

    return TCL_OK;
}
