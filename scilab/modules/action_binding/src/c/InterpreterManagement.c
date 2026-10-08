/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Clément DAVID
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

#include "InterpreterManagement.h"
#include "storeCommand.h"
#include "callCommand.h"
#include "configvariable_interface.h"
#include "scilabWrite.hxx"
/*--------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
    return StoreCommandWithFlags(command, 0, 1, INTERNAL);
}
/*--------------------------------------------------------------------------*/
/*
* requestScilabExec
*
* WARNING : if the command is taking some time, scilab will not do anything else
* before the command returns.
*/
int requestScilabExec(char *command)
{
    return StoreCommandWithFlags(command, 1, 0, INTERNAL);
}
/*--------------------------------------------------------------------------*/
int putCallbackInScilabQueue(char *command)
{
    return StoreCommand(command);
}
/*--------------------------------------------------------------------------*/
int requestCallbackExec(char *command)
{
    return StorePrioritaryCommand(command);
}
/*--------------------------------------------------------------------------*/
int callScilabExecstr(const char *command)
{
    return callCommandFromGateway(command);
}
/*--------------------------------------------------------------------------*/
int interruptScilab(void)
{
    setExecutionBreak();
    return 0;
}
/*--------------------------------------------------------------------------*/
void startScilabOutputCapture(int silent)
{
    scilabStartOutputCapture(silent);
}
/*--------------------------------------------------------------------------*/
char* stopScilabOutputCapture(void)
{
    return scilabStopOutputCapture();
}
/*--------------------------------------------------------------------------*/
int getScilabDebugState(void)
{
    return getDebugState();
}
/*--------------------------------------------------------------------------*/
