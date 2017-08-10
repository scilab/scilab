/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997 - INRIA
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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

#ifndef __SCI_TOOLS_H__
#define __SCI_TOOLS_H__

#include "machine.h"
#include "dynlib_core.h"

#define DATA_TYPE_SCALAR     1

#define TYPE_DOUBLE  0
#define TYPE_COMPLEX 1

/* valid for Real or Complex matrix */
#define TYPE(ptr)   (((int*) (ptr))[-1])
#define NB_COL(ptr) (((int*) (ptr))[-2])
#define NB_ROW(ptr) (((int*) (ptr))[-3])
#define DATA_TYPE(ptr) (((int*) (ptr))[-4])

#define SET_DATA_TYPE_SCALAR(ptr)  (((int*) (ptr))[-4] = DATA_TYPE_SCALAR)
#define SET_TYPE_DOUBLE(ptr)  (((int*) (ptr))[-1] = TYPE_DOUBLE)
#define SET_TYPE_COMPLEX(ptr) (((int*) (ptr))[-1] = TYPE_COMPLEX)
#define SET_NB_COL(ptr,m) (((int*) (ptr))[-2] = (m))
#define SET_NB_ROW(ptr,n) (((int*) (ptr))[-3] = (n))

//CORE_IMPEXP void SciToF77(double *ptr, int size, int lda);
//CORE_IMPEXP void F77ToSci(double *ptr, int size, int lda);
//
//CORE_IMPEXP void double2z(double *ptr, double *ptr77z, int size, int lda);
//CORE_IMPEXP void z2double(double *ptrz, double *ptrsci, int size, int lda);

#endif /* __SCI_TOOLS_H__ */
