
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#ifndef __ERROR_H__
#define __ERROR_H__

#include "machine.h" /* C2F */

/* routine used by fortran */

/**
* error display and handling
* @param[in]  n : error number
* @return 0
*/
int C2F(error)(int *n);


/**
* error display and handling
* as Scierror uses default message associated to 'n'
* @param n error code
*/
void SciError(int n);

/**
* Stroe error number
* @param n error code
*/
void SciStoreError(int n);

#endif /* __ERROR_H__ */
/*--------------------------------------------------------------------------*/
