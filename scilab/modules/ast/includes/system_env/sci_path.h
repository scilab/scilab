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

#ifndef __SCI_PATH_H__
#define __SCI_PATH_H__

#include <wchar.h>

#include "dynlib_ast.h"

EXTERN_AST char* getSCI(void);
EXTERN_AST void setSCI(const char* _sci_path);
EXTERN_AST char* computeSCI();
EXTERN_AST char* getenvSCI(void);
EXTERN_AST void putenvSCI(const char* _sci_path);

EXTERN_AST void defineSCI();

#endif // ! __SCI_PATH_H__