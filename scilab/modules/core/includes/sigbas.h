/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
