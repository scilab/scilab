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

#include "getScilabPreference.h"
#include "api_scilab.h"
#include "setieee.h"
#include "setformat.h"

void InitializePreferences()
{
    const ScilabPreferences * prefs = getScilabPreferences();
    int ieee = 0;
    int formatWidth = 0;

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

    clearScilabPreferences();
}
