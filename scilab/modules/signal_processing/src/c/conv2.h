/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - INRIA - Serge Steer
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*------------------------------------------------------------------------*/
/* file: conv2.h                                                          */
/* desc : Functions to compute 2-D convolutions                           */
/*------------------------------------------------------------------------*/
#ifndef __CONV2_H__
#define __CONV2_H__


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
void conv2_separable_R(double *R, int nR,
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
void conv2_separable_C(double *Rr, double *Ri, int nR,
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
void conv2_R(double *A, int mA, int nA,
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
void conv2_C(double *Ar, double *Ai, int mA, int nA,
             double *Br, double *Bi, int mB, int nB,
             double *Outr, double *Outi, int mOut, int nOut,
             int edgM, int edgN);
#endif /* __CONV2_H__ */

