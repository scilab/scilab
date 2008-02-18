
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

#ifndef __ERROR_INTERNAL_H__
#define __ERROR_INTERNAL_H__

#define ERROR_FROM_FORTRAN 0
#define ERROR_FROM_C 1

#include "machine.h"

/**
* error_internal
* @param [in] n : error code
* @param [in] buffer : string error
* @param [in] mode : ERROR_FROM_FORTRAN (not use buffer) or ERROR_FROM_C
* @return 0
*/
int error_internal(integer *n,char *buffer,int mode);

#endif /* __ERROR_INTERNAL_H__ */
/*--------------------------------------------------------------------------*/ 
