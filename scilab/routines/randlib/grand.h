#ifndef SCI_GRAND
#define SCI_GRAND

extern double C2F(genbet) __PARAMS((double *aa, double *bb));
extern double C2F(genchi) __PARAMS((double *df));
extern double C2F(genexp) __PARAMS((double *av));
extern double C2F(genf) __PARAMS((double *dfn, double *dfd));
extern double C2F(gengam) __PARAMS((double *a, double *r__));
extern double C2F(gennch) __PARAMS((double *df, double *xnonc));
extern double C2F(gennf) __PARAMS((double *dfn, double *dfd, double *xnonc));
extern double C2F(gennor) __PARAMS((double *av, double *sd));
extern double C2F(genunf) __PARAMS((double *low, double *high));
extern double C2F(ranf) __PARAMS((void));
extern double C2F(sdot) __PARAMS((int *n, double *sx, int *incx, double *sy, int *incy));
extern double C2F(sexpo) __PARAMS((void));
extern double C2F(sgamma) __PARAMS((double *a));
extern double C2F(snorm) __PARAMS((void));
extern int C2F(advnst) __PARAMS((int *k));
extern int C2F(genmn) __PARAMS((double *parm, double *x, double *work));
extern int C2F(genmul) __PARAMS((int *n, double *p, int *ncat, int *ix));
extern int C2F(genprm) __PARAMS((int *iarray, int *larray));
extern int C2F(getcgn) __PARAMS((int *g));
extern int C2F(getsd) __PARAMS((int *iseed1, int *iseed2));
extern int C2F(ignbin) __PARAMS((int *n, double *pp));
extern int C2F(ignlgi) __PARAMS((void));
extern int C2F(ignnbn) __PARAMS((int *n, double *p));
extern int C2F(ignpoi) __PARAMS((double *mu));
extern int C2F(ignuin) __PARAMS((int *low, int *high, int *ierr));
extern int C2F(initgn) __PARAMS((int *isdtyp));
extern int C2F(inrgcm) __PARAMS((void));
extern int C2F(lennob) __PARAMS((char *string, int string_len));
extern int C2F(mltmod) __PARAMS((int *a, int *s, int *m, int *ierr));
extern int C2F(phrtsd) __PARAMS((char *phrase, int *phrasel, int *seed1, int *seed2, int phrase_len));
extern int C2F(qrgnin) __PARAMS((void));
extern int C2F(qrgnsn) __PARAMS((int *qvalue));
extern int C2F(rgnqsd) __PARAMS((int *qssd));
extern int C2F(setall) __PARAMS((int *iseed1, int *iseed2));
extern int C2F(setant) __PARAMS((int *qvalue));
extern int C2F(setcgn) __PARAMS((int *g));
extern int C2F(setgmn) __PARAMS((double *meanv, double *covm, int *ldcovm, int *p, double *parm, int *ierr));
extern int C2F(setsd) __PARAMS((int *iseed1, int *iseed2));
extern int C2F(spofa) __PARAMS((double *a, int *lda, int *n, int *info));

#endif /** SCI_GRAND   **/

