/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - INRIA - Serge STEER
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
#include "machine.h"
#include "core_math.h"
#include "sci_malloc.h"
#include "conv2.h"
/*--------------------------------------------------------------------------*/
extern double C2F(ddot)(int *n, double *A, int *iA, double *B, int *iB);
/*--------------------------------------------------------------------------*/
void conv2_separable_R(double *R, int nR, double *C, int mC, double *A, int mA, int nA, double *Out, int mOut, int nOut, int edgM, int edgN, double *T)
{
    int ai = 0, tj = 0, ci = 0, rj = 0; /*current index over A,T,C and R */
    int i = 0, j = 0; /* loop variables*/
    int l = 0;
    int one = 1, minusone = -1;

    for (i = 0; i < mOut; i++ )
    {
        /*Compute  the 1-D conv A(i,:) and C  in T */
        ai = Max(0, i - edgM) ;
        ci = mC - 1 - Max(0, edgM - i);
        l = Min(ci + 1, mA - ai);
        for (j = 0; j < nA; j++ )
        {
            T[j] = C2F(ddot)(&l, A + ai + mA * j, &one, C + ci - l + 1, &minusone);
        }
        /*1-D convolution of T and  R */
        for (j = 0; j < nOut; j++ )
        {
            rj = nR - 1 - Max(0, edgN - j);
            tj = Max(0, j - edgN) ;
            l = Min(rj + 1, nA - tj);
            Out[i + j * mOut] = C2F(ddot)(&l, T + tj, &one, R + rj - l + 1, &minusone);
        }
    }
}
/*--------------------------------------------------------------------------*/
void conv2_separable_C(double *Rr, double *Ri, int nR, double *Cr, double *Ci, int mC, double *Ar, double *Ai, int mA, int nA, double *Outr, double *Outi, int mOut, int nOut, int edgM, int edgN, double *Tr, double *Ti)
{
    int ai = 0, tj = 0, ci = 0, rj = 0; /*current index over A,T,C and R */
    int i = 0, j = 0; /* loop variables*/
    int l = 0;
    int one = 1, minusone = -1;

    for (i = 0; i < mOut; i++ )
    {
        /*Compute  the 1-D conv A(i,:) and C  in T */
        ai = Max(0, i - edgM) ;
        ci = mC - 1 - Max(0, edgM - i);
        l = Min(ci + 1, mA - ai);
        if (Ai != NULL && Ci != NULL)
        {
            for (j = 0; j < nA; j++ )
            {
                Tr[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Cr + ci - l + 1, &minusone) -
                        C2F(ddot)(&l, Ai + ai + mA * j, &one, Ci + ci - l + 1, &minusone);

                Ti[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Ci + ci - l + 1, &minusone) +
                        C2F(ddot)(&l, Ai + ai + mA * j, &one, Cr + ci - l + 1, &minusone);
            }
        }
        else if (Ci != NULL)
        {
            for (j = 0; j < nA; j++ )
            {
                Tr[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Cr + ci - l + 1, &minusone);
                Ti[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Ci + ci - l + 1, &minusone);
            }
        }
        else if (Ai != NULL)
        {
            for (j = 0; j < nA; j++ )
            {
                Tr[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Cr + ci - l + 1, &minusone);
                Ti[j] = C2F(ddot)(&l, Ai + ai + mA * j, &one, Cr + ci - l + 1, &minusone);
            }
        }
        else
        {
            for (j = 0; j < nA; j++ )
            {
                Tr[j] = C2F(ddot)(&l, Ar + ai + mA * j, &one, Cr + ci - l + 1, &minusone);
                Ti[j] = 0.0;
            }
        }
        /*1-D convolution of T and  R */
        for (j = 0; j < nOut; j++ )
        {
            rj = nR - 1 - Max(0, edgN - j);
            tj = Max(0, j - edgN) ;
            l = Min(rj + 1, nA - tj);
            Outr[i + j * mOut] = C2F(ddot)(&l, Tr + tj, &one, Rr + rj - l + 1, &minusone);
            Outi[i + j * mOut] = C2F(ddot)(&l, Ti + tj, &one, Rr + rj - l + 1, &minusone);
            if (Ri != NULL)
            {
                Outr[i + j * mOut] -= C2F(ddot)(&l, Ti + tj, &one, Ri + rj - l + 1, &minusone);
                Outi[i + j * mOut] = C2F(ddot)(&l, Tr + tj, &one, Ri + rj - l + 1, &minusone);
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
void conv2_R(double *A, int mA, int nA, double *B, int mB, int nB, double *Out, int mOut, int nOut, int edgM, int edgN)
{
    int ai = 0, aj = 0, bi = 0, bj = 0; /*current index over A and B */
    int i = 0, j = 0; /* loop variables*/
    int l = 0;
    int one = 1, minusone = -1;
    double sum = 0;
    if (nOut == 1)
    {
        /* A and B are column vectors nA=nB=nOut=1 */
        for (i = 0; i < mOut; i++ )
        {
            bi = mB - 1 - Max(0, edgM - i);
            ai = Max(0, i - edgM);
            l = Min(bi + 1, mA - ai);
            Out[i] = C2F(ddot)(&l, A + ai, &one, B + bi - l + 1, &minusone);
        }
    }
    else if (mOut == 1)
    {
        /* A and B are row vectors mA=mB=mOut=1 */
        for (j = 0; j < nOut; j++ )
        {
            bj = nB - 1 - Max(0, edgN - j);
            aj = Max(0, j - edgN);
            l = Min(bj + 1, nA - aj);
            Out[j] = C2F(ddot)(&l, A + aj, &one, B + bj - l + 1, &minusone);
        }
    }
    else
    {
        /* general array case */
        for (i = 0; i < mOut; i++ )
        {
            bi = mB - 1 - Max(0, edgM - i);
            ai = Max(0, i - edgM);
            for (j = 0; j < nOut; j++ )
            {
                sum = 0;
                for (bj = nB - 1 - Max(0, edgN - j), aj = Max(0, j - edgN); bj >= 0 && aj < nA; bj--, aj++)
                {
                    l = Min(bi + 1, mA - ai);
                    sum += C2F(ddot)(&l, A + ai + mA * aj, &one, B + bi - l + 1 + mB * bj, &minusone);
                }
                Out[i + j * mOut] = sum;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
void conv2_C(double *Ar, double *Ai, int mA, int nA, double *Br, double *Bi, int mB, int nB, double *Outr, double *Outi, int mOut, int nOut, int edgM, int edgN)
{
    int ai = 0, aj = 0, bi = 0, bj = 0; /*current index over A and B */
    int i = 0, j = 0; /* loop variables*/
    int l = 0;
    int one = 1, minusone = -1;
    double sumr = 0, sumi = 0;

    if (Bi != NULL && Ai != NULL)
    {
        if (nOut == 1)
        {
            /* fastest code */
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                l = Min(bi + 1, mA - ai);
                Outr[i] = C2F(ddot)(&l, Ar + ai, &one, Br + bi - l + 1, &minusone) -
                          C2F(ddot)(&l, Ai + ai, &one, Bi + bi - l + 1, &minusone);

                Outi[i] = C2F(ddot)(&l, Ar + ai, &one, Bi + bi - l + 1, &minusone) +
                          C2F(ddot)(&l, Ai + ai, &one, Br + bi - l + 1, &minusone);
            }
        }
        else
        {
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                for (j = 0; j < nOut; j++ )
                {
                    sumr = 0;
                    sumi = 0;
                    for (bj = nB - 1 - Max(0, edgN - j), aj = Max(0, j - edgN); bj >= 0 && aj < nA; bj--, aj++)
                    {
                        l = Min(bi + 1, mA - ai);
                        sumr += C2F(ddot)(&l, Ar + ai + mA * aj, &one, Br + bi - l + 1 + mB * bj, &minusone)
                                - C2F(ddot)(&l, Ai + ai + mA * aj, &one, Bi + bi - l + 1 + mB * bj, &minusone);

                        sumi += C2F(ddot)(&l, Ar + ai + mA * aj, &one, Bi + bi - l + 1 + mB * bj, &minusone) +
                                C2F(ddot)(&l, Ai + ai + mA * aj, &one, Br + bi - l + 1 + mB * bj, &minusone);
                    }
                    Outr[i + j * mOut] = sumr;
                    Outi[i + j * mOut] = sumi;
                }
            }
        }
    }
    else if (Ai != NULL)
    {
        if (nOut == 1)
        {
            /* fastest code */
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                l = Min(bi + 1, mA - ai);
                Outr[i] = C2F(ddot)(&l, Ar + ai, &one, Br + bi - l + 1, &minusone);
                Outi[i] = C2F(ddot)(&l, Ai + ai, &one, Br + bi - l + 1, &minusone);
            }
        }
        else
        {
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                for (j = 0; j < nOut; j++ )
                {
                    sumr = 0;
                    sumi = 0;
                    for (bj = nB - 1 - Max(0, edgN - j), aj = Max(0, j - edgN); bj >= 0 && aj < nA; bj--, aj++)
                    {
                        l = Min(bi + 1, mA - ai);
                        sumr += C2F(ddot)(&l, Ar + ai + mA * aj, &one, Br + bi - l + 1 + mB * bj, &minusone);
                        sumi += C2F(ddot)(&l, Ai + ai + mA * aj, &one, Br + bi - l + 1 + mB * bj, &minusone);
                    }
                    Outr[i + j * mOut] = sumr;
                    Outi[i + j * mOut] = sumi;
                }
            }
        }
    }
    else if (Bi != NULL)
    {
        if (nOut == 1)
        {
            /* fastest code */
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                l = Min(bi + 1, mA - ai);
                Outr[i] = C2F(ddot)(&l, Ar + ai, &one, Br + bi - l + 1, &minusone);
                Outi[i] = C2F(ddot)(&l, Ar + ai, &one, Bi + bi - l + 1, &minusone);
            }
        }
        else
        {
            for (i = 0; i < mOut; i++ )
            {
                bi = mB - 1 - Max(0, edgM - i);
                ai = Max(0, i - edgM);
                for (j = 0; j < nOut; j++ )
                {
                    sumr = 0;
                    sumi = 0;
                    for (bj = nB - 1 - Max(0, edgN - j), aj = Max(0, j - edgN); bj >= 0 && aj < nA; bj--, aj++)
                    {
                        l = Min(bi + 1, mA - ai);
                        sumr += C2F(ddot)(&l, Ar + ai + mA * aj, &one, Br + bi - l + 1 + mB * bj, &minusone);
                        sumi += C2F(ddot)(&l, Ar + ai + mA * aj, &one, Bi + bi - l + 1 + mB * bj, &minusone);
                    }
                    Outr[i + j * mOut] = sumr;
                    Outi[i + j * mOut] = sumi;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
