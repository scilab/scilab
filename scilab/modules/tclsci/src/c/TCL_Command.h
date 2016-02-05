/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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

/*
** Manage Tcl Loop as a standalone one.
** An  important  constraint  of  the  Tcl  threads implementation
** is that only the thread that created a Tcl interpreter can use that interpreter.
*/

#ifndef __TCL_COMMAND_H__
#define __TCL_COMMAND_H__

#include <tcl.h>
#include <string.h>

#include "Thread_Wrapper.h"

/*
** TCL Event loop refresh rate.
*/
#define TIME_TO_SLEEP 10000

/*
** This function start an endless Tcl Loop
** in order the Scilab Global Tcl Interpreter
** do some "update" and let the Tcl Applications live.
*/
void		startTclLoop(void);

/*
** Send Tcl Command
** Ask the interpreter to execute some command.
** Manage mutex and lock process.
*/
#define		sendTclCommand(command)	sendTclCommandToSlave(command, NULL)

/*
** Send Tcl File
** Ask the interpreter to load some file
** Manage mutex and lock process.
*/
#define		sendTclFile(command)	sendTclFileToSlave(command, NULL)

/*
** Send Tcl Command To Slave
** Ask the interpreter to execute some command.
** Manage mutex and lock process.
*/

int		sendTclCommandToSlave(char* command, char* slave);

/*
** Send Tcl File To Slave
** Ask the interpreter to load some file
** Manage mutex and lock process.
*/
int		sendTclFileToSlave(char* command, char* slave);

/*
** Get Tcl Command Return Value
** Porpagate return value of Tcl Interpreter
** caught in thread.
*/
int		getTclCommandReturn(void);

/*
** Get Tcl Command Result
** Porpagate return message of Tcl Interpreter
** caught in thread.
** This can be an error or a string value.
*/
char		*getTclCommandResult(void);

/* The tclLoop thread Id
in order to wait it ends when closing Scilab */
__threadId TclThread;

__threadSignal InterpReady;
__threadSignalLock InterpReadyLock;

#endif /* !__TCL_COMMAND_H__ */
