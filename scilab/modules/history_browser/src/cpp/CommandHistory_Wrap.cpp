/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011  - DIGITEO - Allan CORNET
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
/*------------------------------------------------------------------------*/
#include "CommandHistory.hxx"
#include "CommandHistory_Wrap.h"
extern "C"
{
#include "getScilabJavaVM.h"
#include "loadOnUseClassPath.h"
#include "BOOL.h"
#include "configvariable_interface.h"
};
/*------------------------------------------------------------------------*/
using namespace org_scilab_modules_history_browser;
static BOOL alreadyLoadedJava = FALSE;
/*------------------------------------------------------------------------*/
void CommandHistoryAppendLine (char* _pstLine)
{
    if (getScilabMode() == SCILAB_STD)
    {
        if (strlen(_pstLine) != 0)
        {
            CommandHistory::appendLine(getScilabJavaVM(), _pstLine);
        }
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
        loadOnUseClassPath(const_cast<char*>("SciNotes"));
        alreadyLoadedJava = TRUE;
    }

    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::initialize(getScilabJavaVM());
    }
}
/*------------------------------------------------------------------------*/
void CommandHistoryExpandAll (void)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::expandAll(getScilabJavaVM());
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
void CommandHistoryLaunch(void)
{
    if (getScilabMode() == SCILAB_STD)
    {
        CommandHistory::launchHistoryBrowser(getScilabJavaVM());
    }
}
/*------------------------------------------------------------------------*/
