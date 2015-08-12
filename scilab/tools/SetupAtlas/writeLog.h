/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __WRITELOG_H__
#define __WRITELOG_H__
/*--------------------------------------------------------------------------*/
#include <windows.h>
#include <wchar.h>

/**
* append message to a log file
* @param[in] message
* @param[in] filename destination
* @return TRUE or FALSE
*/
BOOL AppendMessageToLog(wchar_t *Message, wchar_t *FullFilename);

/**
* get current date and time
*/
wchar_t *getTimeString(void);

#endif
/*--------------------------------------------------------------------------*/