/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include <stdlib.h>

#include "getScilabPreference.h"
#include "api_scilab.h"
#include "configvariable_interface.h"
#include "setlines.h"
#include "setformat.h"
#include "os_string.h"
#include "TerminateHistoryManager.h"
#include "InitializeHistoryManager.h"
#include "HistoryManager.h"
#include "scicurdir.h"

void InitializePreferences(int useCWD)
{
    const ScilabPreferences * prefs = getScilabPreferences();

    // Set ieee
    if (prefs->ieee)
    {
        int ieee = atoi(prefs->ieee);
        setieee(ieee);
    }

    // Set format
    if (prefs->formatWidth && prefs->format)
    {
        int formatWidth = (int)atof(prefs->formatWidth);
        formatWidth = Max(0, formatWidth);
        formatWidth = Min(25, formatWidth);
        setformat(prefs->format, formatWidth);
    }

    // Set history
    if (prefs->historyEnable)
    {
        if (!stricmp(prefs->historyEnable, "true"))
        {
            if (prefs->historyFile && prefs->historyLines)
            {
                int historyLines = 0;
                InitializeHistoryManager();
                setFilenameScilabHistory((char*)prefs->historyFile);
                historyLines = (int)atof(prefs->historyLines);
                if (historyLines > 0)
                {
                    setSizeMaxScilabHistory(historyLines);
                }
            }
        }
        else
        {
            TerminateHistoryManager();
        }
    }

    // Set lines
    if (prefs->adaptToDisplay && prefs->columnsToDisplay && prefs->linesToDisplay)
    {
        if (stricmp(prefs->adaptToDisplay, "true"))
        {
            // it is not true so ...
            int lines = (int)atof(prefs->linesToDisplay);
            int cols = (int)atof(prefs->columnsToDisplay);
            setConsoleLines(lines);
            setConsoleWidth(cols);
        }
    }

    // Set current directory
    if (!useCWD && prefs->startup_dir_use)
    {
        if (stricmp(prefs->startup_dir_use, "current"))
        {
            // Not in cwd
            if (stricmp(prefs->startup_dir_use, "previous") == 0 && prefs->startup_dir_previous && *prefs->startup_dir_previous)
            {
                scichdir((char*)prefs->startup_dir_previous);
            }
            else if (stricmp(prefs->startup_dir_use, "default") == 0 && prefs->startup_dir_default && *prefs->startup_dir_default)
            {
                scichdir((char*)prefs->startup_dir_default);
            }
        }
    }

    //recursion limit
    if (prefs->recursionlimit)
    {
        int recursionlimit = atoi(prefs->recursionlimit);
        setRecursionLimit(recursionlimit);
    }


    clearScilabPreferences();
}
