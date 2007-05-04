#ifndef __CALELM_H 
#define  __CALELM_H 

typedef integer (*I_fp)();

int C2F(vfinite)  _PARAMS((int *n, double *v));

extern int C2F(wsort) _PARAMS((double *countr, double *counti, int *n, int *index, I_fp test));

extern int C2F(rcsort) _PARAMS((I_fp test, int *isz, int *iptr, int *iv, int *n, int *index));

extern double C2F(arcosh) _PARAMS((double *x));
extern double C2F(arsinh) _PARAMS((double *x));
extern int C2F(calerf) _PARAMS((double *arg, double *result, int *jint));
extern double C2F(derf) _PARAMS((double *x));
extern double C2F(derfc) _PARAMS((double *x));
extern double C2F(derfcx) _PARAMS((double *x));
extern int C2F(ccopy) _PARAMS((int *n, char *dx, int *incx, char *dy, int *incy, int dx_len, int dy_len));
extern double C2F(coshin) _PARAMS((double *x));
extern int C2F(cupro) _PARAMS((int *n, double *w));
extern int C2F(cuproi) _PARAMS((int *n, double *wr, double *wi));
extern int C2F(cusum) _PARAMS((int *n, double *w));
extern int C2F(dadd) _PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(dad) _PARAMS((double *a, int *na, int *i1, int *i2, int *j1, int *j2, double *r__, int *isw));
/* extern double C2F(dcabs1) _PARAMS((doublecomplex *z__));*/
extern int C2F(ddif) _PARAMS((int *n, double *a, int *na, double *b, int *nb));
extern int C2F(ddpow1) _PARAMS((int *n, double *v, int *iv, double *p, int *ip, double *rr, double *ri, int *ir, int *ierr, int *iscmpl));
extern int C2F(ddpowe) _PARAMS((double *v, double *p, double *rr, double *ri, int *ierr, int *iscmpl));
extern int C2F(ddpow) _PARAMS((int *n, double *vr, double *vi, int *iv, double *dpow, int *ierr, int *iscmpl));
extern int C2F(ddrdiv) _PARAMS((double *a, int *ia, double *b, int *ib, double *r__, int *ir, int *n, int *ierr));
extern int C2F(dipowe) _PARAMS((double *v, int *p, double *r__, int *ierr));
extern int C2F(dipow) _PARAMS((int *n, double *v, int *iv, int *ipow, int *ierr));
extern int C2F(dlblks) _PARAMS((char *name__, int *nbc, int name_len));
extern double C2F(dlgama) _PARAMS((double *x));
extern int C2F(dmcopy) _PARAMS((double *a, int *na, double *b, int *nb, int *m, int *n));
extern int C2F(dmmul1) _PARAMS((double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n));
extern int C2F(dmmul) _PARAMS((double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n));
extern int C2F(dmprod) _PARAMS((int *flag__, double *a, int *na, int *m, int *n, double *v, int *nv));
extern int C2F(dmsum) _PARAMS((int *flag__, double *a, int *na, int *m, int *n, double *v, int *nv));
extern int C2F(drdiv) _PARAMS((double *a, int *ia, double *b, int *ib, double *r__, int *ir, int *n, int *ierr));
extern int C2F(dset) _PARAMS((int *n, double *dx, double *dy, int *incy));
extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
extern double C2F(dsum) _PARAMS((int *n, double *dx, int *incx));
extern int C2F(dtild) _PARAMS((int *n, double *x, int *incx));
extern int C2F(dvmul) _PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(dwdiv) _PARAMS((double *ar, double *br, double *bi, double *cr, double *ci, int *ierr));
extern int C2F(dwpow1) _PARAMS((int *n, double *v, int *iv, double *pr, double *pi, int *ip, double *rr, double *ri, int *ir, int *ierr));
extern int C2F(dwpowe) _PARAMS((double *v, double *pr, double *pi, double *rr, double *ri, int *ierr));
extern int C2F(dwpow) _PARAMS((int *n, double *vr, double *vi, int *iv, double *powr, double *powi, int *ierr));
extern int C2F(dwrdiv) _PARAMS((double *ar, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr));
extern int C2F(entier) _PARAMS((int *n, double *d__, int *s));
extern int C2F(franck) _PARAMS((double *a, int *na, int *n, int *job));
extern double C2F(dgamma) _PARAMS((double *x));
extern int C2F(gdcp2i) _PARAMS((int *n, int *itab, int *m));
extern int C2F(hilber) _PARAMS((double *a, int *lda, int *n));
extern int C2F(icopy) _PARAMS((int *n, int *dx, int *incx, int *dy, int *incy));
extern int C2F(idmax) _PARAMS((int *n, double *dx, int *incx));
extern int C2F(idmin) _PARAMS((int *n, double *dx, int *incx));
extern int C2F(imcopy) _PARAMS((int *a, int *na, int *b, int *nb, int *m, int *n));
extern double C2F(infinity) _PARAMS((double *x));
extern int C2F(int2db) _PARAMS((int *n, int *dx, int *incx, double *dy, int *incy));
extern int C2F(intp) _PARAMS((double *x, double *xd, double *yd, int *n, int *nc, double *y));
extern int C2F(iset) _PARAMS((int *n, int *dx, int *dy, int *incy));
extern int C2F(isort) _PARAMS((int *count, int *n, int *index));
extern int C2F(isova0) _PARAMS((double *a, int *lda, int *m, int *n, double *path, int *kpath, int *ir, int *ic, int *dir, int *pend, int *h__, int *v, double *c__));
extern int C2F(isoval) _PARAMS((double *a, int *lda, int *m, int *n, double *c__, double *path, int *npath, int *maxp, int *ierr, int *iw, int *job));
extern int C2F(ivimp) _PARAMS((int *i1, int *i2, int *pas, int *iv));
extern int C2F(iwamax) _PARAMS((int *n, double *xr, double *xi, int *incx));
extern int C2F(kronc) _PARAMS((double *ar, double *ai, int *ia, int *ma, int *na, double *br, double *bi, int *ib, int *mb, int *nb, double *pkr, double *pki, int *ik));
extern int C2F(kronr) _PARAMS((double *a, int *ia, int *ma, int *na, double *b, int *ib, int *mb, int *nb, double *pk, int *ik));
extern int C2F(lnblnk) _PARAMS((char *str, int str_len));
extern int C2F(magic) _PARAMS((double *a, int *lda, int *n));
extern int C2F(mtran) _PARAMS((double *a, int *na, double *b, int *nb, int *m, int *n));
extern double C2F(nearfloat) _PARAMS((double *x, double *dir));
extern double C2F(psi) _PARAMS((double *xx));
extern double C2F(pythag) _PARAMS((double *a, double *b));
extern int C2F(rat) _PARAMS((double *x, double *eps, int *n, int *d__, int *fail));
extern int C2F(rcopy) _PARAMS((int *n, float *dx, int *incx, float *dy, int *incy));
extern int C2F(rea2db) _PARAMS((int *n, float *dx, int *incx, double *dy, int *incy));
extern int C2F(ribesl) _PARAMS((double *x, double *alpha, int *nb, int *ize, double *b, int *ncalc));
extern int C2F(rjbesl) _PARAMS((double *x, double *alpha, int *nb, double *b, int *ncalc));
extern int C2F(rkbesl) _PARAMS((double *x, double *alpha, int *nb, int *ize, double *bk, int *ncalc));
/*extern double C2F(round) _PARAMS((double *x1));*/
extern int C2F(rybesl) _PARAMS((double *x, double *alpha, int *nb, double *by, int *ncalc));
extern int C2F(simple) _PARAMS((int *n, double *d__, float *s));
extern int C2F(unsfdcopy) _PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));
extern int C2F(scidcopy) _PARAMS((int *n, double *dx, int *incx, double *dy, int *incy));

extern double C2F(urand) _PARAMS((int *iy));
extern int C2F(vpythag) _PARAMS((int *n, double *xr, double *xi));
extern int C2F(wacos) _PARAMS((double *zr, double *zi, double *ar, double *ai));
extern int C2F(wasin) _PARAMS((double *zr, double *zi, double *ar, double *ai));
extern double C2F(wasum) _PARAMS((int *n, double *xr, double *xi, int *incx));
extern int C2F(watan) _PARAMS((double *xr, double *xi, double *yr, double *yi));
extern double C2F(lnp1m1) _PARAMS((double *s));
extern double C2F(logp1) _PARAMS((double *x));
/* comlen stack_ 16 */
/* comlen vstk_ 65568 */
/* comlen recu_ 8212 */
/* comlen iop_ 65620 */
/* comlen errgst_ 24 */
/* comlen com_ 68 */
/* comlen cha1_ 4222 */
/* comlen dbg_ 976 */
/* comlen adre_ 260 */
/* comlen intersci_ 1924 */
extern int C2F(waxpy) _PARAMS((int *n, double *sr, double *si, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy));
extern int C2F(wcopy) _PARAMS((int *n, double *dxr, double *dxi, int *incx, double *dyr, double *dyi, int *incy));
extern int C2F(wddiv) _PARAMS((double *ar, double *ai, double *br, double *cr, double *ci, int *ierr));
extern int C2F(wdiv) _PARAMS((double *ar, double *ai, double *br, double *bi, double *cr, double *ci));
extern double C2F(wdotci) _PARAMS((int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy));
extern double C2F(wdotcr) _PARAMS((int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy));
extern int C2F(wdpow1) _PARAMS((int *n, double *vr, double *vi, int *iv, double *p, int *ip, double *rr, double *ri, int *ir, int *ierr));
extern int C2F(wdpowe) _PARAMS((double *vr, double *vi, double *p, double *rr, double *ri, int *ierr));
extern int C2F(wdpow) _PARAMS((int *n, double *vr, double *vi, int *iv, double *dpow, int *ierr));
extern int C2F(wdrdiv) _PARAMS((double *ar, double *ai, int *ia, double *br, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr));
extern int C2F(wipowe) _PARAMS((double *vr, double *vi, int *p, double *rr, double *ri, int *ierr));
extern int C2F(wipow) _PARAMS((int *n, double *vr, double *vi, int *iv, int *ipow, int *ierr));
extern int C2F(wlog) _PARAMS((double *xr, double *xi, double *yr, double *yi));
extern int C2F(wmmul) _PARAMS((double *ar, double *ai, int *na, double *br, double *bi, int *nb, double *cr, double *ci, int *nc, int *l, int *m, int *n));
extern int C2F(wmprod) _PARAMS((int *flag__, double *ar, double *ai, int *na, int *m, int *n, double *vr, double *vi, int *nv));
extern int C2F(wmsum) _PARAMS((int *flag__, double *ar, double *ai, int *na, int *m, int *n, double *vr, double *vi, int *nv));
extern int C2F(wmul) _PARAMS((double *ar, double *ai, double *br, double *bi, double *cr, double *ci));
extern double C2F(wnrm2) _PARAMS((int *n, double *xr, double *xi, int *incx));
extern int C2F(wrscal) _PARAMS((int *n, double *s, double *xr, double *xi, int *incx));
extern int C2F(wscal) _PARAMS((int *n, double *sr, double *si, double *xr, double *xi, int *incx));
extern int C2F(wsign) _PARAMS((double *xr, double *xi, double *yr, double *yi, double *zr, double *zi));
extern int C2F(rptest) _PARAMS((double *r1, double *i1, double *r2, double *i2));
extern int C2F(modtest) _PARAMS((double *r1, double *i1, double *r2, double *i2));
extern int C2F(wsqrt) _PARAMS((double *xr, double *xi, double *yr, double *yi));
extern int C2F(wswap) _PARAMS((int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy));
extern int C2F(wtan) _PARAMS((double *xr, double *xi, double *yr, double *yi));
extern int C2F(wvmul) _PARAMS((int *n, double *dxr, double *dxi, int *incx, double *dyr, double *dyi, int *incy));
extern int C2F(wwdiv) _PARAMS((double *ar, double *ai, double *br, double *bi, double *cr, double *ci, int *ierr));
extern int C2F(wwpow1) _PARAMS((int *n, double *vr, double *vi, int *iv, double *pr, double *pi, int *ip, double *rr, double *ri, int *ir, int *ierr));
extern int C2F(wwpowe) _PARAMS((double *vr, double *vi, double *pr, double *pi, double *rr, double *ri, int *ierr));
extern int C2F(wwpow) _PARAMS((int *n, double *vr, double *vi, int *iv, double *powr, double *powi, int *ierr));
extern int C2F(wwrdiv) _PARAMS((double *ar, double *ai, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr));
#endif /** SCI_   **/
