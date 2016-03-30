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
#ifndef __ISANAN_H__
#define __ISANAN_H__

#include "machine.h" /* C2F */

/**
* testing Nan returns 1 if a Nan is found and 0 elsewhere
* @param value to check
* @return 1 if a Nan is found and 0 elsewhere
*/
int C2F(isanan)(double *x);

#endif /* __ISANAN_H__ */
/*--------------------------------------------------------------------------*/
