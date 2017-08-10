/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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

#include <math.h>
#include <string.h>
#include <stdio.h>
#include "core_math.h"
#include "genmdsp.h"

#define DSP(Type,Fmt) {\
Type *X;\
Type a;\
double aa;\
X=(Type *)x;\
    --iw;\
    --X;\
    m = Abs(*mm); n = Abs(*nn);\
    dl = ' '; if (m * n > 1) dl = ' ';\
    lbloc = n; nbloc = 1;\
    iw[lbloc + nbloc] = n;\
    lp = -(*nx);s = 0;\
    for (k = 1; k <= n; ++k) {\
	iw[k] = 0;\
	lp += *nx;\
	for (l = 1; l <= m; ++l) {\
	    aa = Abs((double)X[lp + l]);\
	    if (aa == 0) {\
		fl = 0;\
	    } else {\
	      fl = (int)(log(aa)/log(10.))  ;\
	    }\
	    iw[k] = Max(iw[k],fl + 2);\
	}\
	s += iw[k];\
	if (s > *ll - 2) {\
	    iw[lbloc + nbloc] = k - 1;\
	    ++nbloc;\
	    iw[lbloc + nbloc] = n;\
	    s = iw[k];\
	}\
    }\
    if (*mm < 0) {\
      cw[0] = '\0';\
      strcpy(&(cw[0]), "(eye *)");\
      C2F(basout)(&io, lunit, cw, 7L);\
      C2F(basout)(&io, lunit, " ", 1L);\
      if (io == -1) return 0;\
    }\
    k1 = 1;\
    for (ib = 1; ib <= nbloc; ++ib) {\
	k2 = iw[lbloc + ib];\
	if (nbloc != 1) {\
	    C2F(blktit)(lunit, &k1, &k2, &io);\
	    if (io == -1) return 0;\
	}\
	for (l = 1; l <= m; ++l) {\
	  cw[0] = dl;l1 = 1;\
	    for (k = k1; k <= k2; ++k) {\
		a = X[l + (k - 1) * *nx];\
                sprintf((char *)&(cw[l1]),Fmt,iw[k],a);\
		l1 += iw[k]+1;\
	    }\
	    cw[l1] = dl;\
	    C2F(basout)(&io, lunit, cw, l1+1);\
	    if (io == -1) return 0;\
	}\
	k1 = k2 + 1;\
    }\
}

int C2F(genmdsp)(int *typ, int *x, int *nx, int *mm, int *nn, int *ll, int *lunit, char cw[], int *iw, int cw_len)
{
    static int k, l, m, n, s, lbloc, nbloc, k1, l1, k2, ib;
    static char dl;
    static int fl, io, lp;
    extern  int C2F(blktit)(), C2F(basout)();

    switch (*typ)
    {
        case 1:
            DSP(integer1, "%*i ");
            break;
        case 2:
            DSP(integer2, "%*i ");
            break;
        case 4:
            DSP(integer, "%*i ");
            break;
        case 11:
            DSP(unsigned char, "%*u ");
            break;
        case 12:
            DSP(unsigned short, "%*u ");
            break;
        case 14:
            DSP(unsigned int, "%*u ");
            break;
    }
    return 0;
}
