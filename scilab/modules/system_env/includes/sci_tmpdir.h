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

#ifndef __SCI_TMPDIR_H__
#define __SCI_TMPDIR_H__

#include <wchar.h>
#include "dynlib_system_env.h"

#ifdef __cplusplus
extern "C"
{
#endif
EXTERN_SYSTEM_ENV char* getTMPDIR(void);

EXTERN_SYSTEM_ENV wchar_t *getTMPDIRW(void);

EXTERN_SYSTEM_ENV void setTMPDIR(const char* _sci_tmpdir);

EXTERN_SYSTEM_ENV char* computeTMPDIR();

EXTERN_SYSTEM_ENV char* getenvTMPDIR(void);

EXTERN_SYSTEM_ENV void putenvTMPDIR(const char* _sci_tmpdir);

EXTERN_SYSTEM_ENV void defineTMPDIR();

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_TMPDIR_H__