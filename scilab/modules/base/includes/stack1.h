#ifndef STACK1_H 
#define STACK1_H 


typedef  void (*S_fp) __PARAMS((char *,int *));

/** used in stack1.c : defined elsewhere */

extern  int C2F(cvname) __PARAMS((integer *,char *,integer *, unsigned long int));
extern  int C2F(cvstr)  __PARAMS((integer *,integer *,char *,integer *,unsigned long int));
extern int C2F(icopy)  __PARAMS((integer *,integer *,integer *,integer *,integer *));
extern int C2F(dcopy)  __PARAMS((integer *,double *,integer *,double *,integer *));
extern int C2F(unsfdcopy)  __PARAMS((integer *,double *,integer *,double *,integer *));
extern int C2F(rcopy)  __PARAMS((integer *,float *,integer *,float *,integer *));
extern void C2F(cstringf) __PARAMS((char ***,integer *,integer *,integer *,integer *,integer *));
extern int s_copy __PARAMS((char *,char *, integer, unsigned long int));
extern int C2F(stringc)  __PARAMS((integer *,char ***,integer *));
extern int C2F(stackg)   __PARAMS((integer *));

#ifdef __STDC__
	extern int  Scierror __PARAMS((int iv,char *fmt,...));
#else
	extern int Scierror __PARAMS(()) ;
#endif 

/* calelm */

extern int C2F(entier)  __PARAMS((integer *n, double *d__, integer *s));
extern int C2F(simple)  __PARAMS((integer *n, double *d__, float *s));
extern int C2F(iset)  __PARAMS((integer *n, integer *dx, integer *dy, integer *incy));
extern int C2F(rea2db)  __PARAMS((integer *n, float *dx, integer *incx, double *dy, integer *incy));
extern int C2F(int2db)  __PARAMS((integer *n, integer *dx, integer *incx, double *dy, integer *incy));

/* stack0.c */

extern int C2F(stacki2d)  __PARAMS((integer *n, integer *il1, integer *dl1));
extern int C2F(stackr2d)  __PARAMS((integer *n, integer *sl1, integer *dl1));
extern int C2F(stackc2i)  __PARAMS((integer *n, integer *sl1, integer *il1));

/* cvstr */

extern int C2F(cvstr)  __PARAMS((integer *n, integer *line, char *str, integer *job, unsigned long str_len));
extern int C2F(cvstr1)  __PARAMS((integer *n, integer *line, char *str, integer *job, unsigned long str_len));
extern int C2F(codetoascii)  __PARAMS((integer *n, integer *line, char *str, unsigned long str_len));
extern int C2F(asciitocode)  __PARAMS((integer *n, integer *line, char *str, integer *flag__, unsigned long str_len));
extern integer C2F(getcode)  __PARAMS((unsigned char *mc, unsigned long mc_len));

/* stack1.c */

extern int C2F(getwsmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *ilr, integer *ilrd, unsigned long fname_len));
extern int C2F(getlistwsmat)  __PARAMS((char *fname, integer *topk, integer *spos,integer*lnum, integer *m, integer *n, integer *ilr, integer *ilrd, unsigned long fname_len));

extern char * get_fname __PARAMS(( char *fname,   unsigned long fname_len));
extern int cre_smat_from_str __PARAMS((char *fname,  integer *lw, integer * m, integer * n, char **Str,    unsigned long fname_len));

extern int C2F(isoptlw)  __PARAMS((integer *topk, integer *lw, char *name__, unsigned long name_len));
extern integer C2F(numopt)  __PARAMS((void));
extern int C2F(getlistmat)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getmati)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, int *inlist__, integer *nel, unsigned long fname_len));
extern int C2F(listcremat)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *lrs, integer *lcs, unsigned long fname_len));
extern int C2F(cremat)  __PARAMS((char *fname, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(fakecremat)  __PARAMS((integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc));
extern int C2F(cremati)  __PARAMS((char *fname, integer *stlw, integer *it, integer *m, integer *n, integer *lr, integer *lc, int *flag__, unsigned long fname_len));
extern int C2F(getlistbmat)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getbmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getbmati)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *m, integer *n, integer *lr, int *inlist__, integer *nel, unsigned long fname_len));
extern int C2F(listcrebmat)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *lrs, unsigned long fname_len));
extern int C2F(crebmat)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(fakecrebmat)  __PARAMS(( integer *lw, integer *m, integer *n, integer *lr));
extern int C2F(crebmati)  __PARAMS((char *fname, integer *stlw, integer *m, integer *n, integer *lr, int *flag__, unsigned long fname_len));
extern int C2F(getsparse)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getlistsparse)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getsparsei)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, int *inlist__, integer *nellist, unsigned long fname_len));
extern int C2F(listcresparse)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lrs, integer *lcs, unsigned long fname_len));
extern int C2F(cresparse)  __PARAMS((char *fname, integer *lw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(cresparsei)  __PARAMS((char *fname, integer *stlw, integer *it, integer *m, integer *n, integer *nel, integer *mnel, integer *icol, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getlistvect)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getvect)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getrmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getcmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getrvect)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getscalar)  __PARAMS((char *fname, integer *topk, integer *lw, integer *lr, unsigned long fname_len));
extern int C2F(matsize)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len));
extern int C2F(vectsize)  __PARAMS((char *fname, integer *topk, integer *lw, integer *n, unsigned long fname_len));
extern int C2F(matbsize)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len));
extern int C2F(getsmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len));
extern int C2F(getsimat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len));
extern int C2F(listcresmat)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *nchar, integer *job, integer *ilrs, unsigned long fname_len));
extern int C2F(cresmat)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, integer *nchar, unsigned long fname_len));
extern int C2F(cresmati)  __PARAMS((char *fname, integer *stlw, integer *m, integer *n, integer *nchar, integer *job, integer *lr, integer *sz, unsigned long fname_len));
extern int C2F(cresmat1)  __PARAMS((char *fname, integer *lw, integer *m, integer *nchar, unsigned long fname_len));
extern int C2F(cresmat3)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, integer *nchar, char *buffer, unsigned long fname_len, unsigned long buffer_len));
extern int C2F(cresmat2)  __PARAMS((char *fname, integer *lw, integer *nchar, integer *lr, unsigned long fname_len));
extern int C2F(smatj)  __PARAMS((char *fname, integer *lw, integer *j, unsigned long fname_len));
extern int C2F(lmatj)  __PARAMS((char *fname, integer *lw, integer *j, unsigned long fname_len));
extern int C2F(pmatj)  __PARAMS((char *fname, integer *lw, integer *j, unsigned long fname_len));
extern int C2F(copysmat)  __PARAMS((char *fname, integer *flw, integer *tlw, unsigned long fname_len));
extern int C2F(setsimat)  __PARAMS((char *fname, integer *lw, integer *i__, integer *j, integer *nlr, unsigned long fname_len));
extern int C2F(realmat)  __PARAMS((void));
extern int C2F(crewmat)  __PARAMS((char *fname, integer *lw, integer *m, integer *lr, unsigned long fname_len));
extern int C2F(copyobj)  __PARAMS((char *fname, integer *lw, integer *lwd, unsigned long fname_len));
extern int C2F(vcopyobj)  __PARAMS((char *fname, integer *lw, integer *lwd, unsigned long fname_len));
extern int C2F(swapmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it1, integer *m1, integer *n1, integer *mn1, integer *it2, integer *m2, integer *n2, integer *mn2, unsigned long fname_len));
extern int C2F(insmat)  __PARAMS((integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, integer *lc, integer *lr1, integer *lc1));
extern integer C2F(gettype)  __PARAMS((integer *lw));
extern integer C2F(ogettype)  __PARAMS((integer *lw));
extern int C2F(stackinfo)  __PARAMS((integer *lw, integer *typ));
extern int C2F(allmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, unsigned long fname_len));
extern int C2F(allmatset)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, unsigned long fname_len));
extern int C2F(getilist)  __PARAMS((char *fname, integer *topk, integer *lw, integer *n, integer *i__, integer *ili, unsigned long fname_len));
extern int C2F(objvide)  __PARAMS((char *fname, integer *lw, unsigned long fname_len));
extern int C2F(getexternal)  __PARAMS((char *fname, integer *topk, integer *lw, char *name__, int *type__, S_fp setfun, unsigned long fname_len, unsigned long name_len));
extern int C2F(getpoly)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, char *name__, integer *namel, integer *ilp, integer *lr, integer *lc, unsigned long fname_len, unsigned long name_len));
extern int C2F(crewimat)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(getwimat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(checkval)  __PARAMS((char *fname, integer *ival1, integer *ival2, unsigned long fname_len));
extern int C2F(optvarget)  __PARAMS((char *fname, integer *topk, integer *iel, char *name__, unsigned long fname_len, unsigned long name_len));
extern int C2F(bufstore)  __PARAMS((char *fname, integer *lbuf, integer *lbufi, integer *lbuff, integer *lr, integer *nlr, unsigned long fname_len));
extern int C2F(crestringv)  __PARAMS((char *fname, integer *spos, integer *ilorig, integer *lw, unsigned long fname_len));

extern int C2F(crepointer)  __PARAMS((char *fname, integer *spos, integer *lw, unsigned long fname_len));
extern int C2F(listcrepointer)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw,  integer *lrs,  unsigned long fname_len));
extern int C2F(crepointeri)  __PARAMS((char *fname, integer *stlw, 
				      integer *lr, int *flag__, 
				      unsigned long fname_len));


extern int C2F(lcrestringmatfromc)  __PARAMS((char *fname, integer *spos, integer *numi, integer *stlw, integer *lorig, integer *m, integer *n, unsigned long fname_len));
extern int C2F(crestringmatfromc)  __PARAMS((char *fname, integer *spos, integer *lorig, integer *m, integer *n, unsigned long fname_len));
extern int C2F(crelist)  __PARAMS((integer *slw, integer *ilen, integer *lw));
extern int C2F(cretlist)  __PARAMS((integer *slw, integer *ilen, integer *lw));
extern int C2F(cremlist)  __PARAMS((integer *slw, integer *ilen, integer *lw));
extern int C2F(getlistvectrow)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getvectrow)  __PARAMS((char *fname, integer *topk, integer *spos, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getlistvectcol)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getvectcol)  __PARAMS((char *fname, integer *topk, integer *spos, integer *it, integer *m, integer *n, integer *lr, integer *lc, unsigned long fname_len));
extern int C2F(getlistscalar)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *lr, unsigned long fname_len));
extern int C2F(listcrestring)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *nch, integer *ilrs, unsigned long fname_len));
extern int C2F(crestring)  __PARAMS((char *fname, integer *spos, integer *nchar, integer *ilrs, unsigned long fname_len));
extern int C2F(crestringi)  __PARAMS((char *fname, integer *stlw, integer *nchar, integer *ilrs, unsigned long fname_len));
extern int C2F(getlistsimat)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, unsigned long fname_len));
extern int C2F(getsmati)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *m, integer *n, integer *i__, integer *j, integer *lr, integer *nlr, int *inlist__, integer *nel, unsigned long fname_len));

extern int C2F(getsimati)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *m, integer *n, integer *i, integer *j, integer *lr, integer *nlr,integer *inlist,integer *nel, unsigned long fname_len));
extern int C2F(getonepoly)  __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *md, char *name__, integer *namel, integer *lr, integer *lc, unsigned long fname_len, unsigned long name_len));
extern int C2F(crempointer)  __PARAMS((char *fname, integer *spos, integer *lw, unsigned long fname_len));
extern int C2F(getmpointer)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, unsigned long fname_len));
extern int C2F(creopointer)  __PARAMS((char *fname, integer *spos, integer *lw, unsigned long fname_len));
extern int C2F(getopointer)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, unsigned long fname_len));

extern int C2F(getlistpointer)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *lr,
					 unsigned long fname_len));
extern int C2F(getpointer)  __PARAMS((char *fname, integer *topk, integer *lw, integer *lr, unsigned long fname_len));
extern int C2F(getpointeri)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *lr, 
				      int *inlist__, integer *nel, unsigned long fname_len));



extern int C2F(mspcreate)  __PARAMS((integer *lw, integer *m, integer *n, integer *nzmax, integer *it));
extern int C2F(cresmat4)  __PARAMS((char *fname, integer *lw, integer *m, integer *nchar, integer *lr, unsigned long fname_len));
extern int C2F(credata) __PARAMS((char *fname, integer *lw, integer m, unsigned long fname_len));

int cre_smat_from_str  __PARAMS((char *fname, integer *lw, integer *m, integer *n, char **Str, long unsigned int fname_len));
int cre_listsmat_from_str  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, char **Str, long unsigned int fname_len));

int cre_sparse_from_ptr __PARAMS((char *fname,integer * lw,integer *m, integer *n,SciSparse *Str, unsigned long fname_len ));

int cre_listsparse_from_ptr  __PARAMS((char *fname,integer * lw,integer * numi,integer * stlw,integer *  m,integer * n,
				      SciSparse *M, unsigned long fname_len ));



/**********************************************************************
 * INT MATRICES 
 **********************************************************************/
int C2F(getlistimat) __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len));

int C2F(getimat) __PARAMS((char *fname, integer *topk, integer *lw, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len));

int C2F(getimati) __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *it, integer *m, integer *n, integer *lr, int *inlistx, integer *nel, long unsigned int fname_len));

int C2F(listcreimat) __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *it, integer *m, integer *n, integer *lrs, long unsigned int fname_len));

int C2F(creimat) __PARAMS((char *fname, integer *lw, integer *it, integer *m, integer *n, integer *lr, long unsigned int fname_len));

int C2F(creimati) __PARAMS((char *fname, integer *stlw, integer *it, integer *m, integer *n, integer *lr, int *flagx, long unsigned int fname_len));

/**********************************************************************
 * HANDLE MATRICES 
 **********************************************************************/
extern int C2F(getlisthmat)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lnum, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(gethmat)  __PARAMS((char *fname, integer *topk, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(gethmati)  __PARAMS((char *fname, integer *topk, integer *spos, integer *lw, integer *m, integer *n, integer *lr, int *inlist__, integer *nel, unsigned long fname_len));


extern  int C2F(listcrehmat)  __PARAMS((char *fname, integer *lw, integer *numi, integer *stlw, integer *m, integer *n, integer *lrs, unsigned long fname_len));
extern int C2F(crehmat)  __PARAMS((char *fname, integer *lw, integer *m, integer *n, integer *lr, unsigned long fname_len));
extern int C2F(crehmati)  __PARAMS((char *fname, integer *stlw, integer *m, integer *n, integer *lr, int *flag__, unsigned long fname_len));
#endif 
