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

#ifndef __SCI_TMPDIR_H__
#define __SCI_TMPDIR_H__

#include <wchar.h>

#include "dynlib_ast.h"

EXTERN_AST char* getTMPDIR(void);
EXTERN_AST void setTMPDIR(const char* _sci_tmpdir);
EXTERN_AST char* computeTMPDIR();
EXTERN_AST char* getenvTMPDIR(void);
EXTERN_AST void putenvTMPDIR(const char* _sci_tmpdir);

EXTERN_AST void defineTMPDIR();
EXTERN_AST void clearTMPDIR();

#endif // ! __SCI_TMPDIR_H__