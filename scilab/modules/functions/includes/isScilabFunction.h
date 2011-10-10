/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
