/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __SCI_SIMPMODE_H__
#define __SCI_SIMPMODE_H__

#include "dynlib_system_env.h"

#ifdef __cplusplus
extern "C"
{
#endif

    EXTERN_SYSTEM_ENV int getSimpMode(void);
    EXTERN_SYSTEM_ENV void setSimpMode(int);

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_SIMPMODE_H__
