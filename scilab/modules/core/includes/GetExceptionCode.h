/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __GETEXCEPTIONCODE__
#define __GETEXCEPTIONCODE__

/* Only for Windows */

#include <Windows.h>
#include "MALLOC.h"

char *GetExceptionString(DWORD ExceptionCode);

#endif /* __GETEXCEPTIONCODE__ */
