/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h> /* printf */
#include <string.h>
#include "machine.h"
#ifdef UNIX
#include <unistd.h>
#include <sys/socket.h>
#endif
#include <math.h>
#include <stdlib.h>
#include "dmmul.h"
/*--------------------------------------------------------------------------*/
/* Table of constant values */
static double c_b4 = 1.;
static double c_b5 = 0.;
/*--------------------------------------------------------------------------*/
typedef int integer;
typedef double doublereal;
extern int C2F(dgemm)(char *transa, char *transb, integer *m, integer *
                      n, integer *k, doublereal *alpha, doublereal *a, integer *lda,
                      doublereal *b, integer *ldb, doublereal *beta, doublereal *c, integer
                      *ldc); /* blas routine */
/*--------------------------------------------------------------------------*/
int dmmul(double *a, int *na, double *b, int *nb, double *c__,
          int *nc, int *l, int *m, int *n)
{
    int a_dim1 = 0, a_offset = 0, b_dim1 = 0, b_offset = 0, c_dim1 = 0, c_offset = 0;

    /*     PURPOSE */
    /*        computes the matrix product C = A * B */
    /*            C   =   A   *   B */
    /*          (l,n)   (l,m) * (m,n) */

    /*     PARAMETERS */
    /*        input */
    /*        ----- */
    /*        A : (double) array (l, m) with leading dim na */

    /*        B : (double) array (m, n) with leading dim nb */

    /*        na, nb, nc, l, m, n : integers */

    /*        output */
    /*        ------ */
    /*        C : (double) array (l, n) with leading dim nc */

    /*     NOTE */
    /*        (original version substituted by a call to the blas dgemm) */
    /* Parameter adjustments */
    a_dim1 = *na;
    a_offset = 1 + a_dim1 * 1;
    a -= a_offset;
    c_dim1 = *nc;
    c_offset = 1 + c_dim1 * 1;
    c__ -= c_offset;
    b_dim1 = *nb;
    b_offset = 1 + b_dim1 * 1;
    b -= b_offset;

    /* Function Body */
    C2F(dgemm)("n", "n", l, n, m, &c_b4, &a[a_offset], na, &b[b_offset], nb, &
               c_b5, &c__[c_offset], nc);
    return 0;
} /* dmmul */
/*--------------------------------------------------------------------------*/
