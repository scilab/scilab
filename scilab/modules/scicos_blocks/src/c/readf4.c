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
#include "machine.h"

/* Common Block Declarations */
int bfrdr();
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

typedef struct
{	long int cierr;
	long int ciunit;
	long int ciend;
	char *cifmt;
	long int cirec;
} cilist;

#define intersci_1 intersci_

typedef char *address;

typedef struct { double r, i; } doublecomplex;


#ifndef max
#define max(a,b) ((a) >= (b) ? (a) : (b))
#endif

/* Table of constant values */

static int c__1 = 1;
static int c__3 = 3;
static int c__2 = 2;

int readf4(scicos_block *block,int flag)
{

  int nz=block->nz;
  double* z__=block->z;
  double* y=block->outptr[0];  
  int* ny=block->outsz;
  int* ipar=block->ipar;
  double *tvec=block->evout;
  double t=get_scicos_time();

  /* System generated locals */
  address a__1[3], a__2[2];
  int i__1, i__2[3], i__3[2];
  char ch__1[4118], ch__2[4115];
  
  /* Builtin functions */
  int s_cat();
  
  /* Local variables */
  static int mode[2], lfil, kmax;
#define cstk ((char *)&stack_1)
  static int ierr;
#define istk ((int *)&stack_1)
  static int ievt, lfmt;
#define sstk ((float *)&stack_1)
#define zstk ((doublecomplex *)&stack_1)
  static int k, n;
  extern int dcopy_();
  static int lunit;
  extern int cvstr_();
  static int io, no;
  extern int basout_(), clunit_();
  
  /*     Copyright INRIA

     Scicos block simulator
     write read from a binary or formatted file
     include 'stack.h'
     ipar(1) = lfil : file name length
     ipar(2) = lfmt : format length (0) if binary file
     ipar(3) = ievt  : 1 if each data have a an associated time
     ipar(4) = N : buffer length
     ipar(5:4+lfil) = character codes for file name
     ipar(5+lfil:4+lfil+lfmt) = character codes for format if any
     ipar(5+lfil+lfmt:5+lfil+lfmt+ny+ievt) = reading mask */
  

  /* Parameter adjustments */
  --y;
  --ipar;
  --tvec;
  --z__;
  
  
  /* Function Body */
  if (flag == 1) {
    /*     discrete state */
    n = ipar[4];
    k = (int) z__[1];
    ievt = ipar[3];
    kmax = (int) z__[2];
    lunit = (int) z__[3];
    if (k + 1 > kmax && kmax == n) {
      /*     output */
      dcopy_(ny, &z__[n * ievt + 3 + k], &n, &y[1], &c__1);
      /*     .     read a new buffer */
      no = (nz - 3) / n;
      bfrdr(&lunit, &ipar[1], &z__[4], &no, &kmax, &ierr);
      if (ierr != 0) {
	goto L110;
      }
      z__[1] = 1.;
      z__[2] = (double) kmax;
    } else if (k < kmax) {
      /*     output */
      dcopy_(ny, &z__[n * ievt + 3 + k], &n, &y[1], &c__1);
      z__[1] += 1.;
    } else if (k+1> kmax) {
      dcopy_(ny, &z__[n * ievt + 3 + kmax], &n, &y[1], &c__1);
    }
  } else if (flag == 3) {
    n = ipar[4];
    k = (int) z__[1];
    kmax = (int) z__[2];
    if (k > kmax && kmax < n) {
      tvec[1] = t - 1.;
    } else {
      tvec[1] = z__[k + 3];
    }
  } else if (flag == 4) {
    /*     file opening */
    lfil = ipar[1];
    ievt = ipar[3];
    n = ipar[4];
    cvstr_(&lfil, &ipar[5], cha1_1.buf, &c__1, (short)4096);
    lfmt = ipar[2];
    lunit = 0;
    if (lfmt > 0) {
      mode[0] = 1;
      mode[1] = 0;
      clunit_(&lunit, cha1_1.buf, mode, lfil);
      if (iop_1.err > 0) {
	goto L100;
      }
    } else {
      mode[0] = 101;
      mode[1] = 0;
      clunit_(&lunit, cha1_1.buf, mode, lfil);
      if (iop_1.err > 0) {
	goto L100;
      }
    }
    z__[3] = (double) lunit;
    /*     buffer initialisation */
    no = (nz - 3) / n;
    bfrdr(&lunit, &ipar[1], &z__[4], &no, &kmax, &ierr);
    if (ierr != 0) {
      goto L110;
    }
    z__[1] = 1.;
    z__[2] = (double) kmax;
  } else if (flag == 5) {
    lfil = ipar[1];
    n = ipar[4];
    k = (int) z__[1];
    lunit = (int) z__[3];
    if (lunit == 0) {
      return 0;
    }
    i__1 = -lunit;
    clunit_(&i__1, cha1_1.buf, mode, lfil);
    if (iop_1.err > 0) {
      goto L100;
    }
    z__[3] = 0.;
  }
  return 0;
 L100:
  iop_1.err = 0;
  lfil = ipar[1];
  /* Writing concatenation */
  i__2[0] = 5, a__1[0] = "File ";
  i__2[1] = lfil, a__1[1] = cha1_1.buf;
  i__2[2] = 17, a__1[2] = " Cannot be opened";
  s_cat(ch__1, a__1, i__2, &c__3, (short)4118);
  basout_(&io, &iop_1.wte, ch__1, lfil + 22);
  flag = -1;
  return 0;
 L110:
  lfil = ipar[1];
  cvstr_(&lfil, &ipar[5], cha1_1.buf, &c__1, (short)4096);
  i__1 = -lunit;
  clunit_(&i__1, cha1_1.buf, mode, lfil);
  /* Writing concatenation */
  i__3[0] = 19, a__2[0] = "Read error on file ";
  i__3[1] = lfil, a__2[1] = cha1_1.buf;
  s_cat(ch__2, a__2, i__3, &c__2, (short)4115);
  basout_(&io, &iop_1.wte, ch__2, lfil + 19);
  flag = -1;
  return 0;
} /* readf */

#undef zstk
#undef sstk
#undef istk
#undef cstk


int bfrdr(lunit, ipar, z__, no, kmax, ierr)
     int *lunit, *ipar;
     double *z__;
     int *no, *kmax, *ierr;
{
  /* System generated locals */
  int i__1, i__2, i__3;
  cilist ci__1;
  
  /* Builtin functions */
  int s_rsue(), do_uio(), e_rsue(), s_rsfe(), do_fio(), e_rsfe();
  
  /* Local variables */
#define cstk ((char *)&stack_1)
  static int lfmt;
#define istk ((int *)&stack_1)
  static int ievt;
#define sstk ((float *)&stack_1)
#define zstk ((doublecomplex *)&stack_1)
  static int i__, j, n, imask;
  extern /* Subroutine */ int cvstr_();
  static int mm;
  static double tmp[100];
  
  /* Fortran I/O blocks */
  static cilist io___26 = { 1, 0, 1, 0, 0 }; 
  
  
  /* *------------------------------------------------------------------ */

  /* Parameter adjustments */
  --z__;
  --ipar;
  
  /* Function Body */
  ievt = ipar[3];
  n = ipar[4];
  /*      no=(nz-3)/N */
  /*     maximum number of value to read */
  imask = ipar[1] + 5 + ipar[2];
  if (ievt == 0) {
    ++imask;
  }
  mm = 0;
  i__1 = *no - 1;
  for (i__ = 0; i__ <= i__1; ++i__) {
    /* Computing MAX */
    i__2 = mm, i__3 = ipar[imask + i__];
    mm = max(i__2,i__3);
    /* L10: */
  }
  
  lfmt = ipar[2];
  *kmax = 0;
  if (lfmt == 0) {
    /*     unformatted read */
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
      io___26.ciunit = *lunit;
      i__2 = s_rsue(&io___26);
      if (i__2 != 0) {
	goto L100001;
      }
      i__3 = mm;
      for (j = 1; j <= i__3; ++j) {
	i__2 = do_uio(&c__1, (char *)&tmp[j - 1], (short)sizeof(
								 double));
	if (i__2 != 0) {
	  goto L100001;
	}
      }
      i__2 = e_rsue();
    L100001:
      if (i__2 < 0) {
	goto L20;
      }
      if (i__2 > 0) {
	goto L100;
      }
      i__2 = *no - 1;
      for (j = 0; j <= i__2; ++j) {
	z__[j * n + i__] = tmp[ipar[imask + j] - 1];
	/* L11: */
      }
      ++(*kmax);
      /* L12: */
    }
  } else {
    /*     formatted read */
    cvstr_(&ipar[2], &ipar[ipar[1] + 5], cha1_1.buf, &c__1, (short)4096);
    i__1 = n;
    for (i__ = 1; i__ <= i__1; ++i__) {
      ci__1.cierr = 1;
      ci__1.ciend = 1;
      ci__1.ciunit = *lunit;
      ci__1.cifmt = cha1_1.buf;
      i__2 = s_rsfe(&ci__1);
      if (i__2 != 0) {
	goto L100002;
      }
      i__3 = mm;
      for (j = 1; j <= i__3; ++j) {
	i__2 = do_fio(&c__1, (char *)&tmp[j - 1], (short)sizeof(
								 double));
	if (i__2 != 0) {
	  goto L100002;
	}
      }
      i__2 = e_rsfe();
    L100002:
      if (i__2 < 0) {
	goto L20;
      }
      if (i__2 > 0) {
	goto L100;
      }
      i__2 = *no - 1;
      for (j = 0; j <= i__2; ++j) {
	z__[j * n + i__] = tmp[ipar[imask + j] - 1];
	/* L13: */
      }
      ++(*kmax);
      /* L14: */
    }
  }
 L20:
  *ierr = 0;
  return 0;
 L100:
  *ierr = 1;
  return 0;
} /* bfrdr */

#undef zstk
#undef sstk
#undef istk
#undef cstk


