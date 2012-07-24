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

#ifndef __COMMANDHISTORY_WRAP_H__
#define __COMMANDHISTORY_WRAP_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "dynlib_history_browser.h"

    HISTORY_BROWSER_IMPEXP void CommandHistoryAppendLine (char * lineToAppend);

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

