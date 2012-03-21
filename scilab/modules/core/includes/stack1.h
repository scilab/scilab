/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef STACK1_H 
#define STACK1_H 

#if defined(__SCILAB_TOOLBOX__) && !defined(__INTERNAL_API_SCILAB__) && !defined(__USE_DEPRECATED_STACK_FUNCTIONS__) && !defined(__MEX_INCLUDE__)
    #error Using stack1.h is deprecated. Please use api_scilab instead (try 'help api_scilab'). Note the stack1.h API will be removed after Scilab 6.0. You can define __USE_DEPRECATED_STACK_FUNCTIONS__ to bypass this error.
#endif

#include "machine.h"
#include "scisparse.h" /* SciSparse */

typedef  void (*S_fp)(char *,int *);

extern int C2F(ogettype)(int *lw);


/** used in stack1.c : defined elsewhere */

extern  int C2F(cvname)(int *,char *,int *, unsigned long int);
extern int C2F(icopy)(int *,int *,int *,int *,int *);
extern int C2F(dcopy)(int *,double *,int *,double *,int *);
extern int C2F(scidcopy)(int *,double *,int *,double *,int *);
extern int C2F(unsfdcopy)(int *,double *,int *,double *,int *);
extern int C2F(rcopy)(int *,float *,int *,float *,int *);
extern int C2F(stackg)(int *);

/* calelm */

extern int C2F(entier)(int *n, double *d__, int *s);
extern int C2F(simple)(int *n, double *d__, float *s);
extern int C2F(iset)(int *n, int *dx, int *dy, int *incy);

/* stack0.c */

extern int C2F(stacki2d)(int *n, int *il1, int *dl1);
extern int C2F(stackr2d)(int *n, int *sl1, int *dl1);
extern int C2F(stackc2i)(int *n, int *sl1, int *il1);

/* stack1.c */

int C2F(getwsmat)(char *fname, int *topk, int *lw, int *m, int *n, int *ilr, int *ilrd, unsigned long fname_len);
int C2F(getlistwsmat)(char *fname, int *topk, int *spos,int*lnum, int *m, int *n, int *ilr, int *ilrd, unsigned long fname_len);

char * get_fname ( char *fname,   unsigned long fname_len);
int cre_smat_from_str (char *fname,  int *lw, int * m, int * n, char **Str,    unsigned long fname_len);

int C2F(getlistmat)(char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getmat)(char *fname, int *topk, int *lw, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);

int C2F(listcremat)(char *fname, int *lw, int *numi, int *stlw, int *it, int *m, int *n, int *lrs, int *lcs, unsigned long fname_len);
int C2F(cremat)(char *fname, int *lw, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(crematvar)(int *id, int *lw, int *it, int *m, int *n, double *rtab, double *itab);
int C2F(crebmatvar) (int *id, int *lw, int *m, int *n, int *val);
int C2F(cresmatvar) (int *id, int *lw, char *str, int *lstr, unsigned long str_len);
int C2F(fakecremat)(int *lw, int *it, int *m, int *n, int *lr, int *lc);
int C2F(getlistbmat)(char *fname, int *topk, int *spos, int *lnum, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(getbmat)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);

int C2F(listcrebmat)(char *fname, int *lw, int *numi, int *stlw, int *m, int *n, int *lrs, unsigned long fname_len);
int C2F(crebmat)(char *fname, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(fakecrebmat)( int *lw, int *m, int *n, int *lr);

int C2F(getsparse)(char *fname, int *topk, int *lw, int *it, int *m, int *n, int *nel, int *mnel, int *icol, int *lr, int *lc, unsigned long fname_len);
int C2F(getlistsparse)(char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *nel, int *mnel, int *icol, int *lr, int *lc, unsigned long fname_len);
int C2F(listcresparse)(char *fname, int *lw, int *numi, int *stlw, int *it, int *m, int *n, int *nel, int *mnel, int *icol, int *lrs, int *lcs, unsigned long fname_len);
int C2F(cresparse)(char *fname, int *lw, int *it, int *m, int *n, int *nel, int *mnel, int *icol, int *lr, int *lc, unsigned long fname_len);

int C2F(getlistvect)(char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getvect)(char *fname, int *topk, int *lw, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getrmat)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(getcmat)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(getrvect)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(getscalar)(char *fname, int *topk, int *lw, int *lr, unsigned long fname_len);
int C2F(matsize)(char *fname, int *topk, int *lw, int *m, int *n, unsigned long fname_len);
int C2F(vectsize)(char *fname, int *topk, int *lw, int *n, unsigned long fname_len);
int C2F(matbsize)(char *fname, int *topk, int *lw, int *m, int *n, unsigned long fname_len);
int C2F(getsmat)(char *fname, int *topk, int *lw, int *m, int *n, int *i__, int *j, int *lr, int *nlr, unsigned long fname_len);
int C2F(getsimat)(char *fname, int *topk, int *lw, int *m, int *n, int *i__, int *j, int *lr, int *nlr, unsigned long fname_len);
int C2F(listcresmat)(char *fname, int *lw, int *numi, int *stlw, int *m, int *n, int *nchar, int *job, int *ilrs, unsigned long fname_len);
int C2F(cresmat)(char *fname, int *lw, int *m, int *n, int *nchar, unsigned long fname_len);
int C2F(cresmati)(char *fname, int *stlw, int *m, int *n, int *nchar, int *job, int *lr, int *sz, unsigned long fname_len);
int C2F(cresmat1)(char *fname, int *lw, int *m, int *nchar, unsigned long fname_len);
int C2F(cresmat3)(char *fname, int *lw, int *m, int *n, int *nchar, char *buffer, unsigned long fname_len, unsigned long buffer_len);
int C2F(cresmat2)(char *fname, int *lw, int *nchar, int *lr, unsigned long fname_len);
int C2F(smatj)(char *fname, int *lw, int *j, unsigned long fname_len);
int C2F(lmatj)(char *fname, int *lw, int *j, unsigned long fname_len);
int C2F(pmatj)(char *fname, int *lw, int *j, unsigned long fname_len);
int C2F(copysmat)(char *fname, int *flw, int *tlw, unsigned long fname_len);
int C2F(setsimat)(char *fname, int *lw, int *i__, int *j, int *nlr, unsigned long fname_len);
int C2F(realmat)(void);
int C2F(crewmat)(char *fname, int *lw, int *m, int *lr, unsigned long fname_len);
int C2F(copyobj)(char *fname, int *lw, int *lwd, unsigned long fname_len);
int C2F(vcopyobj)(char *fname, int *lw, int *lwd, unsigned long fname_len);
int C2F(swapmat)(char *fname, int *topk, int *lw, int *it1, int *m1, int *n1, int *mn1, int *it2, int *m2, int *n2, int *mn2, unsigned long fname_len);
int C2F(insmat)(int *topk, int *lw, int *it, int *m, int *n, int *lr, int *lc, int *lr1, int *lc1);
int C2F(stackinfo)(int *lw, int *typ);
int C2F(allmat)(char *fname, int *topk, int *lw, int *m, int *n, unsigned long fname_len);
int C2F(allmatset)(char *fname, int *lw, int *m, int *n, unsigned long fname_len);
int C2F(getilist)(char *fname, int *topk, int *lw, int *n, int *i__, int *ili, unsigned long fname_len);
int C2F(objvide)(char *fname, int *lw, unsigned long fname_len);
int C2F(getexternal)(char *fname, int *topk, int *lw, char *name__, int *type__, S_fp setfun, unsigned long fname_len, unsigned long name_len);
int C2F(getpoly)(char *fname, int *topk, int *lw, int *it, int *m, int *n, char *name__, int *namel, int *ilp, int *lr, int *lc, unsigned long fname_len, unsigned long name_len);
int C2F(crewimat)(char *fname, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(getwimat)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(checkval)(char *fname, int *ival1, int *ival2, unsigned long fname_len);
int C2F(optvarget)(char *fname, int *topk, int *iel, char *name__, unsigned long fname_len, unsigned long name_len);
int C2F(bufstore)(char *fname, int *lbuf, int *lbufi, int *lbuff, int *lr, int *nlr, unsigned long fname_len);
int C2F(crestringv)(char *fname, int *spos, int *ilorig, int *lw, unsigned long fname_len);

int C2F(crepointer)(char *fname, int *spos, int *lw, unsigned long fname_len);
int C2F(listcrepointer)(char *fname, int *lw, int *numi, int *stlw,  int *lrs,  unsigned long fname_len);

int C2F(lcrestringmatfromc)(char *fname, int *spos, int *numi, int *stlw, int *lorig, int *m, int *n, unsigned long fname_len);
int C2F(crestringmatfromc)(char *fname, int *spos, int *lorig, int *m, int *n, unsigned long fname_len);
int C2F(crelist)(int *slw, int *ilen, int *lw);
int C2F(cretlist)(int *slw, int *ilen, int *lw);
int C2F(cremlist)(int *slw, int *ilen, int *lw);
int C2F(getlistvectrow)(char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getvectrow)(char *fname, int *topk, int *spos, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getlistvectcol)(char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getvectcol)(char *fname, int *topk, int *spos, int *it, int *m, int *n, int *lr, int *lc, unsigned long fname_len);
int C2F(getlistscalar)(char *fname, int *topk, int *spos, int *lnum, int *lr, unsigned long fname_len);
int C2F(listcrestring)(char *fname, int *lw, int *numi, int *stlw, int *nch, int *ilrs, unsigned long fname_len);
int C2F(crestring)(char *fname, int *spos, int *nchar, int *ilrs, unsigned long fname_len);
int C2F(crestringi)(char *fname, int *stlw, int *nchar, int *ilrs, unsigned long fname_len);
int C2F(getlistsimat)(char *fname, int *topk, int *spos, int *lnum, int *m, int *n, int *i__, int *j, int *lr, int *nlr, unsigned long fname_len);


int C2F(getsimati)(char *fname, int *topk, int *spos, int *lw, int *m, int *n, int *i, int *j, int *lr, int *nlr,int *inlist,int *nel, unsigned long fname_len);
int C2F(getonepoly)(char *fname, int *topk, int *lw, int *it, int *md, char *name__, int *namel, int *lr, int *lc, unsigned long fname_len, unsigned long name_len);

int C2F(getlistpointer)(char *fname, int *topk, int *spos, int *lnum, int *lr, unsigned long fname_len);
 int C2F(getpointer)(char *fname, int *topk, int *lw, int *lr, unsigned long fname_len);




int C2F(mspcreate)(int *lw, int *m, int *n, int *nzmax, int *it);
int C2F(cresmat4)(char *fname, int *lw, int *m, int *nchar, int *lr, unsigned long fname_len);
int C2F(credata) (char *fname, int *lw, int m, unsigned long fname_len);

int cre_listsmat_from_str(char *fname, int *lw, int *numi, int *stlw, int *m, int *n, char **Str, long unsigned int fname_len);

int cre_sparse_from_ptr (char *fname,int * lw,int *m, int *n,SciSparse *Str, unsigned long fname_len );

int cre_listsparse_from_ptr(char *fname,int * lw,int * numi,int * stlw,int *  m,int * n, SciSparse *M, unsigned long fname_len );



/**********************************************************************
 * INT MATRICES 
 **********************************************************************/
int C2F(getlistimat) (char *fname, int *topk, int *spos, int *lnum, int *it, int *m, int *n, int *lr, long unsigned int fname_len);

int C2F(getimat) (char *fname, int *topk, int *lw, int *it, int *m, int *n, int *lr, long unsigned int fname_len);

int C2F(listcreimat) (char *fname, int *lw, int *numi, int *stlw, int *it, int *m, int *n, int *lrs, long unsigned int fname_len);

int C2F(creimat) (char *fname, int *lw, int *it, int *m, int *n, int *lr, long unsigned int fname_len);
int C2F(creimati)(char *fname,int *stlw,int *it,int *m,int *n,int *lr,int *flagx,unsigned long fname_len);

/**********************************************************************
 * HANDLE MATRICES 
 **********************************************************************/
int C2F(getlisthmat)(char *fname, int *topk, int *spos, int *lnum, int *m, int *n, int *lr, unsigned long fname_len);
int C2F(gethmat)(char *fname, int *topk, int *lw, int *m, int *n, int *lr, unsigned long fname_len);


int C2F(listcrehmat)(char *fname, int *lw, int *numi, int *stlw, int *m, int *n, int *lrs, unsigned long fname_len);
int C2F(crehmat)(char *fname, int *lw, int *m, int *n, int *lr, unsigned long fname_len);

#endif 
