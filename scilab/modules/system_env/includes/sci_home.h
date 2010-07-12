/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SCI_HOME_H__
#define __SCI_HOME_H__

#include <wchar.h>
#include "dynlib_system_env.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C"
{
#endif

EXTERN_SYSTEM_ENV wchar_t *getSCIHOMEW(void);
EXTERN_SYSTEM_ENV char *getSCIHOME(void);

EXTERN_SYSTEM_ENV void setSCIHOMEW(const wchar_t* _sci_path);
EXTERN_SYSTEM_ENV void setSCIHOME(const char* _sci_path);

EXTERN_SYSTEM_ENV wchar_t* computeSCIHOMEW();
EXTERN_SYSTEM_ENV char* computeSCIHOME();

EXTERN_SYSTEM_ENV wchar_t* getenvSCIHOMEW(void);
EXTERN_SYSTEM_ENV char* getenvSCIHOME(void);

EXTERN_SYSTEM_ENV void putenvSCIHOMEW(const wchar_t* _sci_path);
EXTERN_SYSTEM_ENV void putenvSCIHOME(const char* _sci_path);

EXTERN_SYSTEM_ENV void defineSCIHOME();

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_HOME_H__
