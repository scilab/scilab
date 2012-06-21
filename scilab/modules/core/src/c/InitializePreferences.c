/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h>

#include "getScilabPreference.h"
#include "api_scilab.h"
#include "setieee.h"
#include "setlines.h"
#include "setformat.h"
#include "stricmp.h"
#include "TerminateHistoryManager.h"
#include "InitializeHistoryManager.h"
#include "HistoryManager.h"

void InitializePreferences()
{
    const ScilabPreferences * prefs = getScilabPreferences();
    int ieee = 0;
    int lines = 0;
    int cols = 0;
    int formatWidth = 0;
    int historyLines = 0;

    // Set ieee
    if (prefs->ieee)
    {
        ieee = atoi(prefs->ieee);
        setieee(ieee);
    }

    // Set format
    if (prefs->formatWidth && prefs->format)
    {
        formatWidth = (int)atof(prefs->formatWidth);
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
            lines = (int)atof(prefs->linesToDisplay);
            cols = (int)atof(prefs->columnsToDisplay);
            setlines(lines, cols);
        }
    }


    clearScilabPreferences();
}
