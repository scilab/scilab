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

#ifndef __SCI_PATH_H__
#define __SCI_PATH_H__

#include <wchar.h>
#include "dynlib_system_env.h"

#ifdef __cplusplus
extern "C"
{
#endif

EXTERN_SYSTEM_ENV wchar_t* getSCIW(void);
EXTERN_SYSTEM_ENV char* getSCI(void);

EXTERN_SYSTEM_ENV void setSCIW(const wchar_t* _sci_path);
EXTERN_SYSTEM_ENV void setSCI(const char* _sci_path);

EXTERN_SYSTEM_ENV wchar_t* computeSCIW();
EXTERN_SYSTEM_ENV char* computeSCI();

EXTERN_SYSTEM_ENV wchar_t* getenvSCIW(void);
EXTERN_SYSTEM_ENV char* getenvSCI(void);

EXTERN_SYSTEM_ENV void putenvSCIW(const wchar_t* _sci_path);
EXTERN_SYSTEM_ENV void putenvSCI(const char* _sci_path);

EXTERN_SYSTEM_ENV void defineSCI();

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_PATH_H__