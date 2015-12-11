/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA
 * Copyright (C) 2008-2008 - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __STORECOMMAND_H__
#define __STORECOMMAND_H__

enum command_origin_t
{
    NONE,
    CONSOLE, // command from console
    TCLSCI   // command from tclsci using ScilabEval interpreter
};

/**
 * Store a non-prioritary and interruptible command
 *
 * @param command           : command wich will be stored
 * @param piInterruptible   : 1 if it is a interruptible command
 * @param piPrioritary      : 1 if it is a prioritary command
 * @param iCmdorigin        : origine of the command
 * @return <ReturnValue>
 */
int StoreCommandWithFlags(char* command, int iPrioritary, int iInterruptible, enum command_origin_t iCmdorigin);

/**
 * Store a non-prioritary and interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */
int StoreCommand(char *command);

/**
 * Store a prioritary and interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */

int StoreConsoleCommand(char *command, int iWaitFor);
/**
 * Store a prioritary and non-interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */
int StorePrioritaryCommand(char *command);

/**
 * Get the next command to execute
 *
 * @param command           : command wich will be executed
 * @param piInterruptible   : 1 if it is a interruptible command
 * @param piPrioritary      : 1 if it is a prioritary command
 * @param iCmdorigin        : origine of the command
 * @return <ReturnValue>    : 0 if command queue is empty
 */
int GetCommand(char** command, int* piPrioritary, int* piInterruptible, enum command_origin_t* iCmdorigin);

/**
* check if command queue is empty
* @return 1 if empty , 0 is not empty
*/
int isEmptyCommandQueue(void);

/*
 * Checks if there's something on the
 * commandQueue
 * @return
 */
int ismenu(void);


#endif /* __STORECOMMAND_H__ */
