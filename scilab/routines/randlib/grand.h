#ifndef SCI_GRAND
#define SCI_GRAND

extern int C2F(setgmn) __PARAMS((double *meanv, double *covm, int *ldcovm, int *p, double *parm, int *ierr));
extern double C2F(genbet) __PARAMS((double *aa, double *bb));
extern double C2F(genchi) __PARAMS((double *df));
extern double C2F(genexp) __PARAMS((double *av));
extern double C2F(genf) __PARAMS((double *dfn, double *dfd));
extern double C2F(gengam) __PARAMS((double *a, double *r__));
extern double C2F(gennch) __PARAMS((double *df, double *xnonc));
extern double C2F(gennf) __PARAMS((double *dfn, double *dfd, double *xnonc));
extern double C2F(gennor) __PARAMS((double *av, double *sd));
extern double C2F(sdot) __PARAMS((int *n, double *sx, int *incx, double *sy, int *incy));
extern double C2F(sexpo) __PARAMS((void));
extern double C2F(sgamma) __PARAMS((double *a));
extern double C2F(snorm) __PARAMS((void));
extern double igngeom(double p);
extern int C2F(genmn) __PARAMS((double *parm, double *x, double *work));
extern int C2F(genmul) __PARAMS((int *n, double *p, int *ncat, int *ix));
extern int C2F(genprm) __PARAMS((double *array, int *larray));
extern int C2F(ignbin) __PARAMS((int *n, double *pp));
extern int C2F(ignnbn) __PARAMS((int *n, double *p));
extern int C2F(ignpoi) __PARAMS((double *mu));
extern int C2F(phrtsd) __PARAMS((char *phrase, int *phrasel, int *seed1, int *seed2, int phrase_len));
extern int C2F(spofa) __PARAMS((double *a, int *lda, int *n, int *info));

#endif /** SCI_GRAND   **/

