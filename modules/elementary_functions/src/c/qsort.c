/*
 * See Copyright below 
 * Copyright (c) 1992, 1993
 * The Regents of the University of California.  All rights reserved.
 */

#include <stdlib.h>
#include <string.h>
#include "machine.h" 
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
 *  Modified for Scilab Jean-Philippe Chancelier 
 *  to keep a permutation index 
 */
/*--------------------------------------------------------------------------*/ 
/*
 * Qsort routine from Bentley & McIlroy's "Engineering a Sort Function".
 */
/*--------------------------------------------------------------------------*/ 
void sciqsort(char *a, char *tab, int flag, int n, int es, int es1, int (*cmp)(), int (*swapcode)(), int (*lswapcodeind)())
{
  char *pa, *pb, *pc, *pd, *pl, *pm, *pn;
  char *taba, *tabb, *tabc, *tabd, *tabl, *tabm, *tabn;
  int d,dind, r,r1,  swap_cnt;

 loop:	
  swap_cnt = 0;
  if (n < 7) {
    for (pm = a + es, tabm= tab + es1 ; pm < (char *) a + n * es; pm += es, tabm +=es1 )
      {
	for (pl = pm, tabl= tabm ; pl > (char *) a && cmp(pl - es, pl) > 0;  pl -= es, tabl -=es1)
	  {
	    swapind(tabl,tabl- es1);
	    swap(pl, pl - es);
	  }
      }
    return;
  }
  pm = a + (n / 2) * es;
  tabm = tab + (n / 2)*es1 ;
  if (n > 7) {
    pl = a; 
    tabl = tab;
    pn = a + (n - 1) * es; 
    tabn = tab + (n-1) *es1;
    if (n > 40) {
      dind= (n/8) *es1;
      d =   (n/8) *es;
      med3(pl,tabl,pl, pl + d, pl + 2 * d, tabl, tabl + dind, tabl + 2 * dind, cmp);
      med3(pm,tabm,pm - d, pm, pm + d, tabm - dind, tabm, tabm + dind, cmp);
      med3(pn,tabn,pn - 2 * d, pn - d, pn, tabn - 2 * dind, tabn - dind, tabn, cmp);
    }
    med3(pm,tabm,pl, pm, pn, tabl, tabm, tabn, cmp);
  }
  swapind(tab,tabm);
  swap(a, pm);

  pa = pb = a + es;
  pc = pd = a + (n - 1) * es;

  taba = tabb = tab + es1;
  tabc = tabd = tab + (n - 1) * es1;

  for (;;) {
    while (pb <= pc && (r = cmp(pb, a)) <= 0) {
      if (r == 0) {
	swap_cnt = 1;
	swapind(taba,tabb);
	taba +=es1;
	swap(pa, pb);
	pa += es;
      }
      pb += es;
      tabb += es1;
    }
    while (pb <= pc && (r = cmp(pc, a)) >= 0) {
      if (r == 0) {
	swap_cnt = 1;
	swapind(tabc,tabd);
	tabd -= es1;
	swap(pc, pd);
	pd -= es;
      }
      pc -= es;
      tabc -= es1;
    }
    if (pb > pc)
      break;
    swapind(tabb,tabc);
    tabb += es1;
    tabc -= es1;
    swap(pb, pc);
    swap_cnt = 1;
    pb += es;
    pc -= es;
  }

  if (swap_cnt == 0) {  /* Switch to insertion sort */
    for (pm = a + es, tabm= tab + es1 ; pm < (char *) a + n * es; pm += es, tabm +=es1)
      {
	for (pl = pm, tabl= tabm ; pl > (char *) a && cmp(pl - es, pl) > 0;  pl -= es, tabl -=es1)
	  {
	    swapind(tabl,tabl- es1);
	    swap(pl, pl - es);
	  }
      }
    return;
  }

  pn = a + n * es;
  r = (int)Min(pa - (char *)a, pb - pa);
  vecswap(a, pb - r, r);

  tabn = tab + n*es1 ;
  r1 = (int)Min(taba - (char *) tab, tabb - taba);
  vecswapind(tab, tabb - r1, r1);

  r = (int)Min(pd - pc, pn - pd - es);
  vecswap(pb, pn - r, r);

  r1 = (int)Min(tabd - tabc, tabn - tabd - es1 );
  vecswapind(tabb, tabn - r1, r1);

  if ((r = (int)(pb - pa)) > es )
    sciqsort(a, tab,flag, r / es, es, es1, cmp,swapcode,lswapcodeind);
  if ((r = (int)(pd - pc)) > es) { 
    /* Iterate rather than recurse to save stack space */
    a = pn - r;
    tab = tabn - (tabd - tabc);
    n = r / es;
    goto loop;
  }
}
/*--------------------------------------------------------------------------*/ 
int swapcodeint(char * parmi,char * parmj,int n,int incr) 
{ 		
  int i = n;
  register int *pi = (int *) (parmi); 		
  register int *pj = (int *) (parmj); 
  register int inc1 = incr/sizeof(int);
  do 
  { 						
    register int t = *pi;		
    *pi = *pj;				
    *pj = t;				
    pi += inc1;
    pj += inc1;
  } while (--i > 0);				
  return(0);
}
/*--------------------------------------------------------------------------*/ 
