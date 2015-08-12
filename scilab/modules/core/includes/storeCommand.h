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

#include <wchar.h>
#include "machine.h"

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
 * @param piConsole         : 1 if it is a console command
 * @return <ReturnValue>    : 0 if command queue is empty
 */
int GetCommand (char** command, int* piInterruptible, int* piPrioritary, int* piConsole);

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


/**
*
*/
int C2F(getmen)(char * btn_cmd, int * lb, int * entry);


#endif /* __STORECOMMAND_H__ */
