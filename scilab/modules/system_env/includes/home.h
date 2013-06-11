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

#ifndef __HOME_H__
#define __HOME_H__

#include <wchar.h>
#include "dynlib_system_env.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C"
{
#endif

    EXTERN_SYSTEM_ENV wchar_t *getHOMEW(void);
    EXTERN_SYSTEM_ENV char *getHOME(void);

    EXTERN_SYSTEM_ENV void setHOMEW(const wchar_t* _sci_path);
    EXTERN_SYSTEM_ENV void setHOME(const char* _sci_path);

    EXTERN_SYSTEM_ENV wchar_t* computeHOMEW();
    EXTERN_SYSTEM_ENV char* computeHOME();

    EXTERN_SYSTEM_ENV wchar_t* getenvHOMEW(void);
    EXTERN_SYSTEM_ENV char* getenvHOME(void);

    EXTERN_SYSTEM_ENV void putenvHOMEW(const wchar_t* _sci_path);
    EXTERN_SYSTEM_ENV void putenvHOME(const char* _sci_path);

    EXTERN_SYSTEM_ENV void defineHOME();

#ifdef __cplusplus
}
#endif

#endif // ! __HOME_H__
