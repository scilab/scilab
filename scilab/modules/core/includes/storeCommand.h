/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA
 * Copyright (C) 2008-2008 - Bruno JOFRET
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
int StoreCommandWithFlags(const char* command, int iPrioritary, int iInterruptible, enum command_origin_t iCmdorigin);

/**
 * Store a non-prioritary and interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */
int StoreCommand(const char *command);

/**
 * Store a prioritary and interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */

int StoreConsoleCommand(const char *command, int iWaitFor);
/**
 * Store a prioritary and non-interruptible command
 *
 * @param command : the command
 * @return <ReturnValue>
 */
int StorePrioritaryCommand(const char *command);

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
