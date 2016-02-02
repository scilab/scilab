/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
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
