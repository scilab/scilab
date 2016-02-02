/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef __SEMWINDOWS_H__
#define __SEMWINDOWS_H__

#include <Windows.h>

/* emulates unix semaphore on windows */

#define sem_t HANDLE

/* http://msdn.microsoft.com/en-us/library/ms682438(VS.85).aspx */
#define sem_init(__sem, __0, __1) *__sem = CreateSemaphore(NULL, 0, 100, NULL)

/* http://msdn.microsoft.com/en-us/library/ms687032(VS.85).aspx */
#define sem_wait(__sem) WaitForSingleObject(*__sem, INFINITE)

/* http://msdn.microsoft.com/en-us/library/ms685071(VS.85).aspx */
#define sem_post(__sem) ReleaseSemaphore(*__sem, 1, NULL)

#endif /* __SEMWINDOWS_H__ */
/*--------------------------------------------------------------------------*/
