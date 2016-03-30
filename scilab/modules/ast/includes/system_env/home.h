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

#ifndef __HOME_H__
#define __HOME_H__

#include <wchar.h>
#include "BOOL.h"

#include "dynlib_ast.h"

EXTERN_AST wchar_t *getHOMEW(void);
EXTERN_AST char *getHOME(void);

EXTERN_AST void setHOMEW(const wchar_t* _sci_path);
EXTERN_AST void setHOME(const char* _sci_path);

EXTERN_AST wchar_t* computeHOMEW();
EXTERN_AST char* computeHOME();

EXTERN_AST wchar_t* getenvHOMEW(void);
EXTERN_AST char* getenvHOME(void);

EXTERN_AST void putenvHOMEW(const wchar_t* _sci_path);
EXTERN_AST void putenvHOME(const char* _sci_path);

EXTERN_AST void defineHOME();

#endif // ! __HOME_H__
