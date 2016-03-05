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

#ifndef __SCI_TMPDIR_H__
#define __SCI_TMPDIR_H__

#include <wchar.h>

#include "dynlib_ast.h"

EXTERN_AST wchar_t* getTMPDIRW(void);
EXTERN_AST char* getTMPDIR(void);

EXTERN_AST void setTMPDIRW(const wchar_t* _sci_tmpdir);
EXTERN_AST void setTMPDIR(const char* _sci_tmpdir);

EXTERN_AST wchar_t* computeTMPDIRW();
EXTERN_AST char* computeTMPDIR();

EXTERN_AST wchar_t* getenvTMPDIRW(void);
EXTERN_AST char* getenvTMPDIR(void);

EXTERN_AST void putenvTMPDIRW(const wchar_t* _sci_tmpdir);
EXTERN_AST void putenvTMPDIR(const char* _sci_tmpdir);

EXTERN_AST void defineTMPDIR();
EXTERN_AST void clearTMPDIR();

#endif // ! __SCI_TMPDIR_H__