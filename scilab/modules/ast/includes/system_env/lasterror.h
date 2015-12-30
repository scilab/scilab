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

#ifndef __SCI_LASTERROR_H__
#define __SCI_LASTERROR_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_ast.h"

EXTERN_AST int getLastErrorNumber(void);
EXTERN_AST char* getLastErrorMessage(void);
EXTERN_AST int getLastErrorLine(void);
EXTERN_AST char* getLastErrorFunction(void);

EXTERN_AST void setLastErrorNumber(int _iError);
EXTERN_AST void setLastErrorMessage(const char* _pstError);
EXTERN_AST void setLastErrorLine(int _iLine);
EXTERN_AST void setLastErrorFunction(const char* _pstFunction);

EXTERN_AST void setLastError(int _iError, const char* _pstError, int _iLine, const char* _pstFunction);

EXTERN_AST void SetError();
EXTERN_AST int isError();
EXTERN_AST void resetError();

#endif // ! __SCI_LASTERROR_H__
