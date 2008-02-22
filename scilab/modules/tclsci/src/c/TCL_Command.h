/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

#include "machine.h"
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

#endif /* !__TCL_COMMAND_H__ */
