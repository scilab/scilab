/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __GETBLASTYPE_H__
#define __GETBLASTYPE_H__

#include "dynlib_scilab_windows.h"

typedef enum { BLAS_UNKNOW = -1,
               BLAS_REF = 0,
               BLAS_ATLAS = 1,
               BLAS_MKL = 2
             } blas_type;

/* detect blas type used by scilab */
SCILAB_WINDOWS_IMPEXP blas_type getBlasType(void);

#endif /* __GETBLASTYPE_H__ */
/*--------------------------------------------------------------------------*/
