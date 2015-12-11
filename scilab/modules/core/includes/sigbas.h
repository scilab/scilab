/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#ifndef __SIGBAS_H__
#define __SIGBAS_H__

#include "machine.h"

/**
* dealing with signals inside Scilab
* @param[in] sig : a number signal
* @return 0
* sig == 2 ???
* sig == 8 Floating point exception
* sig == 11 fatal error!!!
*/
int C2F(sigbas)(int *sig);

#endif /* __SIGBAS_H__  */
/*--------------------------------------------------------------------------*/
