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

#ifndef __SCI_IEEE_H__
#define __SCI_IEEE_H__

#include "dynlib_system_env.h"

#ifdef __cplusplus
extern "C"
{
#endif

    EXTERN_SYSTEM_ENV int getieee(void);
    EXTERN_SYSTEM_ENV void setieee(int);

#ifdef __cplusplus
}
#endif

#endif // ! __SCI_IEEE_H__
