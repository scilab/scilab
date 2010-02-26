/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CONSOLEPRINTF_H__
#define __CONSOLEPRINTF_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_console.h"
/*--------------------------------------------------------------------------*/
/**
* ConsolePrintf function
* @param a string to display
* @return total number of characters written is returned.
*/
CONSOLE_IMPEXP int ConsolePrintf(char *line);
/*--------------------------------------------------------------------------*/
#endif /* __CONSOLEPRINTF_H__ */

