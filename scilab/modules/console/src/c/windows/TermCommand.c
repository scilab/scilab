/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "TermCommand.h"
#include "machine.h" /* C2F */
#include "sigbas.h" /* C2F (sigbas) */
#include "storeCommand.h" /* StoreCommand */
#include "clrscr_nw.h"
#include "TermLine.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
void ControlC_Command(void)
{
    int j = 2;
    C2F (sigbas) (&j);
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
