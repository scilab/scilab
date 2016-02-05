/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/
/*--------------------------------------------------------------------------*/
// splin
extern int C2F(derivd)(double* x, double* y, double* d, int* n, int* inc, int* type);
extern int C2F(dpchim)(int* n, double* x, double* y, double* d, int* inc);
extern int C2F(splinecub)(double* x, double* y, double* d, int* n, int* type, double* wk1, double* wk2, double* wk3, double* wk4);

// lsq_splin
extern int C2F(spfit)(double* xd, double* yd, double* wd, int* sizeOfXd, double* x, int* sizeOfX, double* y, double* d, double* mwork, int* ierr);

//splin2d
extern int C2F(bicubicsubspline)(double* x, double* y, double* z, int* sizeOfX, int* sizeOfY, double* out, double* p, double* q, double* r, int* type);
extern int C2F(bicubicspline)(double *x, double* y, double* u, int* sizeOfX, int* sizeOfY, double * out, double* p, double* q, double* r, double* A_d, double* A_sd, double* d, double* ll, double* qdu, double* u_temp, int* type);

//splin3d
extern int C2F(db3ink)(double* x, int* nx, double* y, int* ny, double* z, int* nz, double* v, int* mv, int* nv, int* order1, int* order2, int* order3, double* outX, double* outY, double* outZ, double* coef, double* work, int* flag);

//bsplin3val
extern double C2F(db3val)(double* xval, double* yval, double* zval, int* idx, int* idy, int* idz, double* tx, double* ty, double* tz, int* nx, int* ny, int* nz, int* kx, int* ky, int* kz, double* bcoef, double* work);

// interp
extern int C2F(evalpwhermite)(double* t, double* st, double* dst, double* ddst, double* dddst, int* sizeOfT, double* x, double* y, double* d, int* sizeOfX, int* outmode);

// interp2d
extern int C2F(bicubicinterp)(double* x, double* y, double* c, int* sizeOfX, int* sizeOfY, double* xp, double* yp, double* zp, int* sizeOfXp, int* iType);

extern int C2F(bicubicinterpwithgrad)(double* x, double* y, double* c, int* sizeOfX, int* sizeOfY, double* xp, double* yp, double* zp, double* zpdx, double* zpdy, int* sizeOfXp, int* iType);

extern int C2F(bicubicinterpwithgradandhes)(double* x, double* y, double* c, int* sizeOfX, int* sizeOfY, double* xp, double* yp, double* zp, double* zpdx, double* zpdy, double* d2zdx2p, double* d2zdxyp, double* d2zdy2p, int* sizeOfXp, int* iType);

// interp3d
extern int C2F(driverdb3val)(double* xp, double* yp, double* zp, double* fp, int* sizeOfXp, double* x, double* y, double* z, int* sizeOfX, int* sizeOfY, int* sizeOfZ, int* kx, int* ky, int* kz, double* bcoef, double* work, double* xmin, double* xmax, double* ymin, double* ymax, double* zmin, double* zmax, int* iType);

extern int C2F(driverdb3valwithgrad)(double* xp, double* yp, double* zp, double* fp, double* fpdx, double* fpdy, double* fpdz, int* sizeOfXp, double* x, double* y, double* z, int* sizeOfX, int* sizeOfY, int* sizeOfZ, int* kx, int* ky, int* kz, double* bcoef, double* work, double* xmin, double* xmax, double* ymin, double* ymax, double* zmin, double* zmax, int* iType);

// cshep2
extern int C2F(cshep2) (int* n, double* x, double* y, double* z, int* nc, int* nw, int* nr, int* lcell, int* lnext, double* xmin, double* xmax, double* dx, double* dy, double* rmax, double* rw, double* a, int* ier);

// eval_cshep2
extern double C2F(cs2val)(double* px, double* py, int* n, double* x, double* y, double* f, int* nr, int* lcell, int* lnext, double* xmin, double* ymin, double* dx, double* dy, double* rmax, double* rw, double* a);
extern int C2F(cs2grd)(double* px, double* py, int* n, double* x, double* y, double* f, int* nr, int* lcell, int* lnext, double* xmin, double* ymin, double* dx, double* dy, double* rmax, double* rw, double* a, double* c, double* cx, double* cy, int* ier);
extern int C2F(cs2hes)(double* px, double* py, int* n, double* x, double* y, double* f, int* nr, int* lcell, int* lnext, double* xmin, double* ymin, double* dx, double* dy, double* rmax, double* rw, double* a, double* c, double* cx, double* cy, double* cxx, double* cxy, double* cyy, int* ier);

