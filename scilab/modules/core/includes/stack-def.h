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

#ifndef STACK_DEF
#define STACK_DEF

#include "machine.h"

#if defined(__SCILAB_TOOLBOX__) && !defined(__INTERNAL_API_SCILAB__) && !defined(__USE_DEPRECATED_STACK_FUNCTIONS__) && !defined(__MEX_INCLUDE__)
#error Using stack-def.h is deprecated. Please use api_scilab instead (try 'help api_scilab'). Note the stack-def.h API will be removed after Scilab 6.0. You can define __USE_DEPRECATED_STACK_FUNCTIONS__ to bypass this error.
#endif

#if _LCC_ & FORDLL
#define IMPORT __declspec (dllimport)
#else
#ifdef FORDLL
#define IMPORT extern  __declspec (dllimport)
#else
#define IMPORT extern
#endif
#endif

/* C version of stack.h (fortran) */

/* csiz used for character coding */
#define csiz 63

/* bsiz size of internal chain buf */
#define bsiz 4096

/* isizt  maximum number of scilab variables global and local */
#define isizt 10000

/* psiz  defines recursion size */
#define psiz 4096

#define nsiz 6

/* lsiz  dim. of vector containing the command line */
#define lsiz 65536

/* nlgh  length of variable names */
#define nlgh nsiz*4

/* vsiz  size of internal scilab stack */
#define vsiz 2

/* intersiz used in interfaces */
#define intersiz 1024

#define maxdb 100
#define maxbpt 1000

/**
 * @TODO : add comment
 */
typedef struct
{
    int ddt; /**< @TODO : add comment */
    int err; /**< @TODO : add comment */
    int lct[8]; /**< @TODO : add comment */
    int lin[lsiz]; /**< @TODO : add comment */
    int lpt[6]; /**< @TODO : add comment */
    int rio; /* Id input (fortran) file descriptor */
    int rte; /* Id input (fortran) terminal descriptor (stdin)  */
    int wte; /* Id output (fortran) terminal descriptor (stdout) */
} IOP_struct;

IMPORT IOP_struct C2F(iop);

/**
 * @TODO : add comment
 */
typedef struct
{
    int err1; /**< @TODO : add comment */
    int err2; /**< @TODO : add comment */
    int errct; /**< @TODO : add comment */
    int toperr; /**< @TODO : add comment */
    int errpt; /**< @TODO : add comment */
    int ieee; /**< @TODO : add comment */
    int errcatch; /**< @TODO : add comment */
} ERRGST_struct;

IMPORT ERRGST_struct C2F(errgst);

/**
 * @TODO : add comment
 */
typedef struct
{
    int sym; /**< @TODO : add comment */
    int syn[nsiz]; /**< @TODO : add comment */
    int char1; /**< @TODO : add comment */
    int fin; /**< @TODO : add comment */
    int fun; /**< @TODO : add comment */
    int lhs; /**< @TODO : add comment */
    int rhs; /**< @TODO : add comment */
    int ran[2]; /**< @TODO : add comment */
    int comp[3]; /**< @TODO : add comment */
} COM_struct;

IMPORT COM_struct C2F(com);

/**
 * @TODO : add comment
 */
typedef struct
{
    int lbot; /**< @TODO : add comment */
    int ie; /**< @TODO : add comment */
    int is; /**< @TODO : add comment */
    int ipal; /**< @TODO : add comment */
    int nbarg; /**< @TODO : add comment */
    int ladr[intersiz]; /**< @TODO : add comment */
} ADRE_struct;

IMPORT ADRE_struct C2F(adre);

/**
 * @TODO : add comment
 */
typedef struct
{
    int wmac; /**< @TODO : add comment */
    int lcntr; /**< @TODO : add comment */
    int nmacs; /**< @TODO : add comment */
    int macnms[nsiz * maxdb]; /**< @TODO : add comment */
    int lgptrs[maxdb + 1]; /**< @TODO : add comment */
    int bptlg[maxbpt]; /**< @TODO : add comment */
} DBG_struct;

IMPORT DBG_struct C2F(dbg);

/**
 * @TODO : add comment
 */
typedef struct
{
    int mmode; /**< @TODO : add comment */
} MTLBC_struct;

IMPORT MTLBC_struct C2F(mtlbc);

/**
 * @TODO : add comment
 */
typedef struct
{
    int iero; /**< @TODO : add comment */
} IERODE_struct;

IMPORT IERODE_struct C2F(ierode);

/**
 * @TODO : add comment
 */
typedef struct
{
    double rownr3[2]; /**< @TODO : add comment */
    double t0; /**< @TODO : add comment */
    double tlast; /**< @TODO : add comment */
    double toutc; /**< @TODO : add comment */
    int lg0; /**< @TODO : add comment */
    int lg1; /**< @TODO : add comment */
    int lgx; /**< @TODO : add comment */
    int iownr3[2]; /**< @TODO : add comment */
    int irfnd; /**< @TODO : add comment */
    int itaskc; /**< @TODO : add comment */
    int ngc; /**< @TODO : add comment */
    int nge; /**< @TODO : add comment */
} LSR001_struct;

//IMPORT LSR001_struct C2F(lsr001);

/**
 * @TODO : add comment
 */
typedef struct
{
    double tret; /**< @TODO : add comment */
    double rowns[209]; /**< @TODO : add comment */
    double ccmax; /**< @TODO : add comment */
    double el0; /**< @TODO : add comment */
    double h__; /**< @TODO : add comment */
    double hmin; /**< @TODO : add comment */
    double hmxi; /**< @TODO : add comment */
    double hu; /**< @TODO : add comment */
    double rc; /**< @TODO : add comment */
    double tn; /**< @TODO : add comment */
    double uround; /**< @TODO : add comment */
    int illin; /**< @TODO : add comment */
    int init; /**< @TODO : add comment */
    int lyh; /**< @TODO : add comment */
    int lewt; /**< @TODO : add comment */
    int lacor; /**< @TODO : add comment */
    int lsavf; /**< @TODO : add comment */
    int lwm; /**< @TODO : add comment */
    int liwm; /**< @TODO : add comment */
    int mxstep; /**< @TODO : add comment */
    int mxhnil; /**< @TODO : add comment */

    int nhnil; /**< @TODO : add comment */
    int ntrep; /**< @TODO : add comment */
    int nslast; /**< @TODO : add comment */
    int nyh; /**< @TODO : add comment */
    int iowns[6]; /**< @TODO : add comment */
    int icf; /**< @TODO : add comment */
    int ierpj; /**< @TODO : add comment */
    int iersl; /**< @TODO : add comment */
    int jcur; /**< @TODO : add comment */

    int jstart; /**< @TODO : add comment */
    int kflag; /**< @TODO : add comment */
    int l; /**< @TODO : add comment */
    int meth; /**< @TODO : add comment */
    int miter; /**< @TODO : add comment */
    int maxord; /**< @TODO : add comment */
    int maxcor; /**< @TODO : add comment */
    int msbp; /**< @TODO : add comment */
    int mxncf; /**< @TODO : add comment */
    int n; /**< @TODO : add comment */
    int nq; /**< @TODO : add comment */
    int nst; /**< @TODO : add comment */
    int nfe; /**< @TODO : add comment */
    int nje; /**< @TODO : add comment */
    int nqu; /**< @TODO : add comment */
} LS0001_struct;

//IMPORT LS0001_struct C2F(ls0001);


/**
 * @TODO : add comment
 */
typedef struct
{
    double tsw; /**< @TODO : add comment */
    double rowns2[20]; /**< @TODO : add comment */
    double pdnorm; /**< @TODO : add comment */
    int insufr; /**< @TODO : add comment */
    int insufi; /**< @TODO : add comment */
    int ixpr; /**< @TODO : add comment */
    int iowns2[2]; /**< @TODO : add comment */
    int jtyp; /**< @TODO : add comment */
    int mused; /**< @TODO : add comment */
    int mxordn; /**< @TODO : add comment */
    int mxords; /**< @TODO : add comment */
} LSA001_struct;

//IMPORT LSA001_struct C2F(lsa001);

typedef struct
{
    int mesflg, lunit;
} EH0001_struct;

//IMPORT EH0001_struct C2F(eh0001);

/**
 * @TODO : add comment
 */
typedef struct
{
    int lf; /**< @TODO : add comment */
    int nfree; /**< @TODO : add comment */
    int nf; /**< @TODO : add comment */
} SCIGSCH_struct;

IMPORT SCIGSCH_struct C2F(scigsch);

/**
 * @TODO : add comment
 */
typedef struct
{
    int lf; /**< @TODO : add comment */
    int nx; /**< @TODO : add comment */
    int nf; /**< @TODO : add comment */
} SCISCH_struct;

IMPORT SCISCH_struct C2F(scisch);

/**
 * @TODO : add comment
 */
typedef union
{
    struct
    {
        int lf; /**< @TODO : add comment */
        int nfree; /**< @TODO : add comment */
        int nf; /**< @TODO : add comment */
    } _1; /**< @TODO : add comment */
    struct
    {
        int lf; /**< @TODO : add comment */
        int nx; /**< @TODO : add comment */
        int nf; /**< @TODO : add comment */
    } _2; /**< @TODO : add comment */
} SCIZGSCH_union;

IMPORT SCIZGSCH_union C2F(scizgsch);

typedef struct
{
    int iflag; /* interruption flag CTRL-C */
    int interruptible; /* */
} BASBRK_struct;

IMPORT BASBRK_struct C2F(basbrk);

/**
* simpmd : rational fraction simplification mode
* used in polynomials and inisci.f (core)
*/
typedef struct
{
    int simpmd;
} CSIMP_struct;

IMPORT CSIMP_struct C2F(csimp);


#endif /** STACK_DEF  **/

