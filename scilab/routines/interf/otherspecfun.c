/*
 *   PURPOSE  
 *      Scilab interface onto some special mathematical 
 *      functions, currently only :
 *
 *        1/ legendre associated function
 *        2/ beta function (real positive case)
 *
 *   AUTHOR
 *      Bruno Pincon <Bruno.Pincon@iecn.u-nancu.fr>
 *             
 */
#ifdef WIN32
#include <string.h> /*pour strcmp */
#endif

#include <math.h>
#include "../stack-c.h"



#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))


#if WIN32
extern int F2C(dxlegf)();
#endif

/* fortran functions headers */
double F2C(dgamma)(double *);
double F2C(betaln)(double *, double *);


static int verify_cstr(double x[], int nb_elt, int *xmin, int *xmax)
{
  /*    1/ verify that the array x is formed by non negative integers
   *       regularly spaced with increment equal to 1 (if yes return 1
   *       if not return 0)
   *    2/ computes the min and the max
   */
  int i;
  if ( ! (floor(x[0]) == x[0]  &&  x[0] >= 0 ))
    return 0;
  for ( i = 1 ; i < nb_elt ; i++ )
    if ( x[i] != x[i-1]+1.0 )
      return 0;

  *xmin = (int) x[0];
  *xmax = (int) x[nb_elt-1];
  return 1;
} 

static double return_an_inf()
{
  static int first = 1;
  static double inf = 1.0;

  if ( first )
    {
      inf = inf/(inf - (double) first);
      first = 0;
    }
  return (inf);
}

static intlegendre(char *fname)
{
  /*
   *   Interface onto the (Slatec) dxleg.f code. 
   *   Scilab calling sequence :
   *
   *   p = legendre(n, m, x [, norm_flag] )
   *
   *      x is a vector with mnx elements (it is better to
   *        have a row vector but this is not forced)
   *
   *      n : a non negative integer scalar (or a vector of such
   *          integer regularly speced with an increment of 1)
   *      m : same constraints than for n
   *
   *      n and m may not be both vectors
   *
   *      norm_flag : optionnal. When it is present and equal to "norm"
   *                  it is a normalised version which is computed
    */
  int minlhs=1, maxlhs=1, minrhs=3, maxrhs=4;
  int it, lc, mM, nM, lM, m1, m2, mN, nN, lN, n1, n2, mx, nx, lx, mnx, ms, ns, ls;
  int M_is_scalar = 0, N_is_scalar = 0, normalised, MNp1, lpqa, lipqa, *ipqa;
  double *x,/* theta,*/ xx, dnu1, *pqa;
  int id, ierror, i, j, nudiff;

  GetRhsVar(1, "d", &mN, &nN, &lN);

  if ( ! verify_cstr(stk(lN), mN*nN, &n1, &n2) )
    {
      Scierror(999,"%s: bad first argument\r\n", fname);
      return 0;
    };
  if ( mN == 1 && nN == 1) N_is_scalar = 1;

  GetRhsVar(2, "d", &mM, &nM, &lM);
  if ( ! verify_cstr(stk(lM), mM*nM, &m1, &m2) )
    {
      Scierror(999,"%s: bad second argument\r\n", fname);
      return 0;
    }
  if ( mM == 1 && nM == 1) M_is_scalar = 1;

  if ( ! M_is_scalar  &&  ! N_is_scalar )
    {
      Scierror(999,"%s: only one of arg1 and arg2 may be a vector\r\n", fname);
      return 0;
    };
       
  GetRhsCVar(3,"d", &it, &mx, &nx, &lx, &lc);
  if ( it != 0 )
    {
      Scierror(999,"%s: 3th argument must be a real matrix\r\n", fname);
      return 0;
    };

  mnx = mx*nx;
  x = stk(lx);
  for ( i = 0 ; i < mnx ; i++ )
    if ( ! (fabs(x[i]) < 1.0) ) 
      {
	Scierror(999,"%s: 3th argument must be a matrix with elements in (-1,1)\r\n", fname);
	return 0;
      };
  
  if ( Rhs == 4 )
    {
      GetRhsVar(4,"c", &ms, &ns, &ls);
      if ( strcmp(cstk(ls),"norm") == 0)
	normalised = 1;
      else
	normalised = 0;
    }
  else
    normalised = 0;
  
  MNp1 = max (n2 - n1, m2 - m1) + 1;

  CreateVar(Rhs+1, "d", &MNp1, &mnx, &lpqa); pqa = stk(lpqa);
  CreateVar(Rhs+2, "i", &MNp1, &mnx, &lipqa); ipqa = istk(lipqa);

  if ( normalised )
    id = 4;
  else
    id = 3;

  nudiff = n2-n1;  
  dnu1 = (double) n1;

  for ( i = 0 ; i < mnx ; i++ )
    {
      xx = fabs(x[i]); /* dxleg computes only for x in [0,1) */
      F2C(dxlegf) (&dnu1, &nudiff, &m1, &m2, &xx, &id, 
		   stk(lpqa+i*MNp1), istk(lipqa+i*MNp1), &ierror);
      if ( ierror != 0 )
	{
	  if ( ierror == 207 )
	    Scierror(999,"%s: overflow or underflow of an extended range number\r\n", fname);
	  else
	    Scierror(999,"%s: error number %d\r\n", fname, ierror);
	  return 0;
	};
    }

  /*  dxlegf returns the result under a form (pqa,ipqa) (to 
   *  compute internaly with an extended exponent range)
   *  When the "exponent" part (ipqa) is 0 then the number is exactly
   *  given by pqa else it leads to an overflow or an underflow.
   */
  for ( i = 0 ; i < mnx*MNp1 ; i++ )
    {
      if ( ipqa[i] < 0 ) 
	pqa[i] = 0.0;
      if ( ipqa[i] > 0 )
	pqa[i] = pqa[i] * return_an_inf(); /* pqa[i] * Inf  to have the sign */
    }

  /* complete the result by odd/even symmetry for negative x */
  for ( i = 0 ; i < mnx ; i++ )
    if ( x[i] < 0.0 )
      if ( (n1+m1) % 2 == 1 )
	for ( j = 0 ; j < MNp1 ; j+=2 )
	  pqa[i*MNp1 + j] = -pqa[i*MNp1 + j];
      else
	for ( j = 1 ; j < MNp1 ; j+=2 )
	  pqa[i*MNp1 + j] = -pqa[i*MNp1 + j];
  
  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}

static intbeta(char *fname)
{
  /*
   *   z = beta(x, y)
   *
   *      x, y : matrices of the same size of positive reals
   *
   *   For small x+y values uses the expression with the
   *   gamma function, else the exponential applied on the log beta
   *   function (provided in the dcd lib)
   *
   *   The switch limit have been set by using the gp-pari software.
   *
   */
  int minlhs=1, maxlhs=1, minrhs=2, maxrhs=2;
  int mx, nx, itx, lx, lxc, my, ny, ity, ly, lyc,/* it,*/ lz, i;
  double *x, *y, *z, xpy;
  double switch_limit = 2;

  GetRhsCVar(1, "d", &itx, &mx, &nx, &lx, &lxc); x = stk(lx);
  GetRhsCVar(2, "d", &ity, &my, &ny, &ly, &lyc); y = stk(ly);
  CheckSameDims(1,2,mx,nx,my,ny);
  if ( itx == 1  ||  ity == 1 )
    {
      Scierror(999,"%s: beta doesn't work for complex arguments \r\n", fname);
      return 0;
    };

  CreateVar(3, "d", &mx, &nx, &lz); z = stk(lz);

  for ( i = 0 ; i < mx*nx ; i++ )
    {
      if ( x[i] <= 0.0  ||  y[i] <= 0.0 )
	{
	  Scierror(999,"%s: arguments must be positive");
	  return 0;
	}
      xpy = x[i] + y[i];
      if ( xpy <= switch_limit )
	z[i] = F2C(dgamma)(&x[i]) * F2C(dgamma)(&y[i]) / F2C(dgamma)(&xpy);
      else
	z[i] = exp(F2C(betaln)(&x[i], &y[i]));
    }
  LhsVar(1) = 3;
  PutLhsVar();
  return 0;
}

static TabF Tab[]={ 
  {intlegendre, "legendre"},
  {intbeta, "beta"}
};

int C2F(otherspfunlib)(void)
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name);
  return 0;
}
