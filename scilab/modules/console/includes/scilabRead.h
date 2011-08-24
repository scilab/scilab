/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILAB_READ_H__
#define __SCILAB_READ_H__

#include "dynlib_console.h"

typedef char* (*SCILAB_INPUT_METHOD)(void);

CONSOLE_IMPEXP void setScilabInputMethod(SCILAB_INPUT_METHOD reader);
CONSOLE_IMPEXP char* scilabRead();

#endif /* !__SCILAB_READ_H__ */
