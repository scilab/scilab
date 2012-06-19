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

#ifndef __GETSCILABPREFERENCES_H__
#define __GETSCILABPREFERENCES_H__

#include "dynlib_preferences_scilab.h"

typedef struct
{
    const char * heapSize;
    const char * columnsToDisplay;
    const char * linesToDisplay;
    const char * historySaveAfter;
    const char * historyFile;
    const char * historyLines;
} ScilabPreferences;

#define HEAPSIZE_XPATH (const xmlChar*)"//general/body/java-heap-memory/@heap-size"
#define COLUMNSTODISPLAY_XPATH (const xmlChar*)"//console/body/display/@columns-to-display"
#define LINESTODISPLAY_XPATH (const xmlChar*)"//console/body/display/@lines-to-display"
#define HISTORYSAVEAFTER_XPATH (const xmlChar*)"//command-history/body/history-save/@after"
#define HISTORYFILE_XPATH (const xmlChar*)"//command-history/body/history-settings/@history-file"
#define HISTORYLINES_XPATH (const xmlChar*)"//command-history/body/history-settings/@history-lines"

PREFERENCES_SCILAB_IMPEXP const ScilabPreferences * getScilabPreferences();
PREFERENCES_SCILAB_IMPEXP void reloadScilabPreferences();
PREFERENCES_SCILAB_IMPEXP void clearScilabPreferences();

#endif // __GETSCILABPREFERENCES_H__
