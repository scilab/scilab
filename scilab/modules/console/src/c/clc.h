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
#ifndef __CLC_H__
#define __CLC_H__
/*--------------------------------------------------------------------------*/
#include "BOOL.h"
#include "dynlib_console.h"
/*--------------------------------------------------------------------------*/
/**
* clc function
* @param nblines numbers of lines -1 --> all
* @return a boolean
*/
CONSOLE_IMPEXP BOOL clc(int nblines);
/*--------------------------------------------------------------------------*/
#endif /* __CLC_H__ */

