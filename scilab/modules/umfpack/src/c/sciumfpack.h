/*
 *   Copyright Bruno Pinçon, ESIAL-IECN, Inria CORIDA project
 *   <bruno.pincon@iecn.u-nancy.fr>
 *   contributor:  Antonio Manoel Ferreria Frasson, Universidade Federal do
 *                 Espírito Santo, Brazil. <frasson@ele.ufes.br>.
 *
 * PURPOSE: Scilab interfaces routines onto the UMFPACK sparse solver
 * (Tim Davis) and onto the TAUCS snmf choleski solver (Sivan Teledo)
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
#include "machine.h"
#ifdef UMFPACK_SUITESPARSE
#include <suitesparse/umfpack.h>
#else
#include <umfpack.h>
#endif

enum {NOT_ENOUGH_PLACE_IN_STK, MAT_IS_NOT_SPD, A_PRIORI_OK};   /* flags for spd_sci_sparse_to_taucs_sparse */

typedef void * Adr;

typedef struct _CellAdr CellAdr ;
struct _CellAdr
{
    Adr adr;
    int it;   // added to see if the LU factors comes from a real or complex matrix
    CellAdr *next;
};

typedef struct  /* a type to handle a choleski factorization */
{
    int * p;      /* for the permutation    */
    void * C;     /* for the factor (lower) */
    int n;        /* to stay the order (in place to read a member of C) */
} taucs_handle_factors;

typedef struct
{
    int m;          /* number of rows */
    int n;          /* number of columns */
    int nel;        /* number of non nuls elements */
    int it;         /* flag type : it=-1 (boolean) it=0 (real) it=1 (complex) */
    int *p;         /* n+1 array : ptr_col[n] must be equal to nel */
    int *irow;
    double *R;
    double *I;
} CcsSparse;
