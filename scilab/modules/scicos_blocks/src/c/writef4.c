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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
#include "scicos_block.h"
#include <math.h>


/* Common Block Declarations */

struct {
    double stk[2];
} stack_;

#define stack_1 stack_

struct {
    int bot, top, idstk[60000]	/* was [6][10000] */, lstk[10000], 
	    leps, bbot, bot0, infstk[10000], gbot, gtop, isiz;
} vstk_;

#define vstk_1 vstk_

struct {
    int ids[24576]	/* was [6][4096] */, pstk[4096], rstk[4096], pt, niv, 
	    macr, paus, icall, krec;
} recu_;

#define recu_1 recu_

struct {
    int ddt, err, lct[8], lin[65536], lpt[6], hio, rio, wio, rte, wte;
} iop_;

#define iop_1 iop_

struct {
    int err1, err2, errct, toperr, errpt, ieee, catch__;
} errgst_;

#define errgst_1 errgst_

struct {
    int sym, syn[6], char1, fin, fun, lhs, rhs, ran[2], comp[3];
} com_;

#define com_1 com_

struct {
    char alfa[63], alfb[63], buf[4096];
} cha1_;

#define cha1_1 cha1_

struct {
    int wmac, lcntr, nmacs, macnms[600]	/* was [6][100] */, lgptrs[
	    101], bptlg[1000];
} dbg_;

#define dbg_1 dbg_

struct {
    int lbot, ie, is, ipal, nbarg, ladr[1024];
} adre_;

#define adre_1 adre_

struct {
    int nbvars, iwhere[1024], nbrows[1024], nbcols[1024], itflag[1024], 
	    ntypes[1024], lad[1024], ladc[1024], lhsvar[1024];
} intersci_;

#define intersci_1 intersci_

typedef struct
{	long int cierr;
	long int ciunit;
	long int ciend;
	char *cifmt;
	long int cirec;
} cilist;

typedef char *address;

typedef struct { double r, i; } doublecomplex;

/* Table of constant values */

static int c__1 = 1;
static double c_b12 = 0.;
static int c__3 = 3;

int writef4(scicos_block *block,int flag)
{
  double* z__=block->z;
  double* u=block->inptr[0];  
  int* nu=block->insz;
  int* ipar=block->ipar;
  int nevprt=block->nevprt;
  double t=get_scicos_time();

  /* System generated locals */
  address a__1[3];
  int i__1, i__2, i__3, i__4[3];
  char ch__1[4118];
  cilist ci__1;
  
  /* Builtin functions */
  int s_wsfe(), do_fio(), e_wsfe(), s_wsue(), do_uio(), e_wsue();
  int s_cat();
  
  /* Local variables */
  static int mode[2], lfil;
  extern int dset_();
  static int lfmt;
#define cstk ((char *)&stack_1)
#define istk ((int *)&stack_1)
#define sstk ((real *)&stack_1)
#define zstk ((doublecomplex *)&stack_1)
  static int i__, j, k, n, iflag, lunit;
  extern int cvstr_();
  static int io;
  extern int basout_(), clunit_();
  
  /* Fortran I/O blocks */
  static cilist io___10 = { 1, 0, 0, 0, 0 };
  static cilist io___14 = { 0, 0, 0, 0, 0 };
  
  
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     write input to a binary or formatted file */
  /*     ipar(1) = lfil : file name length */
  /*     ipar(2) = lfmt : format length (0) if binary file */
  /*     ipar(3)          unused */
  /*     ipar(4) = N : buffer length */
  /*     ipar(5:4+lfil) = character codes for file name */
  /*     ipar(5+lfil:4+lfil+lfmt) = character codes for format if any */
  
  /* Copyright INRIA */
  /* *------------------------------------------------------------------ */
  /* vsiz  size of internal scilab stack */
  
  /* bsiz  size of internal chain buf */
  
  /* isizt  maximum number of scilab variables global and local */
  /* isiz maximum number of scilab local variables */
  /* psiz  defines recursion size */
  /* lsiz  dim. of vector containing the command line */
  /* nlgh  length of variable names */
  /* csiz  used for character coding */
  /* intersiz used in interfaces */
  /* *------------------------------------------------------------------- */
  /* c (DLL Digital Visual Fortran) */
  /* DEC$ IF DEFINED (FORDLL) */
  /* DEC$ ATTRIBUTES DLLIMPORT:: /stack/, /vstk/, /recu/, /iop/ */
  /* DEC$ ATTRIBUTES DLLIMPORT:: /errgst/, /com/, /adre/ */
  /* DEC$ ATTRIBUTES DLLIMPORT:: /intersci/ ,/cha1/ */
  /* DEC$ ENDIF */
  /*     --------------------------------------------------------------- */
  
  /* *------------------------------------------------------------------ */
  
  /* Parameter adjustments */
  --u;
  --ipar;
  --z__;
  
  /* Function Body */
  n = ipar[4];
  k = (int) z__[1];
  lunit = (int) z__[2];
  
  if (flag == 2 && nevprt > 0) {
    /*     add new point to the buffer */
    ++k;
    z__[k + 2] = t;
    i__1 = *nu;
    for (i__ = 1; i__ <= i__1; ++i__) {
      z__[n + 2 + (i__ - 1) * n + k] = u[i__];
      /* L1: */
    }
    z__[1] = (double) k;
    if (k < n) {
      return 0;
    }
    /*     write on the file */
    if (ipar[2] > 0) {
      /*     .     formatted write */
      cvstr_(&ipar[2], &ipar[ipar[1] + 5], cha1_1.buf, &c__1, (short)
	     4096);
      i__1 = k;
      for (j = 1; j <= i__1; ++j) {
	ci__1.cierr = 1;
	ci__1.ciunit = lunit;
	ci__1.cifmt = cha1_1.buf;
	i__2 = s_wsfe(&ci__1);
	if (i__2 != 0) {
	  goto L100;
	}
	i__3 = *nu;
	for (i__ = 0; i__ <= i__3; ++i__) {
	  i__2 = do_fio(&c__1, (char *)&z__[n + 2 + (i__ - 1) * n + 
					    j], (short)sizeof(double));
	  if (i__2 != 0) {
	    goto L100;
	  }
	}
	i__2 = e_wsfe();
	if (i__2 != 0) {
	  goto L100;
	}
	/* L10: */
      }
    } else {
      /*     .     unformatted write */
      i__1 = k;
      for (j = 1; j <= i__1; ++j) {
	io___10.ciunit = lunit;
	i__2 = s_wsue(&io___10);
	if (i__2 != 0) {
	  goto L100;
	}
	i__3 = *nu;
	for (i__ = 0; i__ <= i__3; ++i__) {
	  i__2 = do_uio(&c__1, (char *)&z__[n + 2 + (i__ - 1) * n + 
					    j], (short)sizeof(double));
	  if (i__2 != 0) {
	    goto L100;
	  }
	}
	i__2 = e_wsue();
	if (i__2 != 0) {
	  goto L100;
	}
	/* L11: */
      }
    }
    z__[1] = 0.;
  } else if (flag == 4) {
    /*     file opening */
    lfil = ipar[1];
    cvstr_(&lfil, &ipar[5], cha1_1.buf, &c__1, (short)4096);
    lfmt = ipar[2];
    lunit = 0;
    if (lfmt > 0) {
      mode[0] = 3;
      mode[1] = 0;
      clunit_(&lunit, cha1_1.buf, mode, lfil);
      if (iop_1.err > 0) {
	goto L100;
      }
    } else {
      mode[0] = 103;
      mode[1] = 0;
      clunit_(&lunit, cha1_1.buf, mode, lfil);
      if (iop_1.err > 0) {
	goto L100;
      }
    }
    z__[1] = 0.;
    z__[2] = (double) lunit;
    z__[3] = t;
    i__1 = *nu * n;
    dset_(&i__1, &c_b12, &z__[4], &c__1);
  } else if (flag == 5) {
    if (lunit == 0) {
      return 0;
    }
    if (k >= 1) {
      /*     write on the file */
      lfmt = ipar[2];
      if (lfmt > 0) {
	/*     .        formatted write */
	cvstr_(&lfmt, &ipar[ipar[1] + 5], cha1_1.buf, &c__1, (short)
	       4096);
	i__1 = k;
	for (j = 1; j <= i__1; ++j) {
	  ci__1.cierr = 0;
	  ci__1.ciunit = lunit;
	  ci__1.cifmt = cha1_1.buf;
	  s_wsfe(&ci__1);
	  i__2 = *nu;
	  for (i__ = 0; i__ <= i__2; ++i__) {
	    do_fio(&c__1, (char *)&z__[n + 2 + (i__ - 1) * n + j],
		   (short)sizeof(double));
	  }
	  e_wsfe();
	  /* L20: */
	}
      } else {
	/*     .        unformatted write */
	i__1 = k;
	for (j = 1; j <= i__1; ++j) {
	  io___14.ciunit = lunit;
	  s_wsue(&io___14);
	  i__2 = *nu;
	  for (i__ = 0; i__ <= i__2; ++i__) {
	    do_uio(&c__1, (char *)&z__[n + 2 + (i__ - 1) * n + j],
		   (short)sizeof(double));
	  }
	  e_wsue();
	  /* L21: */
	}
      }
    }
    lfil = ipar[1];
    i__1 = -lunit;
    clunit_(&i__1, cha1_1.buf, mode, lfil);
    if (iop_1.err > 0) {
      goto L100;
    }
    z__[2] = 0.;
  }
  return 0;
 L100:
  iop_1.err = 0;
  /* Writing concatenation */
  i__4[0] = 5, a__1[0] = "File ";
  i__4[1] = lfil, a__1[1] = cha1_1.buf;
  i__4[2] = 17, a__1[2] = " Cannot be opened";
  s_cat(ch__1, a__1, i__4, &c__3, (short)4118);
  basout_(&io, &iop_1.wte, ch__1, lfil + 22);
  iflag = -1;
  return 0;
} /* writef_ */

#undef zstk
#undef sstk
#undef istk
#undef cstk


