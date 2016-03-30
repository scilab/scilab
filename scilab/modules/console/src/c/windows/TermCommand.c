/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include "TermCommand.h"
#include "machine.h" /* C2F */
#include "storeCommand.h" /* StoreCommand */
#include "clrscr_nw.h"
#include "TermLine.h"
#include "configvariable_interface.h"
/*--------------------------------------------------------------------------*/
void ControlC_Command(void)
{
    setExecutionBreak();
}
/*--------------------------------------------------------------------------*/
void ControlX_Command(void)
{
    ControlC_Command();
}
/*--------------------------------------------------------------------------*/
void F1_Command(void)
{
    StoreCommand("help");
}
/*--------------------------------------------------------------------------*/
void F2_Command(void)
{
    clrscr_nw();
    displayPrompt();
}
/*--------------------------------------------------------------------------*/
void ALTF4_Command(void)
{
    StoreCommand("quit");
}
/*--------------------------------------------------------------------------*/
