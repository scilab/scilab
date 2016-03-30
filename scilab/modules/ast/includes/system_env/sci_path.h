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

#ifndef __SCI_PATH_H__
#define __SCI_PATH_H__

#include <wchar.h>

#include "dynlib_ast.h"

EXTERN_AST wchar_t* getSCIW(void);
EXTERN_AST char* getSCI(void);

EXTERN_AST void setSCIW(const wchar_t* _sci_path);
EXTERN_AST void setSCI(const char* _sci_path);

EXTERN_AST wchar_t* computeSCIW();
EXTERN_AST char* computeSCI();

EXTERN_AST wchar_t* getenvSCIW(void);
EXTERN_AST char* getenvSCI(void);

EXTERN_AST void putenvSCIW(const wchar_t* _sci_path);
EXTERN_AST void putenvSCI(const char* _sci_path);

EXTERN_AST void defineSCI();

#endif // ! __SCI_PATH_H__