#ifndef SCI_
#define SCI_

extern double C2F(mnddot) __PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern double C2F(mpadd) __PARAMS((double *p, double *q));
extern double C2F(mpaddmul) __PARAMS((double *r__, double *p, double *q));
extern double C2F(mpddot) __PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern double C2F(mpkarp) __PARAMS((int *ma, int *na, double *a, int *nela, int *inda, int *index));
extern int C2F(algcode) __PARAMS((int *l));
extern int C2F(algtype) __PARAMS((int *m1, int *m2));
extern int C2F(commons) __PARAMS((double *zero));
extern int C2F(mdadd) __PARAMS((int *m, int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(mndadd) __PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(mndmmul) __PARAMS((double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n));
extern int C2F(mndsmsp) __PARAMS((int *nrb, int *ncb, int *nca, double *b, int *mrb, double *a, int *nela, int *inda, double *c__, int *mrc, double *mpzero));
extern int C2F(mnwmmul) __PARAMS((double *ar, double *ai, int *na, double *br, double *bi, int *nb, double *cr, double *ci, int *nc, int *l, int *m, int *n));
extern int C2F(mpchsgn) __PARAMS((int *n, double *da, double *dx, int *incx));
extern int C2F(mpdadd) __PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(mpddif) __PARAMS((int *n, double *dx, int *incx, double *dy, int *incy, double *v));
extern int C2F(mpddpow) __PARAMS((int *n, double *vr, double *vi, int *iv, double *dpow, int *ierr, int *iscmpl));
extern int C2F(mpddpow1) __PARAMS((int *n, double *v, int *iv, double *p, int *ip, double *rr, double *ri, int *ir, int *ierr, int *iscmpl));
extern int C2F(mpddpowe) __PARAMS((double *v, double *p, double *rr, double *ri, int *ierr, int *iscmpl));
extern int C2F(mpdipowe) __PARAMS((double *v, double *p, double *r__, int *ierr));
extern int C2F(mpdmmul) __PARAMS((double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n));
extern int C2F(mpdscal) __PARAMS((int *n, double *da, double *dx, int *incx));
extern int C2F(mpdsmsp) __PARAMS((int *nrb, int *ncb, int *nca, double *b, int *mrb, double *a, int *nela, int *inda, double *c__, int *mrc, double *mpzero));
extern int C2F(mpdspasp) __PARAMS((int *nr, int *nc, double *a, int *nela, int *inda, double *b, int *nelb, int *indb, double *c__, int *nelc, int *indc, int *ierr));
extern int C2F(mpdspdrsp) __PARAMS((int *p, int *q, int *r__, double *a, int *nela, int *inda, double *b, int *nelb, int *indb, int *ia, double *at, int *iat, int *indat, double *v, double *c__, int *mrc, double *mpzero));
extern int C2F(mpdspful) __PARAMS((int *ma, int *na, double *a, int *nela, int *inda, double *r__, int *mpzero));
extern int C2F(mpdspis) __PARAMS((int *ma, int *na, double *a, int *nela, int *inda, int *i__, int *ni, int *j, int *nj, int *mb, int *nb, double *b, int *mr, int *nr, double *r__, int *nelr, int *indr, int *ierr));
extern int C2F(mpdspmas) __PARAMS((int *nr, int *nc, double *a, int *nela, int *inda, double *b, double *c__, double *mpzero, double *d__, int *indd, double *ao));
extern int C2F(mpdspms) __PARAMS((int *nra, int *nca, int *ncb, double *a, int *nela, int *inda, double *b, int *mrb, double *c__, int *mrc, double *mpzero));
extern int C2F(mpdspmsp) __PARAMS((int *p, int *q, int *r__, double *a, int *nela, int *inda, double *b, int *nelb, int *indb, double *c__, int *nelc, int *indc, int *ib, int *ic, double *x, int *xb, int *ierr));
extern int C2F(mpkronr) __PARAMS((double *a, int *ia, int *ma, int *na, double *b, int *ib, int *mb, int *nb, double *pk, int *ik));
extern int C2F(mpwmmul) __PARAMS((double *ar, double *ai, int *na, double *br, double *bi, int *nb, double *cr, double *ci, int *nc, int *l, int *m, int *n));
extern int C2F(mpwmul) __PARAMS((double *ar, double *ai, double *br, double *bi, double *cr, double *ci));
extern int C2F(mpwscal) __PARAMS((int *n, double *sr, double *si, double *xr, double *xi, int *incx));
extern int C2F(mpwspful) __PARAMS((int *ma, int *na, double *ar, double *ai, int *nela, int *inda, double *rr, double *ri));
extern int C2F(mpwspis) __PARAMS((int *ma, int *na, double *ar, double *ai, int *nela, int *inda, int *i__, int *ni, int *j, int *nj, int *mb, int *nb, double *br, double *bi, int *mr, int *nr, double *rr, double *ri, int *nelr, int *indr, int *ierr, int *ita, int *itb));
extern int C2F(talg_p_) __PARAMS((double *x, int *nx, int *mm, int *nn, int *maxc, int *mode, int *ll, int *lunit, char *cw, int *iw, int cw_len));
extern int C2F(typecode) __PARAMS((int *l));

#endif /** SCI_   **/
