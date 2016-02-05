/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - INRIA - Serge Steer
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
/*------------------------------------------------------------------------*/
/* file: conv2.h                                                          */
/* desc : Functions to compute 2-D convolutions                           */
/*------------------------------------------------------------------------*/

#ifndef __CONV2_H__
#define __CONV2_H__
#include "dynlib_signal_processing.h"

/**
 * computes 2-D convolution for separable kernel in real case
 * R is a real vector of length nR
 * C is a real vector of length nC
 * A is a (mA,nA) real matrix
 * Out is a (mOut,nOut) real matrix
 * edgM is an integer
 * edgN is an integer
 * T is a real vector of length nA
 */
SIGNAL_PROCESSING_IMPEXP void conv2_separable_R(double *R, int nR,
        double *C, int nC,
        double *A, int mA, int nA,
        double *Out, int mOut, int nOut,
        int edgM, int edgN, double *T);

/**
 * computes 2-D convolution for separable kernel in complex case
 * Rr and Ri are real vectors of length nR
 * Cr and Ci are real vectors of length nC
 * Ar and Ai are (mA,nA) real matrices
 * Outr and Outii are  (mOut,nOut) real matrices
 * edgM is an integer
 * edgN is an integer
 * Tr and Ti are real vectors of length nA
 */
SIGNAL_PROCESSING_IMPEXP void conv2_separable_C(double *Rr, double *Ri, int nR,
        double *Cr, double *Ci, int nC,
        double *Ar, double *Ai, int mA, int nA,
        double *Outr, double *Outi, int mOut, int nOut,
        int edgM, int edgN, double *Tr, double *Ti);

/**
 * computes 2-D convolution for matrix kernel in real case
 * A is a (mA,nA) real matrix
 * B is a (mB,nB) real matrix
 * Out is a (mOut,nOut) real matrix
 * edgM is an integer
 * edgN is an integer
 */
SIGNAL_PROCESSING_IMPEXP void conv2_R(double *A, int mA, int nA,
                                      double *B, int mB, int nB,
                                      double *Out, int mOut, int nOut,
                                      int edgM, int edgN);

/**
 * computes 2-D convolution for matrix kernel in complex case
 * Ar and Ai are (mA,nA) real matrices
 * Br and Bi are (mB,nB) real matrices
 * Outr and Outi are (mOut,nOut) real matrices
 * edgM is an integer
 * edgN is an integer
 */
SIGNAL_PROCESSING_IMPEXP void conv2_C(double *Ar, double *Ai, int mA, int nA,
                                      double *Br, double *Bi, int mB, int nB,
                                      double *Outr, double *Outi, int mOut, int nOut,
                                      int edgM, int edgN);
#endif /* __CONV2_H__ */

