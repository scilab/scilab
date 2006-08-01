
/* Table of constant values */

static int c__1 = 1;

int dmmul1(double *a, int *na, double *b, int *nb, double *c__, 
	   int *nc, int *l, int *m, int *n)
{
  /* System generated locals */
  int i__1, i__2;
  
  /* Local variables */
  double ddot();
  static int i__, j, ib, ic;
  
  /* !but */
  /*     ce sous programme effectue le produit matriciel: */
  /*     c=c+a*b . */
  /* !liste d'appel */
  
  /*     subroutine dmmul1(a,na,b,nb,c,nc,l,m,n) */
  /*     double precision a(na,m),b(nb,n),c(nc,n) */
  /*     integer na,nb,nc,l,m,n */
  
  /*     a            tableau de taille na*m contenant la matrice a */
  /*     na           nombre de lignes du tableau a dans le programme appel */
  /*     b,nb,c,nc    definitions similaires a celles de a,na */
  /*     l            nombre de ligne des matrices a et c */
  /*     m            nombre de colonnes de a et de lignes de b */
  /*     n            nombre de colonnes de b et c */
  /* !sous programmes utilises */
  /*     neant */
  /* ! */
  /*     Copyright INRIA */
  
  /* Parameter adjustments */
  --c__;
  --b;
  --a;
  
  /* Function Body */
  ib = 1;
  ic = 0;
  i__1 = *n;
  for (j = 1; j <= i__1; ++j) {
    i__2 = *l;
    for (i__ = 1; i__ <= i__2; ++i__) {
      /* L20: */
      c__[ic + i__] += ddot(m, &a[i__], na, &b[ib], &c__1);
    }
    ic += *nc;
    ib += *nb;
    /* L30: */
  }
  return 0;
} /* dmmul1 */

double ddot(int *n, double *dx, int *incx, double *dy, int *incy)
{
  /* System generated locals */
  int i__1;
  double ret_val;
  
  /* Local variables */
  static int i__, m;
  static double dtemp;
  static int ix, iy, mp1;
  
  
  /*     forms the dot product of two vectors. */
  /*     uses unrolled loops for increments equal to one. */
  /*     jack dongarra, linpack, 3/11/78. */
  /*     modified 12/3/93, array(1) declarations changed to array(*) */
  
  
  /* Parameter adjustments */
  --dy;
  --dx;
  
  /* Function Body */
  ret_val = 0.;
  dtemp = 0.;
  if (*n <= 0) {
    return ret_val;
  }
  if (*incx == 1 && *incy == 1) {
    goto L20;
  }
  
  /*        code for unequal increments or equal increments */
  /*          not equal to 1 */
  
  ix = 1;
  iy = 1;
  if (*incx < 0) {
    ix = (-(*n) + 1) * *incx + 1;
  }
  if (*incy < 0) {
    iy = (-(*n) + 1) * *incy + 1;
  }
  i__1 = *n;
  for (i__ = 1; i__ <= i__1; ++i__) {
    dtemp += dx[ix] * dy[iy];
    ix += *incx;
    iy += *incy;
    /* L10: */
  }
  ret_val = dtemp;
  return ret_val;
  
  /*        code for both increments equal to 1 */
  
  
  /*        clean-up loop */
  
 L20:
  m = *n % 5;
  if (m == 0) {
    goto L40;
  }
  i__1 = m;
  for (i__ = 1; i__ <= i__1; ++i__) {
    dtemp += dx[i__] * dy[i__];
    /* L30: */
  }
  if (*n < 5) {
    goto L60;
  }
 L40:
  mp1 = m + 1;
  i__1 = *n;
  for (i__ = mp1; i__ <= i__1; i__ += 5) {
    dtemp = dtemp + dx[i__] * dy[i__] + dx[i__ + 1] * dy[i__ + 1] + dx[
								       i__ + 2] * dy[i__ + 2] + dx[i__ + 3] * dy[i__ + 3] + dx[i__ + 
															       4] * dy[i__ + 4];
    /* L50: */
  }
 L60:
  ret_val = dtemp;
  return ret_val;
} /* ddot */

