/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Adeline CARNIS
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
#ifndef __PROCESS_DNEUPD_H__
#define __PROCESS_DNEUPD_H__
#include "elem_common.h"

/**
 * @TODO add comment
 *
 * @param DR
 * @param DI
 * @param Z
 * @param N
 * @param nev
 * @param AR
 * @param eigenvalue
 * @param eigenvector
 * @param sigma_imaginary
 * @param RVEC
 */

void process_dneupd_data(double* DR, double* DI, double* Z, int N, int nev, double* AR,
                         doublecomplex* eigenvalue, doublecomplex* eigenvector,
                         int sigma_imaginary);

#endif /* __PROCESS_DNEUPD_H__ */
/*--------------------------------------------------------------------------*/
