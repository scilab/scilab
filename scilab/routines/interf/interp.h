/* 
 *  headers of the (Fortran and C) functions called 
 *  by intinterp.c
 */
     
/* from somespline.f */
int C2F(derivd) (double *x, double *y, double *d, int *n, int *inc, int *type);

int C2F(dpchim) (int *n, double *x, double *y, double *d, int *inc);

int C2F(splinecub) (double *x, double *y, double *d, int *n, int *type, 
		    double *wk1, double *wk2, double *wk3, double *wk4);

int C2F(evalpwhermite) (double *t, double *st, double *dst, double *ddst, double *dddst,
			int *m, double *x, double *y, double *d, int *n, int *outmode);

int C2F(bicubicsubspline)(double *x, double *y, double *z, int *nx, int *ny,
			  double *C, double *p, double *q, double *r, int *spline_type);

int C2F(bicubicspline)(double *x, double *y, double *u, int *nx, int *ny,
                       double *C, double *p, double *q, double *r,
		       double *A_d, double *A_sd, double *d, double *ll,
                       double *qdu, double *u_temp, int *spline_type);

int C2F(bicubicinterp)(double *x, double *y, double *C, int *nx, int *ny, 
		       double *x_eval, double *y_eval, double *z_eval, int *m,
                       int *outmode);

/* from dspfit.f */
int C2F(spfit)(double *xp, double *yp, double *wp, int *m, double *x, int *l,
	       double *y, double *d, double *wk, int *ierr);

/* from someinterp.c */
void nlinear_interp(double **x , double val[], int dim[], int n,
		    double **xp, double yp[], int np, int outmode, 
		    double u[], double v[], int ad[], int k[]);

/* used ... */
integer C2F(getfastcode)(unsigned char *c, unsigned long c_len);


/* from cshep2.f */
int C2F(cshep2) (int *n, double *x, double *y, double *z, int *nc, int *nw, 
		 int *nr, int *lcell, int *lnext, double *xmin, double *xmax, 
		 double *dx, double *dy, double *rmax, double *rw, double *a, int *ier);

double C2F(cs2val)(double *px, double *py, int *n, double *x, double *y, 
		    double *f, int *nr, int *lcell, int *lnext, double *xmin, double *ymin,
		    double *dx, double *dy, double *rmax, double *rw, double *a);

int C2F(cs2grd) (double *px, double *py, int *n, double *x, double *y, double *f,
                 int *nr, int *lcell, int *lnext,  double *xmin, double *ymin,
		 double *dx, double *dy, double *rmax, double *rw, double *a,
		 double *c, double *cx, double *cy, int *ier);

int C2F(cs2hes)  (double *px, double *py, int *n, double *x, double *y, double *f,
		  int *nr, int *lcell, int *lnext,  double *xmin, double *ymin,
		  double *dx, double *dy, double *rmax, double *rw, double *a,
		  double *c, double *cx, double *cy, 
		  double *cxx, double *cxy, double *cyy, int *ier);


/* from the tensbs package */
double C2F(db2val)(double *xval, double *yval, int *idx, int *idy, 
		   double *tx, double *ty, int *nx, int *ny, 
		   int *kx, int *ky, double *bcoef, double *work);
double C2F(db3val)(double *xval, double *yval, double *zval, int *idx, int *idy, int *idz, 
		   double *tx, double *ty, double *tz, int *nx, int *ny, int *nz, 
		   int *kx, int *ky, int *kz, double *bcoef, double *work);
