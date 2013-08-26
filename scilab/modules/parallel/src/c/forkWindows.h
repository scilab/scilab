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
#ifndef __FORK_WINDOWS_H__
#define __FORK_WINDOWS_H__

#include "BOOL.h"

/* http://technet.microsoft.com/en-us/library/bb497007.aspx */
/* http://undocumented.ntinternals.net/ */

/**
* simulate fork on Windows
*/
int fork(void);

/**
* check if symbols to simulate fork are present
* and load these symbols
*/
BOOL haveLoadedFunctionsForFork(void);

#endif /* __FORK_WINDOWS_H__ */
/*--------------------------------------------------------------------------*/
