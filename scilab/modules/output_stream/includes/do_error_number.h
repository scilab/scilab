
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
