/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __REFERENCE_MODULES_H__
#define __REFERENCE_MODULES_H__

#include "dynlib_ast.h"

EXTERN_AST int checkReferenceModule(const wchar_t* _module);
EXTERN_AST void addReferenceModule(const wchar_t* _module);
EXTERN_AST void removeReferenceModule(const wchar_t* _module);

#endif // ! __REFERENCE_MODULES_H__
