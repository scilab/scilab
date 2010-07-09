/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*------------------------------------------------------------------------*/
#include "CommandHistory.hxx"
#include "CommandHistory_Wrap.h"
extern "C"
{
#include "getScilabJavaVM.h"
#include "loadOnUseClassPath.h"
#include "BOOL.h"
#include "scilabmode.h"
};
/*------------------------------------------------------------------------*/
using namespace org_scilab_modules_history_browser;
static BOOL alreadyLoadedJava = FALSE;
/*------------------------------------------------------------------------*/
void CommandHistoryAppendLine (char * lineToAppend)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::appendLine(getScilabJavaVM(), lineToAppend);
    }
}
/*------------------------------------------------------------------------*/
void CommandHistoryLoadFromFile (void)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::loadFromFile(getScilabJavaVM());
    }
}
/*------------------------------------------------------------------------*/
void CommandHistoryInitialize (void)
{
    if (!alreadyLoadedJava && (getScilabMode() == SCILAB_STD))
    {
        loadOnUseClassPath("SciNotes");
        alreadyLoadedJava = TRUE;
    }

    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::initialize(getScilabJavaVM());
    }
}
/*------------------------------------------------------------------------*/
void CommandHistoryReset(void)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::reset(getScilabJavaVM());
    }
}
/*------------------------------------------------------------------------*/
void CommandHistoryDeleteLine(int lineNumber)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::deleteLine(getScilabJavaVM(), lineNumber);
    }
}
/*------------------------------------------------------------------------*/
