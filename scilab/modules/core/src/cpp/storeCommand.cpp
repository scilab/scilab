/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "storeCommand.h"
#include "Thread_Wrapper.h"
}

#include "parser.hxx"
#include "execvisitor.hxx"
#include "tasks.hxx"
#include "yaspio.hxx"

/*--------------------------------------------------------------------------*/
/*
 *  Command queue functions
 *  This function is used to store Scilab command in a queue
 *
 *  PUBLIC : int StoreCommand( char *command)
 *           int C2F(ismenu)()
 *           int C2F(getmen)(char * btn_cmd,int * lb, int * entry)
 */
/*--------------------------------------------------------------------------*/
#define IMPORT_SIGNAL extern
/*--------------------------------------------------------------------------*/
typedef struct commandRec
{
	char              *command;		/* command info one string two integers */
	int               flag; /* 1 if the command execution cannot be interrupted */
	struct commandRec *next;
} CommandRec;
/*--------------------------------------------------------------------------*/
/* Extern Signal to say we git a StoreCommand. */
IMPORT_SIGNAL __threadSignal LaunchScilab;
/*--------------------------------------------------------------------------*/
static CommandRec *commandQueue = NULL;
static __threadLock commandQueueSingleAccess = __StaticInitLock;
/*--------------------------------------------------------------------------*/
int StoreCommand (char *command)
{
	return (StoreCommandWithFlag (command, 0));
}
/*--------------------------------------------------------------------------*/
/*
 * try to execute a command or add it to the end of command queue
 * flag = 0 : the command is not shown in scilab window
 * flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
 */
int StoreCommandWithFlag (char *command,int flag)
{
    try
    {
        Parser::getInstance()->parse(command);
        ast::ExecVisitor exec;
        Parser::getInstance()->getTree()->accept(exec);
    }
    catch (std::string error)
    {
        YaspWrite("\n");
        YaspWrite("\n");
        YaspWrite(command);
        YaspWrite("\n");
        YaspWrite(const_cast<char *>(error.c_str()));
        YaspWrite("\n");
        YaspWrite(_("while executing a callback"));
    }   
	return (0);
}
/*--------------------------------------------------------------------------*/
/*
 * try to execute a command or add it to the _BEGINNING_ of command queue
 * flag = 0 : the command is not shown in scilab window
 * flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
 */
int StorePrioritaryCommandWithFlag (char *command,int flag)
{
    try
    {
        Parser::getInstance()->parse(command);
        ast::ExecVisitor exec;
        Parser::getInstance()->getTree()->accept(exec);
    }
    catch (std::string error)
    {
        YaspWrite("\n");
        YaspWrite("\n");
        YaspWrite(command);
        YaspWrite("\n");
        YaspWrite(const_cast<char *>(error.c_str()));
        YaspWrite("\n");
        YaspWrite(_("while executing a callback"));
    }
	return (0);
}
/*--------------------------------------------------------------------------*/
int isEmptyCommandQueue(void)
{
#pragma warning("isEmptyCommandQueue is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
	// FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
/*
 * Gets info on the first queue element
 * and remove it from the queue
 */
int GetCommand ( char *str)
{
#pragma warning("GetCommand is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
	// FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
int ismenu(void)
{
#pragma warning("ismenu is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
	// FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
/* menu/button info for Scilab */
int C2F(getmen)(char * btn_cmd,int * lb, int * entry)
{
#pragma warning("C2F(getmen) is deprecated. It will be removed _BEFORE_ Scilab 6.0.")
	// FIXME : Do not forget to remove me.
    return 0;
}
/*--------------------------------------------------------------------------*/
