/* erro.f -- translated by f2c (version 19961017).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Common Block Declarations */

IMPORT struct {
    doublereal stk[2];
} stack_;

#define stack_1 stack_

IMPORT struct {
    integer bot, top, idstk[60000]	/* was [6][10000] */, lstk[10000], leps, bbot, bot0, infstk[10000], gbot, gtop, isiz;
} vstk_;

#define vstk_1 vstk_

IMPORT struct {
    integer ids[24576]	/* was [6][4096] */, pstk[4096], rstk[4096], pt, niv, macr, paus, icall, krec;
} recu_;

#define recu_1 recu_

IMPORT struct {
    integer ddt, err, lct[8], lin[65536], lpt[6], hio, rio, wio, rte, wte;
} iop_;

#define iop_1 iop_

IMPORT struct {
    integer err1, err2, errct, toperr, errpt, ieee, catch__;
} errgst_;

#define errgst_1 errgst_

IMPORT struct {
    integer sym, syn[6], char1, fin, fun, lhs, rhs, ran[2], comp[3];
} com_;

#define com_1 com_

IMPORT struct {
    char alfa[63], alfb[63], buf[4096];
} cha1_;

#define cha1_1 cha1_

struct {
    integer wmac, lcntr, nmacs, macnms[600]	/* was [6][100] */, lgptrs[101], bptlg[1000];
} dbg_;

#define dbg_1 dbg_

IMPORT struct {
    integer lbot, ie, is, ipal, nbarg, ladr[1024];
} adre_;

#define adre_1 adre_

IMPORT struct {
    integer nbvars, iwhere[1024], nbrows[1024], nbcols[1024], itflag[1024], ntypes[1024], lad[1024], ladc[1024], lhsvar[1024];
} intersci_;

#define intersci_1 intersci_

/* Table of constant values */

static integer c__9999 = 9999;

/* Subroutine */ int erro_(str, str_len)
char *str;
ftnlen str_len;
{
    /* Builtin functions */
    /* Subroutine */ int s_copy();

    /* Local variables */
#define cstk ((char *)&stack_1)
#define istk ((integer *)&stack_1)
#define sstk ((real *)&stack_1)
#define zstk ((doublecomplex *)&stack_1)
    extern /* Subroutine */ int error_();

/*     Copyright INRIA */
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
    s_copy(cha1_1.buf, str, 4096L, str_len);
    error_(&c__9999);
} /* erro_ */

#undef zstk
#undef sstk
#undef istk
#undef cstk


