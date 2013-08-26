/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/

#ifndef __ENVTCLTK_H__
#define __ENVTCLTK_H__

#ifdef _MSC_VER

#include "BOOL.h"

/**
* Set TCL_LIBRARY,TK_LIBRARY, ... environment variables on Windows
* @param[in] scilab root path
* @return TRUE if environment variables are correct
*/
BOOL SetTclTkEnvironment(char *DefaultPath);
#endif

#endif /* __ENVTCLTK_H__ */
/*--------------------------------------------------------------------------*/
