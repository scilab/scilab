/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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
