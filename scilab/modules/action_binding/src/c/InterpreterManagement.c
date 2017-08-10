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

#include "InterpreterManagement.h"
#include "storeCommand.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
    return StoreCommand(command);
}
/*--------------------------------------------------------------------------*/
/*
* requestScilabExec
*
* WARNING : if the command is taking some time, scilab will not do anything else
* before the command returns.
*/
int requestScilabExec(char *command)
{
    return StorePrioritaryCommand(command);
}
/*--------------------------------------------------------------------------*/
int interruptScilab(void)
{
    setExecutionBreak();
    return 0;
}
/*--------------------------------------------------------------------------*/
