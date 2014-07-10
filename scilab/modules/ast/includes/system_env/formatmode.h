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

#ifndef __FORMATMODE_H__
#define __FORMATMODE_H__

#include "dynlib_ast.h"

EXTERN_AST void setFormatSize(int _iFormatSize);
EXTERN_AST int getFormatSize(void);
EXTERN_AST void setFormatMode(int _iFormatMode);
EXTERN_AST int getFormatMode(void);
EXTERN_AST void setConsoleWidth(int _iConsoleWidth);
EXTERN_AST int getConsoleWidth(void);
EXTERN_AST void setConsoleLines(int _iConsoleLines);
EXTERN_AST int getConsoleLines(void);

#endif /* !__FORMATMODE_H__ */