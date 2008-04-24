/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
/*--------------------------------------------------------------------------*/

#ifndef __ENVTCLTK_H__
#define __ENVTCLTK_H__

#ifdef _MSC_VER

#include "BOOL.h"

/**
* Set TCL_LIBRARY environment variable on Windows
* @param[in] scilab root path
* @return TRUE if environment variable is correct
*/
BOOL Set_TCL_LIBRARY_PATH(char *DefaultPath);

/**
* Set TK_LIBRARY environment variable on Windows
* @param[in] scilab root path
* @return TRUE if environment variable is correct
*/
BOOL Set_TK_LIBRARY_PATH(char *DefaultPath);

#endif

#endif /* __ENVTCLTK_H__ */
/*--------------------------------------------------------------------------*/
