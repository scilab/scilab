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

#ifndef __GETSCILABPREFERENCES_H__
#define __GETSCILABPREFERENCES_H__

#include "dynlib_preferences_scilab.h"

typedef struct
{
    const char * heapSize;
    const char * adaptToDisplay;
    const char * columnsToDisplay;
    const char * linesToDisplay;
    const char * historySaveAfter;
    const char * historyFile;
    const char * historyLines;
    const char * historyEnable;
    const char * ieee;
    const char * format;
    const char * formatWidth;
    const char * language;
    const char * startup_dir_use;
    const char * startup_dir_default;
    const char * startup_dir_previous;
    const char * recursionlimit;
} ScilabPreferences;

#define HEAPSIZE_XPATH (const xmlChar*)"//general/body/java-heap-memory/@heap-size"
#define ADAPTTODISPLAY_XPATH (const xmlChar*)"//console/body/display/@adapt-to-display"
#define COLUMNSTODISPLAY_XPATH (const xmlChar*)"//console/body/display/@columns-to-display"
#define LINESTODISPLAY_XPATH (const xmlChar*)"//console/body/display/@lines-to-display"
#define HISTORYSAVEAFTER_XPATH (const xmlChar*)"//command-history/body/history-save/@after"
#define HISTORYFILE_XPATH (const xmlChar*)"//command-history/body/history-settings/@history-file"
#define HISTORYLINES_XPATH (const xmlChar*)"//command-history/body/history-settings/@history-lines"
#define HISTORYENABLE_XPATH (const xmlChar*)"//command-history/body/history-settings/@enable"
#define IEEE_XPATH (const xmlChar*)"//general/body/environment/@fpe"
#define FORMAT_XPATH (const xmlChar*)"//general/body/environment/@printing-format"
#define FORMATWIDTH_XPATH (const xmlChar*)"//general/body/environment/@width"
#define RECURSIONLIMIT_XPATH (const xmlChar*)"//general/body/environment/@recursion-limit"
#define LANGUAGE_XPATH (const xmlChar*)"//general/body/languages/@lang"
#define STARTUP_DIR_USE_XPATH (const xmlChar*)"//general/body/startup/@use"
#define STARTUP_DIR_DEFAULT_XPATH (const xmlChar*)"//general/body/startup/@default"
#define STARTUP_DIR_PREVIOUS_XPATH (const xmlChar*)"//general/body/startup/@previous"

PREFERENCES_SCILAB_IMPEXP const ScilabPreferences * getScilabPreferences(void);
PREFERENCES_SCILAB_IMPEXP void reloadScilabPreferences(void);
PREFERENCES_SCILAB_IMPEXP void clearScilabPreferences(void);
PREFERENCES_SCILAB_IMPEXP char * getPrefAttributeValue(const char * xpath, const char * attribute);
PREFERENCES_SCILAB_IMPEXP char ** getPrefAttributesValues(const char * xpath, const char ** attributes, const unsigned int attrLen);
PREFERENCES_SCILAB_IMPEXP void setAttributesValues(const char * xpath, const char ** attributes, const unsigned int kvLen);
PREFERENCES_SCILAB_IMPEXP void setPrefAttributesValues(const char * xpath, const char ** kv, const unsigned int kvLen);

#endif // __GETSCILABPREFERENCES_H__
