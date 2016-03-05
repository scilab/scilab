/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#ifndef ___ISSCILABFUNCTION_H__
#define ___ISSCILABFUNCTION_H__

#include "dynlib_functions.h"
#include "BOOL.h"

/**
* check if a function name already exists in Scilab
* function eqs builtin or macro in library or macro as variable
* @param[in] function name
* @return TRUE (exists) or FALSE (Not exists)
*/
FUNCTIONS_SCILAB_IMPEXP BOOL isScilabFunction(const char * functionName);

/**
* check if a function name already exists in Scilab
* as macro in a library
* @param[in] function name
* @return TRUE (exists) or FALSE (Not exists)
*/
FUNCTIONS_SCILAB_IMPEXP BOOL isScilabMacro(const char * functionName);

/**
* check if a function name already exists in Scilab
* as macro variable
* @param[in] function name
* @return TRUE (exists) or FALSE (Not exists)
*/
FUNCTIONS_SCILAB_IMPEXP BOOL isScilabMacroVariable(const char * functionName);

/**
* check if a function name already exists in Scilab
* as builtin
* @param[in] function name
* @return TRUE (exists) or FALSE (Not exists)
*/
FUNCTIONS_SCILAB_IMPEXP BOOL isScilabBuiltIn(const char * functionName);

#endif /* ___ISSCILABFUNCTION_H__ */
/*--------------------------------------------------------------------------*/
