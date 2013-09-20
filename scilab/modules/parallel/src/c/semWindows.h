/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
