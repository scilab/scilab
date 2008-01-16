/*
**  -*- C -*-
**
** TCL_Command.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Wed Jan  9 09:16:50 2008 bruno
** Last update Thu Jan 10 15:43:08 2008 bruno
**
** Copyright INRIA 2008
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
void		startTclLoop(Tcl_Interp *TCLInterp);

/*
** Send Tcl Command
** Ask the interpreter to execute some command.
** Manage mutex and lock process.
*/
void		sendTclCommand(char* command);

/*
** Send Tcl Command To Slave
** Ask the interpreter to execute some command.
** Manage mutex and lock process.
*/

void		sendTclCommandToSlave(char* command, char* slave);
/*
** Get Tcl Command Return Value
** Porpagate return value of Tcl Interpreter
** caught in thread.
*/
int getTclCommandReturn(void);

/*
** Get Tcl Command Result
** Porpagate return message of Tcl Interpreter
** caught in thread.
** This can be an error or a string value.
*/
char *getTclCommandResult(void);

#endif /* !__TCL_COMMAND_H__ */
