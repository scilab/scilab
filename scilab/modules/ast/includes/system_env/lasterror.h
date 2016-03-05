/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#ifndef __SCI_LASTERROR_H__
#define __SCI_LASTERROR_H__

#include <wchar.h>
#include "BOOL.h"
#include "dynlib_ast.h"

EXTERN_AST int getLastErrorNumber(void);
EXTERN_AST wchar_t* getLastErrorMessage(void);
EXTERN_AST int getLastErrorLine(void);
EXTERN_AST wchar_t* getLastErrorFunction(void);

EXTERN_AST void setLastErrorNumber(int _iError);
EXTERN_AST void setLastErrorMessage(const wchar_t* _pwstError);
EXTERN_AST void setLastErrorLine(int _iLine);
EXTERN_AST void setLastErrorFunction(const wchar_t* _pwstFunction);

EXTERN_AST void setLastError(int _iError, const wchar_t* _pwstError, int _iLine, const wchar_t* _pwstFunction);

EXTERN_AST void SetError();
EXTERN_AST int isError();
EXTERN_AST void resetError();

#endif // ! __SCI_LASTERROR_H__
