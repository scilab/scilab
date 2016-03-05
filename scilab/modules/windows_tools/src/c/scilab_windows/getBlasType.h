/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
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
