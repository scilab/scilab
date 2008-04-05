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

typedef  void (*S_fp)(char *,int *);

extern integer C2F(ogettype)(integer *lw);


/** used in stack1.c : defined elsewhere */

extern  int C2F(cvname)(integer *,char *,integer *, unsigned long int);
extern int C2F(icopy)(integer *,integer *,integer *,integer *,integer *);
extern int C2F(dcopy)(integer *,double *,integer *,double *,integer *);
extern int C2F(scidcopy)(integer *,double *,integer *,double *,integer *);
extern int C2F(unsfdcopy)(integer *,double *,integer *,double *,integer *);
extern int C2F(rcopy)(integer *,float *,integer *,float *,integer *);
extern void C2F(cstringf)(char ***,integer *,integer *,integer *,integer *,integer *);
extern int C2F(stringc)(integer *,char ***,integer *);
extern int C2F(stackg)(integer *);

/* calelm */

extern int C2F(entier)(integer *n, double *d__, integer *s);
extern int C2F(simple)(integer *n, double *d__, float *s);
extern int C2F(iset)(integer *n, integer *dx, integer *dy, integer *incy);

/* stack0.c */

extern int C2F(stacki2d)(integer *n, integer *il1, integer *dl1);
extern int C2F(stackr2d)(integer *n, integer *sl1, integer *dl1);
extern int C2F(stackc2i)(integer *n, integer *sl1, integer *il1);

/* stack1.c */

int C2F(getwsmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *ilr, integer *ilrd, unsigned long fname_len);
int C2F(getlistwsmat)(char *fname, integer *topk, integer *spos,integer*lnum, integer *m, integer *n, integer *ilr, integer *ilrd, unsigned long fname_len);

char * get_fname ( char *fname,   unsigned long fname_len);
int cre_smat_from_str (char *fname,  integer *lw, integer * m, integer * n, char **Str,    unsigned long fname_len);

int C2F(getlistmat)(char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getmat)(char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);

int C2F(listcremat)(char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *lrs, integer *lcs, unsigned long fname_len);
int C2F(cremat)(char *fname, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(crematvar)(integer *id, integer *lw, integer *it, integer *m, integer *n, double *rtab, double *itab);
int C2F(crebmatvar) (integer *id, integer *lw, integer *m, integer *n, integer *val);
int C2F(cresmatvar) (integer *id, integer *lw, char *str, integer *lstr, unsigned long str_len);
int C2F(fakecremat)(integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc);
int C2F(getlistbmat)(char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(getbmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);

int C2F(listcrebmat)(char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *lrs, unsigned long fname_len);
int C2F(crebmat)(char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(fakecrebmat)( integer *lw, integer *m, integer *n, integer *lr);

int C2F(getsparse)(char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getlistsparse)(char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len);
int C2F(listcresparse)(char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lrs, integer *lcs, unsigned long fname_len);
int C2F(cresparse)(char *fname, integer *lw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len);

int C2F(getlistvect)(char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getvect)(char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getrmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(getcmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(getrvect)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(getscalar)(char *fname, integer *topk, integer *lw, integer *lr, unsigned long fname_len);
int C2F(matsize)(char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len);
int C2F(vectsize)(char *fname, integer *topk, integer *lw, integer *n, unsigned long fname_len);
int C2F(matbsize)(char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len);
int C2F(getsmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len);
int C2F(getsimat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len);
int C2F(listcresmat)(char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *nchar, integer *job, integer *ilrs, unsigned long fname_len);
int C2F(cresmat)(char *fname, integer *lw, integer *m, integer *n, integer *nchar, unsigned long fname_len);
int C2F(cresmati)(char *fname, integer *stlw, integer *m, integer *n, integer *nchar, integer *job, integer *lr, integer *sz, unsigned long fname_len);
int C2F(cresmat1)(char *fname, integer *lw, integer *m, integer *nchar, unsigned long fname_len);
int C2F(cresmat3)(char *fname, integer *lw, integer *m, integer *n, integer *nchar, char *buffer, unsigned long fname_len, unsigned long buffer_len);
int C2F(cresmat2)(char *fname, integer *lw, integer *nchar, integer *lr, unsigned long fname_len);
int C2F(smatj)(char *fname, integer *lw, integer *j, unsigned long fname_len);
int C2F(lmatj)(char *fname, integer *lw, integer *j, unsigned long fname_len);
int C2F(pmatj)(char *fname, integer *lw, integer *j, unsigned long fname_len);
int C2F(copysmat)(char *fname, integer *flw, integer *tlw, unsigned long fname_len);
int C2F(setsimat)(char *fname, integer *lw, integer *i__, integer *j, integer *nlr, unsigned long fname_len);
int C2F(realmat)(void);
int C2F(crewmat)(char *fname, integer *lw, integer *m, integer *lr, unsigned long fname_len);
int C2F(copyobj)(char *fname, integer *lw, integer *lwd, unsigned long fname_len);
int C2F(vcopyobj)(char *fname, integer *lw, integer *lwd, unsigned long fname_len);
int C2F(swapmat)(char *fname, integer *topk, integer *lw, integer *it1, integer *m1, integer *n1, integer *mn1, integer *it2, integer *m2, integer *n2, integer *mn2, unsigned long fname_len);
int C2F(insmat)(integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, integer *lr1, integer *lc1);
int C2F(stackinfo)(integer *lw, integer *typ);
int C2F(allmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len);
int C2F(allmatset)(char *fname, integer *lw, integer *m, integer *n, unsigned long fname_len);
int C2F(getilist)(char *fname, integer *topk, integer *lw, integer *n, integer *i__, integer *ili, unsigned long fname_len);
int C2F(objvide)(char *fname, integer *lw, unsigned long fname_len);
int C2F(getexternal)(char *fname, integer *topk, integer *lw, char *name__, int *type__, S_fp setfun, unsigned long fname_len, unsigned long name_len);
int C2F(getpoly)(char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, char *name__, integer *namel, integer *ilp, integer *lr, integer *lc, unsigned long fname_len, unsigned long name_len);
int C2F(crewimat)(char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(getwimat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(checkval)(char *fname, integer *ival1, integer *ival2, unsigned long fname_len);
int C2F(optvarget)(char *fname, integer *topk, integer *iel, char *name__, unsigned long fname_len, unsigned long name_len);
int C2F(bufstore)(char *fname, integer *lbuf, integer *lbufi, integer *lbuff, integer *lr, integer *nlr, unsigned long fname_len);
int C2F(crestringv)(char *fname, integer *spos, integer *ilorig, integer *lw, unsigned long fname_len);

int C2F(crepointer)(char *fname, integer *spos, integer *lw, unsigned long fname_len);
int C2F(listcrepointer)(char *fname, integer *lw, integer *numi, integer *stlw,  integer *lrs,  unsigned long fname_len);

int C2F(lcrestringmatfromc)(char *fname, integer *spos, integer *numi, integer *stlw, integer *lorig, integer *m, integer *n, unsigned long fname_len);
int C2F(crestringmatfromc)(char *fname, integer *spos, integer *lorig, integer *m, integer *n, unsigned long fname_len);
int C2F(crelist)(integer *slw, integer *ilen, integer *lw);
int C2F(cretlist)(integer *slw, integer *ilen, integer *lw);
int C2F(cremlist)(integer *slw, integer *ilen, integer *lw);
int C2F(getlistvectrow)(char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getvectrow)(char *fname, integer *topk, integer *spos, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getlistvectcol)(char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getvectcol)(char *fname, integer *topk, integer *spos, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len);
int C2F(getlistscalar)(char *fname, integer *topk, integer *spos, integer *lnum, integer *lr, unsigned long fname_len);
int C2F(listcrestring)(char *fname, integer *lw, integer *numi, integer *stlw, integer *nch, integer *ilrs, unsigned long fname_len);
int C2F(crestring)(char *fname, integer *spos, integer *nchar, integer *ilrs, unsigned long fname_len);
int C2F(crestringi)(char *fname, integer *stlw, integer *nchar, integer *ilrs, unsigned long fname_len);
int C2F(getlistsimat)(char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len);


int C2F(getsimati)(char *fname, integer *topk, integer *spos, integer *lw, integer *m, integer *n, integer *i, integer *j, integer *lr, integer *nlr,integer *inlist,integer *nel, unsigned long fname_len);
int C2F(getonepoly)(char *fname, integer *topk, integer *lw, integer *it, integer *md, char *name__, integer *namel, integer *lr, integer *lc, unsigned long fname_len, unsigned long name_len);

int C2F(getlistpointer)(char *fname, integer *topk, integer *spos, integer *lnum, integer *lr, unsigned long fname_len);
 int C2F(getpointer)(char *fname, integer *topk, integer *lw, integer *lr, unsigned long fname_len);




int C2F(mspcreate)(integer *lw, integer *m, integer *n, integer *nzmax, integer *it);
int C2F(cresmat4)(char *fname, integer *lw, integer *m, integer *nchar, integer *lr, unsigned long fname_len);
int C2F(credata) (char *fname, integer *lw, integer m, unsigned long fname_len);

int cre_listsmat_from_str(char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, char **Str, long unsigned int fname_len);

int cre_sparse_from_ptr (char *fname,integer * lw,integer *m, integer *n,SciSparse *Str, unsigned long fname_len );

int cre_listsparse_from_ptr(char *fname,integer * lw,integer * numi,integer * stlw,integer *  m,integer * n, SciSparse *M, unsigned long fname_len );



/**********************************************************************
 * INT MATRICES 
 **********************************************************************/
int C2F(getlistimat) (char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len);

int C2F(getimat) (char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len);

int C2F(listcreimat) (char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *lrs, long unsigned int fname_len);

int C2F(creimat) (char *fname, integer *lw, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len);
int C2F(creimati)(char *fname,integer *stlw,integer *it,integer *m,integer *n,integer *lr,int *flagx,unsigned long fname_len);

/**********************************************************************
 * HANDLE MATRICES 
 **********************************************************************/
int C2F(getlisthmat)(char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *lr, unsigned long fname_len);
int C2F(gethmat)(char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);


int C2F(listcrehmat)(char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *lrs, unsigned long fname_len);
int C2F(crehmat)(char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len);

#endif 
