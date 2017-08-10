/*
* See Copyright below
* Copyright (c) 1992, 1993
* The Regents of the University of California.  All rights reserved.
*/

#include <stdlib.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "qsort.h"
#include "qsort-int.h"
#include "qsort-short.h"
#include "qsort-char.h"
#include "qsort-double.h"
#include "qsort-string.h"
#include "core_math.h"
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*	$NetBSD: qsort.c,v 1.5 1995/12/28 08:52:36 thorpej Exp $	*/
/*-
* Copyright (c) 1992, 1993
*	The Regents of the University of California.  All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. All advertising materials mentioning features or use of this software
*    must display the following acknowledgement:
*	This product includes software developed by the University of
*	California, Berkeley and its contributors.
* 4. Neither the name of the University nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
*  Modified for Scilab Jean-Philippe Chancelier  to keep a permutation index
*  Modified for Scilab by Serge Steer to make it stable when permutation index is computed.
*/
/*--------------------------------------------------------------------------*/
/*
* Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
* Software---Practice and Experience, 23(11):1249-1265
*/
/*--------------------------------------------------------------------------*/
void sciqsort(char *a, char *tab, int flag, int n, int es, int es1, int (*cmp)(), int (*swapcode)(), int (*lswapcodeind)())
{
    char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
    char *taba, *tabb, *tabc, *tabd, *tabl, *tabm, *tabn;
    int d, dind, r, r1;

loop:
    if (n < 7)   /* Insertion sort on smallest arrays */
    {
        for (pm = a + es, tabm = tab + es1 ; pm < (char *) a + n * es; pm += es, tabm += es1 )
        {
            for (pl = pm, tabl = tabm ; pl > (char *) a && cmp(pl - es, pl, tabl - es1, tabl, flag) > 0;  pl -= es, tabl -= es1)
            {
                swapind(tabl, tabl - es1);
                swap(pl, pl - es);
            }
        }

        return;
    }

    /*Determine the pivot */
    pm = a + (n / 2) * es;/* Small arrays, middle element */
    tabm = tab + (n / 2) * es1 ;

    pn = a + (n - 1) * es;
    tabn = tab + (n - 1) * es1;

    if (n > 7)
    {
        pl = a;
        tabl = tab;
        if (n > 40)  /* Big arrays, pseudomedian of 9 */
        {
            dind = (n / 8) * es1;
            d =   (n / 8) * es;
            med3(pl, tabl, pl, pl + d, pl + 2 * d, tabl, tabl + dind, tabl + 2 * dind, cmp);
            med3(pm, tabm, pm - d, pm, pm + d, tabm - dind, tabm, tabm + dind, cmp);
            med3(pn, tabn, pn - 2 * d, pn - d, pn, tabn - 2 * dind, tabn - dind, tabn, cmp);
        }
        med3(pm, tabm, pl, pm, pn, tabl, tabm, tabn, cmp);
    }

    /* Put it at the first position */
    /* Partionning */
    if (cmp(pn, a, tabn, tab, flag))
    {
        swapind(tab, tabn);
        swap(a, pn);
    }

    /* pointers on data array */
    pa = pb = a + es;/* pa and pb start from the beginning of the array */
    pc = pd = a + (n - 1) * es;/* pc and pd start from the end of the array */

    /* similar pointers for index array */
    taba = tabb = tab + es1;
    tabc = tabd = tab + (n - 1) * es1;

    /* here we have
    |a  |pa                            | pc|
    |a  |pb                            | pd|
    |*a |                ?             | ? |
    */
    for (;;)
    {
        /* increase the pointer pb while it points on values lesser  than the pivot (pointer a) */
        while (pb <= pc && (r = cmp(pb, a, tabb, tab, flag)) <= 0)
        {
            if (r == 0)  /*The pivot and  value pointed to by pb are equal */
            {
                /* store the equal value at the location pa and increase pa */
                swapind(taba, tabb);
                taba += es1;
                swap(pa, pb);
                pa += es;
            }
            pb += es;/* next number */
            tabb += es1;
        }

        /* here pb points on a value greater than the pivot */
        /* decrease the pointer pc while it points on a value greater than the pivot (pointer a) */
        while (pb <= pc && (r = cmp(pc, a, tabc, tab, flag)) >= 0)
        {
            if (r == 0)  /*The pivot and  value pointed to by pc are equal */
            {
                /* store the equal value at the location pd and decrease pd */
                swapind(tabc, tabd);
                tabd -= es1;
                swap(pc, pd);
                pd -= es;
            }
            pc -= es;
            tabc -= es1;
        }
        /* here pc points on a value lesser than the pivot */
        if (pb > pc)
        {
            /* here we have
            |a      |pa      |pc|pb     pd|      $|
            |   =*a |    <*a |       >*a  | =*a  $|
            */
            /* partition is done */
            break;
        }
        /*here
        pc  points on a value lesser than the pivot
        and
        pb points on a value greater than the pivot
        swap the values
        */
        swapind(tabb, tabc);
        tabb += es1;
        tabc -= es1;
        swap(pb, pc);
        /* increase pb and decrease pc */
        pb += es;
        pc -= es;
        /* here we have
        |a      |pa      |pb       pc|       pd|      $|
        |   =*a |    <*a |     ?     |    >*a  | =*a  $|
        */
    }

    /* put the equal values in the middle */
    pn = a + n * es;
    r = (int)Min(pa - (char *)a, pb - pa);
    vecswap(a, pb - r, r);

    tabn = tab + n * es1 ;
    r1 = (int)Min(taba - (char *) tab, tabb - taba);
    vecswapind(tab, tabb - r1, r1);

    r = (int)Min(pd - pc, pn - pd - es);
    vecswap(pb, pn - r, r);

    r1 = (int)Min(tabd - tabc, tabn - tabd - es1 );
    vecswapind(tabb, tabn - r1, r1);

    if ((r = (int)(pb - pa)) > es )
        /* recall  sciqsort for the lower part */
    {
        sciqsort(a, tab, flag, r / es, es, es1, cmp, swapcode, lswapcodeind);
    }
    if ((r = (int)(pd - pc)) > es)
    {
        /* Iterate rather than recurse to save stack space */
        a = pn - r;
        tab = tabn - (tabd - tabc);
        n = r / es;
        goto loop;
    }
}
/*--------------------------------------------------------------------------*/
int swapcodeint(char * parmi, char * parmj, int n, int incr)
{
    int i = n;
    register int *pi = (int *) (parmi);
    register int *pj = (int *) (parmj);
    register int inc1 = incr / sizeof(int);
    do
    {
        register int t = *pi;
        *pi = *pj;
        *pj = t;
        pi += inc1;
        pj += inc1;
    }
    while (--i > 0);
    return (0);
}
/*--------------------------------------------------------------------------*/
