/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/**
 * @author Vincent COUVERT
 */

#ifndef __INTERPRETERMANAGEMENT_H__
#define __INTERPRETERMANAGEMENT_H__

#include "BOOL.h" /* BOOL */

/**
 * Put a command in Scilab command queue so that Scilab executes it.
 * The command is displayed in Scilab Window
 *
 * @param command the command to execute
 * @return execution status
 */
int putCommandInScilabQueue(char *command);


/**
 * Put a command in Scilab command queue so that Scilab executes it.
 * The command is executed as soon as possible and may not be interrupted by another one.
 * WARNING : if the command is taking some time, scilab will not do anything else
 * before the command returns.
 * The command is displayed in Scilab Window
 *
 * @param command the command to execute
 * @return execution status
 */
int requestScilabExec(char *command);
/*
* Stops Scilab current work
* @return execution status
*/
int interruptScilab(void);

/*--------------------------------------------------------------------------*/

#endif /* __INTERPRETERMANAGEMENT_H__ */
