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

#ifndef __SCI_MODE_H__
#define __SCI_MODE_H__

#include "dynlib_ast.h"

typedef enum
{
    SCILAB_API = 1, /* Scilab is launch as an API */
    SCILAB_STD, /* The standard Scilab (gui, plot ...) */
    SCILAB_NW, /* Scilab in command line with the plots */
    SCILAB_NWNI /* Scilab in command line without any graphics. What not mandataroy here */
} scilabMode;

EXTERN_AST scilabMode getScilabMode(void);
EXTERN_AST void setScilabMode(scilabMode newmode);
EXTERN_AST const char * getScilabModeString(void);

#endif /* ! __SCI_MODE_H__ */
