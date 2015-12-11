/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*------------------------------------------------------------------------*/
#ifndef __QSORT_H__
#define __QSORT_H__

void sciqsort(char *a, char *tab, int flag, int n, int es, int es1, int (*cmp) (), int (*swapcode) (), int (*swapcodeind) ());
int swapcodeint(char * parmi, char * parmj, int n, int incr);

#define swapcodeind swapcodeint
#define swap(a, b) swapcode(a, b, 1,es)
#define swapind(a, b)  if ( flag==1) swapcodeind(a,b,1,es1)
#define vecswap(a, b, n) if ((n) > 0) swapcode(a, b, n/es,es)
#define vecswapind(a, b, n) if ((n) > 0 && flag == 1) swapcodeind(a,b,n/es1,es1)
#define med3(res,tabres,a, b, c, xa,xb,xc,cmp) cmp(a, b,xa,xb) < 0 ?	\
   (cmp(b, c, xb, xc) < 0 ? (res=b,tabres=xb) : \
    (cmp(a, c, xa, xc) < 0 ? (res=c,tabres=xc) : (res=a,tabres=xa) ))	\
  :(cmp(b, c, xb, xc) > 0 ? (res=b,tabres=xb) : (cmp(a, c, xa, xc) < 0 ? (res=a,tabres=xa) : (res=c,tabres=xc) ))

#endif /* __QSORT_H__ */
