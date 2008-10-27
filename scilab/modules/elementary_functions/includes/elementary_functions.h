/*
 * Scilab( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright(C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __CALELM_H
#define  __CALELM_H

#include "machine.h" /* int , C2F */

typedef int(*I_fp)();

int C2F(vfinite)(int *n, double *v);

extern int C2F(wsort)(double *countr, double *counti, int *n, int *index, I_fp test);

extern int C2F(rcsort)(I_fp test, int *isz, int *iptr, int *iv, int *n, int *index);

extern double C2F(arcosh)(double *x);
extern double C2F(arsinh)(double *x);
extern int C2F(calerf)(double *arg, double *result, int *jint);
extern double C2F(derf)(double *x);
extern double C2F(derfc)(double *x);
extern double C2F(derfcx)(double *x);
extern int C2F(dgefa)(double *a, int *lda, int *n, int *ipvt, int *info);
extern int C2F(ccopy)(int *n, char *dx, int *incx, char *dy, int *incy, int dx_len, int dy_len);
extern double C2F(coshin)(double *x);
extern int C2F(cusum)(int *n, double *w);
extern int C2F(dadd)(int *n, double *dx, int *incx, double *dy, int *incy);
extern int C2F(dad)(double *a, int *na, int *i1, int *i2, int *j1, int *j2, double *r__, int *isw);
extern int C2F(ddif)(int *n, double *a, int *na, double *b, int *nb);
extern int C2F(ddpow1)(int *n, double *v, int *iv, double *p, int *ip, double *rr, double *ri, int *ir, int *ierr, int *iscmpl);
extern int C2F(ddpowe)(double *v, double *p, double *rr, double *ri, int *ierr, int *iscmpl);
extern int C2F(ddpow)(int *n, double *vr, double *vi, int *iv, double *dpow, int *ierr, int *iscmpl);
extern int C2F(ddrdiv)(double *a, int *ia, double *b, int *ib, double *r__, int *ir, int *n, int *ierr);
extern int C2F(dipowe)(double *v, int *p, double *r__, int *ierr);
extern int C2F(dipow)(int *n, double *v, int *iv, int *ipow, int *ierr);
extern int C2F(dlblks)(char *name__, int *nbc, int name_len);
extern double C2F(dlgama)(double *x);
extern int C2F(dmcopy)(double *a, int *na, double *b, int *nb, int *m, int *n);
extern int C2F(dmmul1)(double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n);
extern int C2F(dmmul)(double *a, int *na, double *b, int *nb, double *c__, int *nc, int *l, int *m, int *n);
extern int C2F(dmprod)(int *flag__, double *a, int *na, int *m, int *n, double *v, int *nv);
extern int C2F(dmsum)(int *flag__, double *a, int *na, int *m, int *n, double *v, int *nv);
extern int C2F(drdiv)(double *a, int *ia, double *b, int *ib, double *r__, int *ir, int *n, int *ierr);
extern int C2F(dset)(int *n, double *dx, double *dy, int *incy);
extern int C2F(dsort)(double *count, int *n, int *index);
extern double C2F(dsum)(int *n, double *dx, int *incx);
extern int C2F(dtild)(int *n, double *x, int *incx);
extern int C2F(dvmul)(int *n, double *dx, int *incx, double *dy, int *incy);
extern int C2F(dwdiv)(double *ar, double *br, double *bi, double *cr, double *ci, int *ierr);
extern int C2F(dwpow1)(int *n, double *v, int *iv, double *pr, double *pi, int *ip, double *rr, double *ri, int *ir, int *ierr);
extern int C2F(dwpowe)(double *v, double *pr, double *pi, double *rr, double *ri, int *ierr);
extern int C2F(dwpow)(int *n, double *vr, double *vi, int *iv, double *powr, double *powi, int *ierr);
extern int C2F(dwrdiv)(double *ar, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr);
extern int C2F(franck)(double *a, int *na, int *n, int *job);
extern double C2F(dgamma)(double *x);
extern int C2F(gdcp2i)(int *n, int *itab, int *m);
extern int C2F(hilber)(double *a, int *lda, int *n);

extern int C2F(idmax)(int *n, double *dx, int *incx);
extern int C2F(idmin)(int *n, double *dx, int *incx);
extern int C2F(imcopy)(int *a, int *na, int *b, int *nb, int *m, int *n);
extern double C2F(infinity)(double *x);
extern int C2F(intp)(double *x, double *xd, double *yd, int *n, int *nc, double *y);
extern int C2F(isort)(int *count, int *n, int *index);
extern int C2F(isova0)(double *a, int *lda, int *m, int *n, double *path, int *kpath, int *ir, int *ic, int *dir, int *pend, int *h__, int *v, double *c__);
extern int C2F(isoval)(double *a, int *lda, int *m, int *n, double *c__, double *path, int *npath, int *maxp, int *ierr, int *iw, int *job);
extern int C2F(ivimp)(int *i1, int *i2, int *pas, int *iv);
extern int C2F(iwamax)(int *n, double *xr, double *xi, int *incx);
extern int C2F(kronc)(double *ar, double *ai, int *ia, int *ma, int *na, double *br, double *bi, int *ib, int *mb, int *nb, double *pkr, double *pki, int *ik);
extern int C2F(kronr)(double *a, int *ia, int *ma, int *na, double *b, int *ib, int *mb, int *nb, double *pk, int *ik);
extern int C2F(lnblnk)(char *str, int str_len);
extern int C2F(magic)(double *a, int *lda, int *n);
extern int C2F(mtran)(double *a, int *na, double *b, int *nb, int *m, int *n);
extern double C2F(nearfloat)(double *x, double *dir);
extern double C2F(psi)(double *xx);
extern double C2F(pythag)(double *a, double *b);
extern int C2F(rat)(double *x, double *eps, int *n, int *d__, int *fail);
extern int C2F(ribesl)(double *x, double *alpha, int *nb, int *ize, double *b, int *ncalc);
extern int C2F(rjbesl)(double *x, double *alpha, int *nb, double *b, int *ncalc);
extern int C2F(rkbesl)(double *x, double *alpha, int *nb, int *ize, double *bk, int *ncalc);
/*extern double C2F(round)(double *x1);*/
extern int C2F(rybesl)(double *x, double *alpha, int *nb, double *by, int *ncalc);
extern double C2F(urand)(int *iy);
extern int C2F(vpythag)(int *n, double *xr, double *xi);
extern int C2F(wacos)(double *zr, double *zi, double *ar, double *ai);
extern int C2F(wasin)(double *zr, double *zi, double *ar, double *ai);
extern double C2F(wasum)(int *n, double *xr, double *xi, int *incx);
extern int C2F(watan)(double *xr, double *xi, double *yr, double *yi);
extern double C2F(lnp1m1)(double *s);
extern double C2F(logp1)(double *x);
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
extern int C2F(waxpy)(int *n, double *sr, double *si, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy);
extern int C2F(wcopy)(int *n, double *dxr, double *dxi, int *incx, double *dyr, double *dyi, int *incy);
extern int C2F(wddiv)(double *ar, double *ai, double *br, double *cr, double *ci, int *ierr);
extern int C2F(wdiv)(double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern double C2F(wdotci)(int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy);
extern double C2F(wdotcr)(int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy);
extern int C2F(wdpow1)(int *n, double *vr, double *vi, int *iv, double *p, int *ip, double *rr, double *ri, int *ir, int *ierr);
extern int C2F(wdpowe)(double *vr, double *vi, double *p, double *rr, double *ri, int *ierr);
extern int C2F(wdpow)(int *n, double *vr, double *vi, int *iv, double *dpow, int *ierr);
extern int C2F(wdrdiv)(double *ar, double *ai, int *ia, double *br, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr);
extern int C2F(wipowe)(double *vr, double *vi, int *p, double *rr, double *ri, int *ierr);
extern int C2F(wipow)(int *n, double *vr, double *vi, int *iv, int *ipow, int *ierr);
extern int C2F(wlog)(double *xr, double *xi, double *yr, double *yi);
extern int C2F(wmmul)(double *ar, double *ai, int *na, double *br, double *bi, int *nb, double *cr, double *ci, int *nc, int *l, int *m, int *n);
extern int C2F(wmprod)(int *flag__, double *ar, double *ai, int *na, int *m, int *n, double *vr, double *vi, int *nv);
extern int C2F(wmsum)(int *flag__, double *ar, double *ai, int *na, int *m, int *n, double *vr, double *vi, int *nv);
extern int C2F(wmul)(double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern int C2F(wrscal)(int *n, double *s, double *xr, double *xi, int *incx);
extern int C2F(wscal)(int *n, double *sr, double *si, double *xr, double *xi, int *incx);
extern int C2F(wsign)(double *xr, double *xi, double *yr, double *yi, double *zr, double *zi);
extern int C2F(rptest)(double *r1, double *i1, double *r2, double *i2);
extern int C2F(modtest)(double *r1, double *i1, double *r2, double *i2);
extern int C2F(wsqrt)(double *xr, double *xi, double *yr, double *yi);
extern int C2F(wswap)(int *n, double *xr, double *xi, int *incx, double *yr, double *yi, int *incy);
extern int C2F(wtan)(double *xr, double *xi, double *yr, double *yi);
extern int C2F(wvmul)(int *n, double *dxr, double *dxi, int *incx, double *dyr, double *dyi, int *incy);
extern int C2F(wwdiv)(double *ar, double *ai, double *br, double *bi, double *cr, double *ci, int *ierr);
extern int C2F(wwpow1)(int *n, double *vr, double *vi, int *iv, double *pr, double *pi, int *ip, double *rr, double *ri, int *ir, int *ierr);
extern int C2F(wwpowe)(double *vr, double *vi, double *pr, double *pi, double *rr, double *ri, int *ierr);
extern int C2F(wwpow)(int *n, double *vr, double *vi, int *iv, double *powr, double *powi, int *ierr);
extern int C2F(wwrdiv)(double *ar, double *ai, int *ia, double *br, double *bi, int *ib, double *rr, double *ri, int *ir, int *n, int *ierr);
#endif /** SCI_   **/
