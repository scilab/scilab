/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
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
CONSOLE_IMPEXP int ConsolePrintf(const char *line);
/*--------------------------------------------------------------------------*/
#endif /* __CONSOLEPRINTF_H__ */

