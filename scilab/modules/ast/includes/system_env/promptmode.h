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

#ifndef __PROMPTMODE_H__
#define __PROMPTMODE_H__

#include "dynlib_ast.h"

#define PROMPTMODE_NORMAL   0   //show new values but not commands
#define PROMPTMODE_SILENT   -1  //hide all
#define PROMPTMODE_PROMPT   2   //show all
#define PROMPTMODE_EXEC     1   //show all
#define PROMPTMODE_EXEC3    3   //show all
#define PROMPTMODE_STEP     4   //later ...
#define PROMPTMODE_STEP7    7   //later ...

#define SILENT_ERROR        1
#define VERBOSE_ERROR       0

EXTERN_AST int getPromptMode(void);
EXTERN_AST void setPromptMode(int _iMode);
EXTERN_AST int isPromptShow(void);

EXTERN_AST int getSilentError(void);
EXTERN_AST void setSilentError(int _iSilent);

#endif // ! __PROMPTMODE_H__
