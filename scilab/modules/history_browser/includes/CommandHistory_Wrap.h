/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
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

#ifndef __COMMANDHISTORY_WRAP_H__
#define __COMMANDHISTORY_WRAP_H__

#ifdef __cplusplus

extern "C"
{
#endif

#include "dynlib_history_browser.h"

HISTORY_BROWSER_IMPEXP void CommandHistoryAppendLine (char* _pstLine);

HISTORY_BROWSER_IMPEXP void CommandHistoryLoadFromFile (void);

HISTORY_BROWSER_IMPEXP void CommandHistoryInitialize (void);

HISTORY_BROWSER_IMPEXP void CommandHistoryExpandAll (void);

HISTORY_BROWSER_IMPEXP void CommandHistoryReset(void);

HISTORY_BROWSER_IMPEXP void CommandHistoryDeleteLine(int lineNumber);

HISTORY_BROWSER_IMPEXP void CommandHistoryLaunch(void);

#ifdef __cplusplus
};
#endif

#endif /* __COMMANDHISTORY_WRAP_H__ */

