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

#ifndef __SCI_HOME_H__
#define __SCI_HOME_H__

#include <wchar.h>
#include "BOOL.h"

#include "dynlib_ast.h"

EXTERN_AST wchar_t *getSCIHOMEW(void);
EXTERN_AST char *getSCIHOME(void);

EXTERN_AST void setSCIHOMEW(const wchar_t* _sci_path);
EXTERN_AST void setSCIHOME(const char* _sci_path);

EXTERN_AST wchar_t* computeSCIHOMEW();
EXTERN_AST char* computeSCIHOME();

EXTERN_AST wchar_t* getenvSCIHOMEW(void);
EXTERN_AST char* getenvSCIHOME(void);

EXTERN_AST void putenvSCIHOMEW(const wchar_t* _sci_path);
EXTERN_AST void putenvSCIHOME(const char* _sci_path);

EXTERN_AST void defineSCIHOME();

#endif // ! __SCI_HOME_H__
