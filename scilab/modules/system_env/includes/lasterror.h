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

#ifndef __SCI_LASTERROR_H__
#define __SCI_LASTERROR_H__

#include <wchar.h>
#include "dynlib_system_env.h"
#include "BOOL.h"

#ifdef __cplusplus
extern "C"
{
#endif

    EXTERN_SYSTEM_ENV int       getLastErrorNumber(void);
    EXTERN_SYSTEM_ENV wchar_t*  getLastErrorMessage(void);
    EXTERN_SYSTEM_ENV int       getLastErrorLine(void);
    EXTERN_SYSTEM_ENV wchar_t*  getLastErrorFunction(void);

    EXTERN_SYSTEM_ENV void      setLastErrorNumber(int _iError);
    EXTERN_SYSTEM_ENV void      setLastErrorMessage(const wchar_t* _pwstError);
    EXTERN_SYSTEM_ENV void      setLastErrorLine(int _iLine);
    EXTERN_SYSTEM_ENV void      setLastErrorFunction(const wchar_t* _pwstFunction);

    EXTERN_SYSTEM_ENV void      setLastError(int _iError, const wchar_t* _pwstError, int _iLine, const wchar_t* _pwstFunction);

    EXTERN_SYSTEM_ENV void      SetError();
    EXTERN_SYSTEM_ENV int       isError();
    EXTERN_SYSTEM_ENV void      resetError();

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_LASTERROR_H__
