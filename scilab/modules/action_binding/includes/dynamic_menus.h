/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA
 * Copyright (C) 2008-2008 - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DYNAMIC_MENUS_H__
#define __DYNAMIC_MENUS_H__

#include "dynlib_action_binding.h"
#include "machine.h"

/*
 * Checks if there's something on the
 * commandQueue
 * @return
 */
ACTION_BINDING_IMPEXP int ismenu(void);

/**
 * try to execute a command or add it to the end of command queue
 *
 * @param command the command
 * @return <ReturnValue>
 */
ACTION_BINDING_IMPEXP int StoreCommand (char *command);

/**
 * try to execute a command or add it to the end of command queue
 *
 * @param command the command
 * @param flag a internal execution flag for sequential execution
 * @return <ReturnValue>
 */
ACTION_BINDING_IMPEXP int StoreCommandWithFlag (char *command, int flag);

/**
 * @TODO add comment
 *
 * @param str
 * @return <ReturnValue>
 */
ACTION_BINDING_IMPEXP int GetCommand (char *str);

/**
* try to execute a command or add it to the _BEGINNING_ of command queue
* flag = 0 : the command is not shown in scilab window
* flag = 1 : the command is shown in scilab window (if at prompt) and executed sequentially
*/
ACTION_BINDING_IMPEXP int StorePrioritaryCommandWithFlag (char *command,int flag);

/**
*
*/
ACTION_BINDING_IMPEXP  int C2F(getmen)(char * btn_cmd,int * lb, int * entry);

#endif /* __DYNAMIC_MENUS_H__ */
