/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include "machine.h"
#include "TCL_Global.h"
#include "ScilabEval.h"
#include "sciprint.h"
#include "sciprint_full.h"
#include "Scierror.h"
#include "localization.h"
#include "syncexec.h"
#include "storeCommand.h"
#include "os_string.h"
/*--------------------------------------------------------------------------*/
/* what's the max number of commands in the queue ??*/
#define arbitrary_max_queued_callbacks 20
#define AddCharacters 4
/*--------------------------------------------------------------------------*/
/* Used by tksynchro
 * static int c_n1 = -1;
 */
/*--------------------------------------------------------------------------*/
int TCL_EvalScilabCmd(ClientData clientData, Tcl_Interp * theinterp, int objc, CONST char ** argv)
{
    //    int ierr = 0, seq = 0;
    //    wchar_t *pwstCommand = NULL;
    //    char *pstCommand = NULL;
    //
    //    wchar_t* pwstComm = NULL;
    //    char *comm[arbitrary_max_queued_callbacks];
    //    int   seqf[arbitrary_max_queued_callbacks];
    //    int nc, ncomm = -1;
    //
    //    if (C2F(iop).ddt == -1)
    //    {
    //        /* trace for debugging */
    //        int argc = 1;
    //        char *msg = _("TCL_EvalScilabCmd %s");
    //
    //        sciprint_full(msg, argv[1]);
    //
    //        while (argv[++argc])
    //        {
    //            sciprint(" %s", argv[argc]);
    //        }
    //        sciprint("\n");
    //
    //    }
    //
    //    if (argv[1] != (char *)0)
    //    {
    //        pwstCommand = to_wide_string(argv[1]);
    //        pstCommand = os_strdup(argv[1]);
    //
    //        if (pwstCommand == NULL || pstCommand == NULL)
    //        {
    //            sciprint(_("%s: No more memory.\n"), "TCL_EvalScilabCmd");
    //            return TCL_ERROR;
    //        }
    //
    //
    //        if ( (argv[2] != (char *)0) && (strncmp(argv[2], "sync", 4) == 0) )
    //        {
    //            /* sync or sync seq
    //             * TODO : Scilab is supposed to be busy there. Add mutex lock...
    //             * C2F(tksynchro)(&c_n1);
    //             * set sciprompt to -1 (scilab busy)
    //             */
    //            seq = ( (argv[3] != (char *)0) && (strncmp(argv[3], "seq", 3) == 0) );
    //
    //            if (C2F(iop).ddt == -1)
    //            {
    //                char *msg = _("Execution starts for %s");
    //                sciprint_full(msg, pstCommand);
    //                sciprint("\n");
    //            }
    //
    //            /*
    //            int ns=(int)strlen(command);
    //            Was : syncexec(command,&ns,&ierr,&seq,ns);
    //            So far as Tcl has it's own thread now mixing global values
    //            and threads within parse makes Scilab crash often.
    //            */
    //            //            StorePrioritaryCommandWithFlag(pwstCommand, seq);
    //            ierr = 0;
    //
    //            if (C2F(iop).ddt == -1)
    //            {
    //                char *msg = _("Execution ends for %s");
    //                sciprint_full(msg, pstCommand);
    //                sciprint("\n");
    //            }
    //            // TODO : Scilab is supposed to be busy there. Add mutex lock...
    //            // C2F(tksynchro)(&C2F(recu).paus);
    //            if (ierr != 0)
    //            {
    //                return TCL_ERROR;
    //            }
    //        }
    //        else if (strncmp(pstCommand, "flush", 5) == 0)
    //        {
    //            /* flush */
    //            if (C2F(iop).ddt == -1)
    //            {
    //                sciprint(_(" Flushing starts for queued commands.\n"));
    //            }
    //            while (ismenu() && ncomm < arbitrary_max_queued_callbacks - 1)
    //            {
    //                ncomm++;
    //                comm[ncomm] = (char *) MALLOC (bsiz + 1);
    //                if (comm[ncomm] == (char *) 0)
    //                {
    //                    sciprint(_("%s: No more memory.\n"), "TCL_EvalScilabCmd");
    //                    FREE(pwstCommand);
    //                    FREE(pstCommand);
    //                    return TCL_ERROR;
    //                }
    //                //                seqf[ncomm] = GetCommand (comm[ncomm]);
    //            }
    //            if (ismenu())
    //            {
    //                sciprint(_("Warning: Too many callbacks in queue!\n"));
    //            }
    //            for (nc = 0 ; nc <= ncomm ; nc++ )
    //            {
    //                // TODO : Scilab is supposed to be busy there. Add mutex lock...
    //                // C2F(tksynchro)(&c_n1);  // set sciprompt to -1 (scilab busy)
    //                if (C2F(iop).ddt == -1)
    //                {
    //                    if (seqf[nc] == 0)
    //                    {
    //                        char *msg = _("Flushed execution starts for %s - No option");
    //                        sciprint_full(msg, comm[nc]);
    //                        sciprint("\n");
    //                    }
    //                    else
    //                    {
    //                        char *msg = _("Flushed execution starts for %s - seq");
    //                        sciprint_full(msg, comm[nc]);
    //                        sciprint("\n");
    //                    }
    //                }
    //                /*
    //                Was : syncexec(comm[nc],&ns,&ierr,&(seqf[nc]),ns);
    //                So far as Tcl has it's own thread now mixing global values
    //                and threads within parse makes Scilab crash often.
    //                */
    //                pwstComm = to_wide_string(comm[nc]);
    //                //                StorePrioritaryCommandWithFlag(pwstComm, seqf[nc]);
    //                FREE(pwstComm);
    //                if (C2F(iop).ddt == -1)
    //                {
    //                    char *msg = _("Flushed execution ends for %s");
    //                    sciprint_full(msg, comm[nc]);
    //                    sciprint("\n");
    //                }
    //                FREE(comm[nc]);
    //                // TODO : Scilab is supposed to be busy there. Add mutex lock...
    //                // C2F(tksynchro)(&C2F(recu).paus);
    //                if (ierr != 0)
    //                {
    //                    return TCL_ERROR;
    //                }
    //            }
    //            if (C2F(iop).ddt == -1)
    //            {
    //                sciprint(_("Flushing ends\n"));
    //            }
    //        }
    //        else
    //        {
    //            if ( (argv[2] != (char *)0) && (strncmp(argv[2], "seq", 3) == 0) )
    //            {
    //                /* seq */
    //                //                StoreCommandWithFlag(pwstCommand, 1);
    //            }
    //            else
    //            {
    //                /* no option or unknown option (TODO: no error for this latter case?) */
    //                //                StoreCommand(pwstCommand);
    //                Tcl_SetResult(theinterp, NULL, NULL);
    //            }
    //        }
    //        FREE(pwstCommand);
    //        FREE(pstCommand);
    //
    //    }
    //    else
    //    {
    //        /* ScilabEval called without argument */
    //        Scierror(999, _("%s: Wrong number of input argument(s): at least one expected.\n"), "TCL_EvalScilabCmd");
    //    }

    return TCL_OK;
}
