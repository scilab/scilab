/*  
 *  PURPOSE
 *     interface code for some interpolation / approximation
 *     routines
 *
 *  AUTHOR
 *     Bruno Pincon
 */

#if WIN32
#include <string.h>
#endif
#include <math.h>


#include "../stack-c.h"
#include "interp.h"

#ifdef WIN32

#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#if WIN32
extern char *GetExceptionString(DWORD ExceptionCode);
#endif


#if WIN32
#undef min
#undef max
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) < (b) ? (b) : (a))


#if WIN32
extern int C2F(dset)();
extern int C2F(bicubicinterpwithgrad)();
extern int C2F(bicubicinterpwithgradandhes)();
extern int C2F(db3ink)();
extern int C2F(driverdb3val)();
extern int C2F(driverdb3valwithgrad)();
#endif



enum {NOT_A_KNOT, NATURAL, CLAMPED, PERIODIC, FAST, FAST_PERIODIC, 
      MONOTONE, BY_ZERO, C0, LINEAR, BY_NAN, UNDEFINED};

typedef struct { char *str_type; int type; } TableType;

#define NB_SPLINE_TYPE 7
static TableType SplineTable[NB_SPLINE_TYPE] = { 
  { "not_a_knot"   , NOT_A_KNOT    },
  { "natural"      , NATURAL       },
  { "clamped"      , CLAMPED       },
  { "periodic"     , PERIODIC      },
  { "monotone"     , MONOTONE      },
  { "fast"         , FAST          },
  { "fast_periodic", FAST_PERIODIC }};

#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] = {
  { "C0"        , C0         },
  { "by_zero"   , BY_ZERO    },
  { "natural"   , NATURAL    },
  { "periodic"  , PERIODIC   },
  { "by_nan"    , BY_NAN     },
  { "linear"    , LINEAR     }};

static int good_order(double x[], int n)
{
  /*  test if x[i-1] < x[i] */
  static int first = 1;
  int i;
  static double inf;

  if ( first ) { inf = 1.0 / (double) (first - first) ; first = 0; }

  if (fabs(x[0]) == inf  ||  x[n-1] == inf)
    return ( 0 );

  for ( i = 1 ; i < n ; i++ ) 
    if ( ! (x[i-1] < x[i]) )   /* form of this test for detecting nan ... */ 
      return ( 0 );

  return ( 1 );
}
  
/*
 *  Routines spéciales : 
 *      récupérer une hypermatrice réelle
 *      récupérer une string scilab sans convertion
 *      comparer une string scilab et une C string classique
 *  (elles ne modifient pas les variables correspondantes
 *   ce qui est fondamental pour que cette interface soit
 *   passée par référence).
 */

#define GetRhsScalarString(num,n,t) if (!get_rhs_scalar_string(num,n,t)) { return 0;}
#define GetRhsRealHMat(pos,H) if (!get_rhs_real_hmat(pos,H)) { return 0;}

typedef struct realhypermat {
  int dimsize;        /* number of dimensions of the hyper matrix */
  int size;           /* total number of elements : size = dims[0]x... x dims[dimsize-1] */
  int *dims;          /* number of elements in each dimension */
  double *R;          /* points to the elements  */
} RealHyperMat;

static int get_rhs_real_hmat(int num, RealHyperMat *H)
{
  int il, il1, il2, il3,/* it, */lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw )); 
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));
	  
  if ( *istk(il) != 17 )
    goto err;
  else if ( *istk(il+1) != 3 )  /* a hm mlist must have 3 fields */
    goto err;

  /*  get the pointers for the 3 fields */
  il1 = sadr(il+6);
  il2 = il1 + *istk(il+3) - 1;
  il3 = il1 + *istk(il+4) - 1;
  il1 = iadr(il1); il2 = iadr(il2); il3 = iadr(il3);

  /*  test if the first field is a matrix string with 3 components
   *  and that the first is "hm" (ie 17 22  in scilab char code)
   */
  if ( (*istk(il1) != 10)  |  ((*istk(il1+1))*(*istk(il1+2)) != 3)  )
    goto err;
  else if ( *istk(il1+5)-1 != 2 )  /* 1 str must have 2 chars */
    goto err;
  else if ( *istk(il1+8) != 17  || *istk(il1+9) != 22 )
    goto err;

  /*  get the 2d field */
  if ( (*istk(il2) != 8)  |  (*istk(il2+3) != 4) )
    goto err;
  H->dimsize = (*istk(il2+1))*(*istk(il2+2));
  H->dims = istk(il2+4);

  /*  get the 3d field */
  if ( !( *istk(il3) == 1 && *istk(il3+3)==0) )
    goto err;;

  H->size = (*istk(il3+1))*(*istk(il3+2));
  H->R = stk(sadr(il3+4));

  /* needed for Jpc stuff (putlhsvar) */
  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;  /* a voir ? */
  return 1;

 err:
  Scierror(999,"Argument %d is not a real hypermatrix\r\n", num);
  return 0;
}

static int get_rhs_scalar_string(int num, int *length, int **tabchar)
{
  int il, lw;

  lw = num + Top - Rhs;
  il = iadr(*Lstk( lw )); 
  if ( *istk(il) < 0 )
    il = iadr(*istk(il+1));
	  
  if ( ! ( *istk(il) == 10  &&  (*istk(il+1))*(*istk(il+2)) == 1 ) ) 
    {
      /* we look for a scalar string */
      Scierror(999,"Argument %d is not a scalar string\r\n", num);
      return 0;
    }
  *length = *istk(il+5)-1;
  *tabchar = istk(il+6);

  Nbvars = Max(Nbvars,num);
  C2F(intersci).ntypes[num-1] = '$';
  C2F(intersci).iwhere[num-1] = *Lstk(lw);
  C2F(intersci).lad[num-1] = 0;
  return 1;
}

static int equal_scistring_and_string(int length, int *scistr,  char *str)
{
  /* compare a scistring with a classic C string */
  int i, res;
  
  if ( strlen(str) != length )
    return 0;

  res = 1; i = 0;
  while (res && i < length)
    {
      res = (scistr[i] == (int)C2F(getfastcode)(str+i,1L));
      i++;
    }
  return (res);
}

static int get_type(TableType *Tab, int dim_table, int *scistr, int strlength)
{
  int i = 0, found = 0;
  while ( !found  &&  i < dim_table )
    {
      found =  equal_scistring_and_string(strlength, scistr, Tab[i].str_type);
      i++;
    } 
  if ( found )
    return ( Tab[i-1].type );
  else
    return ( UNDEFINED );
} 


static int intsplin(char *fname,unsigned long fname_len)
{
  int minrhs=2, maxrhs=4, minlhs=1, maxlhs=1;

  int mx, nx, lx, my, ny, ly, mc, nc, lc, n, spline_type;
  int *str_spline_type, ns;
  int /*i,*/ ld/*, flag*/;
  int mwk1, nwk1, lwk1, mwk2, nwk2, lwk2, mwk3, nwk3, lwk3, mwk4, nwk4, lwk4;
  double *x, *y, *d, *c;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mx, &nx, &lx);
  GetRhsVar(2,"d", &my, &ny, &ly);

  if ( mx != my  ||  nx != ny  ||  mx != 1  &&  nx != 1 ) 
    { 
      Scierror(999,"%s: arg1 and arg2 must be 2 vectors with same size\r\n", fname);
      return 0;
    }
  n = mx*nx;    /* number of interpolation points */
  if ( n < 2 ) 
    { 
      Scierror(999,"%s: the number of interpolation points must be >= 2\r\n", fname);
      return 0;
    }
  x = stk(lx); y = stk(ly);
  if (! good_order(x, n))  /* verify strict increasing abscissae */
    {
      Scierror(999,"%s: elts of arg 1 not (strictly) increasing or +-inf detected\r\n", fname);
      return 0;
    }

  if ( Rhs >= 3 )   /* get the spline type */
    {
      GetRhsScalarString(3, &ns, &str_spline_type);
      spline_type =  get_type(SplineTable, NB_SPLINE_TYPE, str_spline_type, ns);
      if ( spline_type == UNDEFINED )
	{
	  Scierror(999,"%s: unknown spline_type\n\r",fname);
	  return 0;
	};
    }
  else
    spline_type = NOT_A_KNOT;

  if ( spline_type == CLAMPED ) /* get arg 4 which contains the end point slopes */
    {
      if ( Rhs != 4 )
	{
	  Scierror(999,"%s: for a clamped spline you must give the endpoint slopes\n\r",fname);
	  return 0;
	}
      GetRhsVar(4,"d", &mc, &nc, &lc);
      if ( mc*nc != 2 )
	{
	  Scierror(999,"%s: bad dimension for the 4 arg (endpoint slopes)\n\r",fname);
	  return 0;
	}
      c = stk(lc);
    }
  else if ( Rhs == 4 )
    {
      Scierror(999,"%s: 4 args are required only for a clamped spline\n\r",fname);
      return 0;
    }
    
  /*  verify y(1) = y(n) for periodic splines */
  if ( (spline_type == PERIODIC || spline_type == FAST_PERIODIC)  &&  y[0] != y[n-1] )
    {
      Scierror(999,"%s: for a periodic spline y(1) must be equal to y(n)\n\r",fname);
      return(0);
    };

  CreateVar(Rhs+1, "d", &mx,  &nx,   &ld); /* memory for d (only argument returned) */   
  d = stk(ld);

  switch(spline_type)
    {
    case(FAST) : case(FAST_PERIODIC) :
      nwk1 = 1;
      C2F(derivd) (x, y, d, &n, &nwk1, &spline_type);
      break;

    case(MONOTONE) :
      nwk1 = 1;
      C2F(dpchim) (&n, x, y, d, &nwk1);
      break;

    case(NOT_A_KNOT) : case(NATURAL) : case(CLAMPED) : case(PERIODIC) :
      /*  (the wk4 work array is used only in the periodic case) */
      mwk1 = n; nwk1 = 1; mwk2 = n-1; nwk2 = 1; mwk3 = n-1; nwk3 = 1; mwk4 = n-1; nwk4 = 1;
      CreateVar(Rhs+2, "d", &mwk1,  &nwk1,   &lwk1);
      CreateVar(Rhs+3, "d", &mwk2,  &nwk2,   &lwk2);
      CreateVar(Rhs+4, "d", &mwk3,  &nwk3,   &lwk3);
      lwk4 = lwk1;
      if (spline_type == CLAMPED) 
	{ d[0] = c[0]; d[n-1] = c[1]; };
      if (spline_type == PERIODIC)
	CreateVar(Rhs+5, "d", &mwk4,  &nwk4,   &lwk4);
      C2F(splinecub) (x, y, d, &n, &spline_type, stk(lwk1), stk(lwk2), stk(lwk3), stk(lwk4));
      break;
    }
  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}

static int intlsq_splin(char *fname,unsigned long fname_len)
{
  /*   interface code for [y, d] = lsq_splin(xd, yd [, wd], x)  */

  int minrhs=3, maxrhs=4, minlhs=1, maxlhs=2;

  int mxd, nxd, lxd, myd, nyd, lyd, mx, nx, lx, mwd, nwd, lwd;
  int ly, ld, lwork, ndata, n, one=1, mwork, ierr;
  double un=1.0;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mxd, &nxd, &lxd);
  GetRhsVar(2,"d", &myd, &nyd, &lyd);
  ndata = mxd*nxd;  /* number of data points */
  if ( ndata < 4  ||  mxd != myd  || nxd != nyd  ||  mxd != 1  &&  nxd != 1 ) 
    { 
      Scierror(999,"%s: arg 1 and 2 must be vectors of same size with at least %d elements\r\n",
	       fname, 4);
      return 0;
    }

  if ( Rhs == 4 ) 
    {
      GetRhsVar(3,"d", &mwd, &nwd, &lwd);
      if ( mxd != mwd  ||  nxd != nwd )
	{ 
	  Scierror(999,"%s: bad input for arg 3\r\n", fname);
	  return 0;
	}
    }
  GetRhsVar(Rhs,"d", &mx, &nx, &lx);
  n = mx*nx;
  if ( n < 2  ||  mx != 1  &&  nx != 1 )
    { 
      Scierror(999,"%s: bad input for x \r\n", fname);
      return 0;
    }
  
  if (! good_order(stk(lx), n))   /* verify strict increasing abscissae */
    {
      Scierror(999,"%s: elts of arg %d not (strictly) increasing or +-inf detected\r\n", fname, Rhs);
      return 0;
    }

  CreateVar(Rhs+1, "d", &mx,  &nx,   &ly);
  CreateVar(Rhs+2, "d", &mx,  &nx,   &ld);
  mwork = 7*n+18;
  CreateVar(Rhs+3, "d", &mwork, &one, &lwork); 
  if ( Rhs == 3 )
    {
      CreateVar(Rhs+4, "d", &mxd, &nxd, &lwd);
      C2F(dset)( &ndata, &un, stk(lwd), &one);  /* set all the weight = 1  */ 
    }

  C2F(spfit)(stk(lxd), stk(lyd), stk(lwd), &ndata, stk(lx), &n, stk(ly),
	     stk(ld), stk(lwork), &ierr);

  if (ierr == -1)
    {
      Scierror(999,"%s: not enought points for the fit \r\n", fname);
      return 0;
    }
  else if (ierr == 1)
    sciprint("%s warning: rank deficiency of the least square matrix\r\n", fname);

  LhsVar(1) = Rhs+1;
  LhsVar(2) = Rhs+2;
  PutLhsVar();
  return 0;
}

static int intinterp1(char *fname,unsigned long fname_len)
{
  int minrhs=4, maxrhs=5, minlhs=1, maxlhs=4;

  int mt, nt, lt, mx, nx, lx, my, ny, ly, md, nd, ld, ns, *str_outmode;
  int n, m, outmode, lst, ldst, lddst, ldddst;
/*  double *x;*/

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mt, &nt, &lt);
  GetRhsVar(2,"d", &mx, &nx, &lx);
  GetRhsVar(3,"d", &my, &ny, &ly);
  GetRhsVar(4,"d", &md, &nd, &ld);

  if ( mx != my  ||  nx != ny  ||  md != mx || nd != nx || mx != 1  &&  nx != 1 || mx*nx < 2) 
    { 
      Scierror(999,"%s: bad inputs \r\n", fname);
      return 0;
    }
  n = mx*nx;    /* number of interpolation points */
  m = mt*nt;    /* number of points to interpolate */

  if ( Rhs == 5 )   /* get the outmode */
    {
      GetRhsScalarString(5, &ns, &str_outmode);
      outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
      if ( outmode == UNDEFINED )
	{
	  Scierror(999,"%s: unknown outmode type\n\r",fname);
	  return 0;
	};
    }
  else
    outmode = C0;  /* default outmode */

  /* memory for st, dst, ddst, dddst */
  CreateVar(Rhs+1, "d", &mt,  &nt, &lst);
  CreateVar(Rhs+2, "d", &mt,  &nt, &ldst);
  CreateVar(Rhs+3, "d", &mt,  &nt, &lddst);
  CreateVar(Rhs+4, "d", &mt,  &nt, &ldddst);

  /*      subroutine EvalPWHermite(t, st, dst, ddst, dddst, m, x, y, d, n, outmode)
   *      integer m, n, outmode
   *      double precision t(m), st(m), dst(m), ddst(m), dddst(m), x(n), y(n), d(n)
   */
  C2F(evalpwhermite) (stk(lt), stk(lst), stk(ldst), stk(lddst), stk(ldddst),
                      &m, stk(lx), stk(ly), stk(ld), &n, &outmode);

  LhsVar(1) = Rhs+1;
  LhsVar(2) = Rhs+2;
  LhsVar(3) = Rhs+3;
  LhsVar(4) = Rhs+4;
  PutLhsVar();
  return 0;
}

static int intlinear_interpn(char *fname,unsigned long fname_len)
{
/*  interpolation lineaire n-dimensionnelle
 *
 *   yp = linear_interpn(xp1, ..., xpn, x1, ..., xn, val, outmode)
 */
  int n, mxp, nxp, lxp, mxpn, nxpn, lxpn, mx, nx, lx, my, ny, ly, one=1;
  int ns, *str_outmode, np, *k, *ad, m, l, i, outmode;
  int *dim;
  double **xp, **x, *val, *u, *v, *yp;
  RealHyperMat U;

  n = (Rhs+1)/2 - 1;
  if ( n < 1 )
    { 
      Scierror(999,"%s: too few arg \r\n", fname);
      return 0;
    }

  /*  les points sur lesquels on evalue par interpolation */
/*   l = I_UINT32; CreateVar(Rhs+1, "I", &n, &one, &l); */
/*   xp = (double **) istk(l); */
  CreateVar(Rhs+1, "d", &n, &one, &l);  /* => lets store an array of pointers  */
  xp = (double **) stk(l);               /*   with size of 4 or 8 bytes */

  GetRhsVar(1,"d", &mxp, &nxp, &lxp);
  xp[0] = stk(lxp);
  np = mxp*nxp;
  for ( i = 2 ; i <= n ; i++ )
    {
      GetRhsVar(i,"d", &mxpn, &nxpn, &lxpn);
      if ( mxp != mxpn || nxp != nxpn )
	{ 
	  Scierror(999,"%s: bad inputs for xp1, xp2, ...., \r\n", fname);
	  return 0;
	}
      xp[i-1] = stk(lxpn);
    }
 
  /* coordonnées de la grille */
  l = I_INT32; CreateVar(Rhs+2, "I", &n, &one, &l);
  dim = istk(l);
/*   l = I_UINT32; CreateVar(Rhs+3, "I", &n, &one, &l); */
/*   x = (double **) istk(l); */
  CreateVar(Rhs+3, "d", &n, &one, &l);  /* => lets store an array of pointers  */
  x = (double **) stk(l);               /*    with size(void *) = 4 or 8 bytes */

  for ( i = 1 ; i <= n ; i++ )
    {
      GetRhsVar(n+i,"d", &mx, &nx, &lx);
      if ( (mx != 1 && nx != 1) && mx*nx < 2)
	{ 
	  Scierror(999,"%s: bad arg number %d \r\n", fname, n+i);
	  return 0;
	}
      x[i-1] = stk(lx);
      dim[i-1] = mx*nx;
      /* verify strict increasing order  */
      if ( !good_order(x[i-1], mx*nx) )
	{
	  Scierror(999,"%s: grid abscissae of dim %d not in strict increasing order \r\n", fname, n+i);
	  return 0;
	}
    }

  /* les valeurs de la grille */
  if ( n >= 3 )
    {
      GetRhsRealHMat(2*n+1,&U);
      if ( U.dimsize != n )
	{ 
	  Scierror(999,"%s: U must be a real %d-dim hypermatrix  \n", fname, n);
	  return 0;
	}
      for ( i = 0 ; i < n ; i++ )
	if ( U.dims[i] != dim[i] )
	  { 
	    Scierror(999,"%s: size incompatibility between grid points and grid values in dim %d \n\r", fname, i+1);
	    return 0;
	  }
      val = U.R;
    }
  else  /* n = 1 or 2 */
    {
      GetRhsVar(2*n+1,"d", &my, &ny, &ly);
      if ( n == 1  &&  my*ny != dim[0] )
	{ 
	  Scierror(999,"%s: size incompatibility between grid points and values in dim 1 \n\r", fname);
	  return 0;
	}
      if ( n == 2  &&  (my != dim[0]  || ny != dim[1]) )
	{ 
	  Scierror(999,"%s: size incompatibility between grid points and values in dim 1 or 2 \n\r", fname);
	  return 0;
	}
      val = stk(ly);
    }

  /* get the outmode */
  if ( Rhs == 2*n + 2 )
    {
      GetRhsScalarString(Rhs, &ns, &str_outmode);
      outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
      if ( outmode == UNDEFINED || outmode == LINEAR )
	{
	  Scierror(999,"%s: unsupported outmode type\n\r",fname);
	  return 0;
	};
    }
  else
    outmode = C0;

  CreateVar(Rhs+4, "d", &n, &one, &l); u = stk(l);
  m = 1; for ( i = 1 ; i <= n ; i++) m = 2*m;
  CreateVar(Rhs+5, "d", &m, &one, &l); v = stk(l);
  l = 4; CreateVar(Rhs+6, "I", &m, &one, &l); ad = istk(l);
  l = 4; CreateVar(Rhs+7, "I", &n, &one, &l); k = istk(l);
  CreateVar(Rhs+8, "d", &mxp, &nxp, &l); yp = stk(l);

  nlinear_interp(x, val, dim, n, xp, yp, np, outmode, u, v, ad, k);

  LhsVar(1) = Rhs+8;
  PutLhsVar();
  /* correction Warning Allan CORNET */
  /* warning C4715: 'intlinear_interpn' : not all control paths return a value */
  return 0;
}


static int intsplin2d(char *fname,unsigned long fname_len)
{
  /*    interface pour splin2d :
   *
   *    C = splin2d(x, y, z [, type])
   *
   */
 
  int minrhs=3, maxrhs=4, minlhs=1, maxlhs=1;

  int mx, nx, lx, my, ny, ly, mz, nz, lz, ns, mc, nc, lc, lp, lq, lr;
  int spline_type, *str_spline_type/*, i*/;
  int one = 1;
  double *x, *y, *C;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mx, &nx, &lx);
  GetRhsVar(2,"d", &my, &ny, &ly);
  GetRhsVar(3,"d", &mz, &nz, &lz);

  if ( mx != 1 || my != 1 || mz != nx || nz != ny || nx < 2 || ny < 2)
    { 
      Scierror(999,"%s: bad inputs \r\n", fname);
      return 0;
    }

  /* verify strict increasing order for x and y */
  x = stk(lx);  y = stk(ly);
  if ( !good_order(x, nx) || !good_order(y, ny))
    {
      Scierror(999,"%s: x and/or y are not in strict increasing order (or +-inf detected) \r\n", fname);
      return 0;
    }

  /* get the spline type */
  if ( Rhs == 4 ) 
    {
      GetRhsScalarString(4, &ns, &str_spline_type);
      spline_type = get_type(SplineTable, NB_SPLINE_TYPE, str_spline_type, ns);
      if ( spline_type == UNDEFINED || spline_type == CLAMPED )
	{
	  Scierror(999,"%s: unsupported spline type\n\r",fname);
	  return 0;
	};
    }
  else
    spline_type = NOT_A_KNOT;

  /* memory for the big C array */
  mc = 16*(nx-1)*(ny-1); nc = 1;
  CreateVar( Rhs+1, "d", &mc,  &nc, &lc);
  C = stk(lc);
  /* memory for work arrays  */
  CreateVar( Rhs+2, "d", &nx, &ny, &lp);
  CreateVar( Rhs+3, "d", &nx, &ny, &lq);
  CreateVar( Rhs+4, "d", &nx, &ny, &lr);

  if (spline_type == MONOTONE || spline_type == FAST || spline_type == FAST_PERIODIC)
    {   
      C2F(bicubicsubspline)(x, y, stk(lz), &nx, &ny, stk(lc), 
			    stk(lp), stk(lq), stk(lr), &spline_type);
    }

  else   /*  spline */
    {
      int lA_d, lA_sd, ld, lqdu, lutemp, nxy, nxym1, nxym2, lll;

      nxy = max(nx,ny); nxym1 = nxy-1; nxym2 = nxy-2; 

      CreateVar( Rhs+5, "d", &nxy,   &one, &lA_d);
      CreateVar( Rhs+6, "d", &nxym1, &one, &lA_sd);
      CreateVar( Rhs+7, "d", &ny,    &one, &ld);
      CreateVar( Rhs+8, "d", &nxy,   &one, &lqdu);
      CreateVar( Rhs+9, "d", &ny,    &one, &lutemp);

      if (spline_type == PERIODIC)
	{
	  CreateVar(Rhs+10, "d", &nxym2, &one, &lll) ;
	}
      else
	lll = lA_sd ;   /* bidon ... */
      C2F(bicubicspline)(x, y, stk(lz), &nx, &ny, stk(lc), stk(lp), stk(lq), stk(lr), 
                         stk(lA_d), stk(lA_sd), stk(ld), stk(lll), stk(lqdu), 
			 stk(lutemp), &spline_type);
    }

  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}

static int intinterp2d(char *fname,unsigned long fname_len)
{
  /*    interface pour interp2d :
   *
   *    [zp [, dzdxp, dzdyp [, d2zdx2, d2zdxy, d2zdy2]]] = interp2d(xp, yp, x, y, C[, outmode])
   */
 
  int minrhs=5, maxrhs=6, minlhs=1, maxlhs=6;

  int mxp, nxp, lxp, myp, nyp, lyp, mx, nx, lx, my, ny, ly;
  int mc, nc, lc, ns, *str_outmode, lzp, ldzdxp, ldzdyp, ld2zdx2p, ld2zdxyp, ld2zdy2p;
  int outmode, m;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mxp, &nxp, &lxp);
  GetRhsVar(2,"d", &myp, &nyp, &lyp);
  GetRhsVar(3,"d", &mx, &nx, &lx);
  GetRhsVar(4,"d", &my, &ny, &ly);
  GetRhsVar(5,"d", &mc, &nc, &lc);

  if ( mxp != myp || nxp != nyp || mx != 1 || my != 1 || nc != 1 || nx < 2 || ny < 2
       || mc != 16*(nx-1)*(ny-1) )
    { 
      Scierror(999,"%s: bad inputs \r\n", fname);
      return 0;
    }

  /* get the outmode */
  if ( Rhs == 6 ) 
    {
      GetRhsScalarString(6, &ns, &str_outmode);
      outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
      if ( outmode == UNDEFINED || outmode == LINEAR )
	{
	  Scierror(999,"%s: unsupported outmode type\n\r",fname);
	  return 0;
	};
    }
  else
    outmode = C0;

  /* memory for zp */
  CreateVar( Rhs+1, "d", &mxp,  &nxp, &lzp);
  m = mxp*nxp;

  if ( Lhs == 1 )
    {
      /*   subroutine BiCubicInterp(x, y, C, nx, ny, x_eval, y_eval, z_eval, m, outmode)
       *     integer nx, ny, m, outmode
       *     double precision x(nx), y(ny), C(4,4,nx-1,ny-1), x_eval(m), y_eval(m), z_eval(m)
       */
      C2F(bicubicinterp)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp), stk(lyp), stk(lzp), 
			 &m, &outmode);
      LhsVar(1) = Rhs+1;
    }
  else   /* got also the derivatives */
    {
      /* memory for dzdxp and dzdyp */
      CreateVar( Rhs+2, "d", &mxp,  &nxp, &ldzdxp);
      CreateVar( Rhs+3, "d", &mxp,  &nxp, &ldzdyp);

      if ( Lhs <= 3 )
	{
	  C2F(bicubicinterpwithgrad)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp), 
				     stk(lyp), stk(lzp), stk(ldzdxp), stk(ldzdyp), 
				     &m, &outmode);
	  LhsVar(1) = Rhs+1;
	  LhsVar(2) = Rhs+2;
	  LhsVar(3) = Rhs+3;
	}
      else /* got also 2d derivatives */
	{
	  CreateVar( Rhs+4, "d", &mxp,  &nxp, &ld2zdx2p);
	  CreateVar( Rhs+5, "d", &mxp,  &nxp, &ld2zdxyp);
	  CreateVar( Rhs+6, "d", &mxp,  &nxp, &ld2zdy2p);
	  C2F(bicubicinterpwithgradandhes)(stk(lx), stk(ly), stk(lc), &nx, &ny, stk(lxp), 
					   stk(lyp), stk(lzp), stk(ldzdxp), stk(ldzdyp), 
					   stk(ld2zdx2p), stk(ld2zdxyp), stk(ld2zdy2p), 
					   &m, &outmode);
	  LhsVar(1) = Rhs+1;
	  LhsVar(2) = Rhs+2;
	  LhsVar(3) = Rhs+3;
	  LhsVar(4) = Rhs+4;
	  LhsVar(5) = Rhs+5;
	  LhsVar(6) = Rhs+6;
	}
    }
  PutLhsVar();
  return 0;
}

/*  
 *   interface sur le package cshep2d ....
 */

static int intcshep2d(char *fname,unsigned long fname_len)
{
  static char *Str[]={"cshep2d", "xyz", "lcell", "lnext", "grdim", "rmax", "rw", "a"};
  int minrhs=1, maxrhs=1, minlhs=1, maxlhs=1;
  int n, dim, nc, nw, nr, one=1, four=4, eight=8, nine=9, ier;
  int lxyz, lxyzn, lcell, lnext, lgrid, lrmax, lrw, la, ltlist, lar;

  double * xyz, * grid;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &n, &dim, &lxyz);
  if ( dim != 3  ||  n < 10 ) 
    { 
      Scierror(999,"%s: xyz must be a (n,3) real matrix with n >= 10 \r\n", fname);
      return 0;
    }

  /* choix pour nc (peut etre futur parametre optionnel) */
  nc = min( 17, n-1 );
  /* choix pour nw */
  nw = min( 30, n-1 );
  /* choix pour nr (grille nr x nr) */
  nr = (int) sqrt( n/3.0 ); /* comme n >= 10 nr >= 1 */

  /* all the information for the "interpolant" will be stored
   * in a tlist (which also contains the entry xyz)  
   */
  CreateVar(2,"t", &eight, &one, &ltlist);
  CreateListVarFromPtr(2, 1, "S", &one,  &eight, Str);
  CreateListVarFrom(2, 2, "d", &n ,   &dim,  &lxyzn, &lxyz);
  lcell = 4; lar = -1;
  CreateListVarFrom(2, 3, "I", &nr,   &nr,   &lcell, &lar); /* lcell */
  lnext = 4; lar = -1;
  CreateListVarFrom(2, 4, "I", &one,  &n,    &lnext, &lar); /* lnext */
  lar = -1;
  CreateListVarFrom(2, 5, "d", &one,  &four, &lgrid, &lar); /* xmin, ymin, dx, dy */
  lar = -1;
  CreateListVarFrom(2, 6, "d", &one,  &one,  &lrmax, &lar); /* rmax */
  lar = -1;
  CreateListVarFrom(2, 7, "d", &one,  &n,    &lrw,   &lar); /* rw */
  lar = -1;
  CreateListVarFrom(2, 8, "d", &nine, &n,    &la,    &lar); /* a */
  grid = stk(lgrid);
  xyz = stk(lxyz);
  
  /*      SUBROUTINE CSHEP2 (N,X,Y,F,NC,NW,NR, LCELL,LNEXT,XMIN,
   *                         YMIN,DX,DY,RMAX,RW,A,IER)
   */
  C2F(cshep2) ( &n, xyz, &xyz[n], &xyz[2*n], &nc, &nw, &nr, istk(lcell),
		istk(lnext), grid, &grid[1], &grid[2], &grid[3], stk(lrmax),
		stk(lrw), stk(la), &ier);

  if ( ier != 0 )
    {
      Scierror(999,"%s: duplicate nodes or all nodes colinears (ier = %d) \r\n", fname, ier);
      return 0;
    }

  LhsVar(1) = 2;
  PutLhsVar();
  return 0;
}

static int inteval_cshep2d(char *fname,unsigned long fname_len)
{
  /*
   *   [f [,dfdx, dfdy [, dffdxx, dffdxy, dffdyy]]] = eval_cshep2d(xp, yp, tlcoef)  
   */

  int minrhs=3, maxrhs=3, minlhs=1, maxlhs=6;
  int mx, nx, lx, my, ny, ly, mt, nt, lt;
  char **Str;
  int m1, n1, m2, n2, m3, n3, m4, n4, m5, n5, m6, n6, m7, n7, m8, n8;
  int lxyz, lgrid, lrmax, lrw, la;
  double *xp, *yp, *xyz, *grid, *f, *dfdx, *dfdy, *dffdxx, *dffdyy, *dffdxy;
  int i, ier, n, np, nr, lf, ldfdx, ldfdy, ldffdxx, ldffdyy, ldffdxy;
  SciIntMat Cell, Next;
  int *cell, *next;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mx, &nx, &lx);
  GetRhsVar(2,"d", &my, &ny, &ly);
  if ( mx != my  ||  nx != ny ) 
    { 
      Scierror(999,"%s: xp and yp must have the same dimension \r\n", fname);
      return 0;
    }

  GetRhsVar(3,"t",&mt, &nt, &lt);
  GetListRhsVar(3, 1, "S", &m1,  &n1, &Str);    /* m1 = 1, n1 = 8 ? a verifier */
  if ( strcmp(Str[0],"cshep2d") != 0) 
    {
      FreeRhsSVar(Str);
      Scierror(999,"%s: Argument 2 is not an cshep2d tlist\r\n", fname);
      return 0;
    }
  FreeRhsSVar(Str);  
  GetListRhsVar(3, 2, "d", &m2, &n2,  &lxyz);   /* m2 = n , n2 = 3  */
  GetListRhsVar(3, 3, "I", &m3, &n3,  (int *)&Cell);  /* m3 = nr, n3 = nr */
  GetListRhsVar(3, 4, "I", &m4, &n4,  (int *)&Next);  /* m4 = 1 , n4 = n  */
  GetListRhsVar(3, 5, "d", &m5, &n5,  &lgrid);  /* m5 = 1 , n5 = 4  */
  GetListRhsVar(3, 6, "d", &m6, &n6,  &lrmax);  /* m6 = 1 , n6 = 1  */
  GetListRhsVar(3, 7, "d", &m7, &n7,  &lrw);    /* m7 = 1 , n7 = n  */
  GetListRhsVar(3, 8, "d", &m8, &n8,  &la);     /* m8 = 9 , n8 = n  */

  cell = (int *)Cell.D; next = (int *)Next.D; 
  xp = stk(lx); yp = stk(ly); np = mx*nx; 
  n = m2; nr = m3;
  xyz = stk(lxyz); grid = stk(lgrid);

  CreateVar(4, "d", &mx, &nx, &lf); f = stk(lf);
  if ( Lhs > 1 ) 
    { 
      CreateVar(5, "d", &mx, &nx, &ldfdx); dfdx = stk(ldfdx);
      CreateVar(6, "d", &mx, &nx, &ldfdy); dfdy = stk(ldfdy);
    }
  if ( Lhs > 3 ) 
    {
      CreateVar(7, "d", &mx, &nx, &ldffdxx); dffdxx = stk(ldffdxx);
      CreateVar(8, "d", &mx, &nx, &ldffdxy); dffdyy = stk(ldffdxy);
      CreateVar(9, "d", &mx, &nx, &ldffdyy); dffdxy = stk(ldffdyy);
    }

  switch( Lhs )
    {
    case ( 1 ) :
      for ( i = 0 ; i < np ; i++ )
	/*            DOUBLE PRECISION FUNCTION CS2VAL (PX,PY,N,X,Y,F,NR,
         *                          LCELL,LNEXT,XMIN,YMIN,DX,DY,RMAX,RW,A)
	 */
	f[i] = C2F(cs2val)(&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2*n], &nr,
			   cell, next, grid, &grid[1], &grid[2], &grid[3],
			   stk(lrmax), stk(lrw), stk(la));
      LhsVar(1) = 4;
      break;

    case ( 2 ) :
    case ( 3 ) :
      for ( i = 0 ; i < np ; i++ )
	/*      SUBROUTINE CS2GRD (PX,PY,N,X,Y,F,NR,LCELL,LNEXT,XMIN,
	 *.                   YMIN,DX,DY,RMAX,RW,A, C,CX,CY,IER)
	 */
	C2F(cs2grd) (&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2*n], &nr,
		     cell, next, grid, &grid[1], &grid[2], &grid[3],
		     stk(lrmax), stk(lrw), stk(la), &f[i], &dfdx[i], &dfdy[i], &ier);
      LhsVar(1) = 4;
      LhsVar(2) = 5;
      LhsVar(3) = 6;
      break;

    case ( 4 ) :
    case ( 5 ) :
    case ( 6 ) :
      for ( i = 0 ; i < np ; i++ )
	{
	/*   SUBROUTINE CS2HES (PX,PY,N,X,Y,F,NR,LCELL,LNEXT,XMIN,
         *.                     YMIN,DX,DY,RMAX,RW,A, C,CX,CY,CXX,CXY,CYY,IER)
         */
	C2F(cs2hes) (&xp[i], &yp[i], &n, xyz, &xyz[n], &xyz[2*n], &nr,
		     cell, next, grid, &grid[1], &grid[2], &grid[3],
		     stk(lrmax), stk(lrw), stk(la), &f[i], &dfdx[i], &dfdy[i], 
		     &dffdxx[i], &dffdxy[i], &dffdyy[i], &ier);
	}
      LhsVar(1) = 4;
      LhsVar(2) = 5;
      LhsVar(3) = 6;
      LhsVar(4) = 7;
      LhsVar(5) = 8;
      LhsVar(6) = 9;
      break;
    } 
  PutLhsVar();
  return 0;
}

static int intsplin3d(char * fname,unsigned long fname_len)
{
  /*
   *   [tlist] = splin3d(x, y, z, v [,orderxyz])
   */
  static char *Str[]={"tensbs3d", "tx", "ty", "tz", "order", "bcoef", "xyzminmax"};

  int minrhs=4, maxrhs=5, minlhs=1, maxlhs=1;

  int mx, nx, lx, my, ny, ly, mz, nz, lz, mo, no, lo, kx, ky, kz;
  int ntx, nty, ntz, ltx, lty, ltz, lbcoef, lxyzminmax, mwk, mwkx, mwky, mwkz;
  int flag, one=1, three=3, six=6, seven=7,ltlist, nxyz, lwork, lar, lorder, *order;
  double *x, *y, *z, *xyzminmax;
  RealHyperMat V;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mx, &nx, &lx);
  CheckVector(1, mx, nx); x = stk(lx);
  GetRhsVar(2,"d", &my, &ny, &ly);
  CheckVector(2, my, ny); y = stk(ly);
  GetRhsVar(3,"d", &mz, &nz, &lz);
  CheckVector(2, mz, nz); z = stk(lz);

  nx = mx*nx; ny = my*ny; nz = mz*nz;

  if ( nx < 3  ||  ny < 3  ||  nz < 3 ) 
    { 
      Scierror(999,"%s: the x, y and z grids must have at least 3 points \r\n", fname);
      return 0;
    }

  GetRhsRealHMat(4, &V);
  if ( V.dimsize != 3 )
    { 
      Scierror(999,"%s: 4 th argument must be a real 3-dim hypermatrix  \n", fname);
      return 0;
    }
  if ( V.dims[0] != nx  ||  V.dims[1] != ny  ||  V.dims[2] != nz  )
    { 
      Scierror(999,"%s: size incompatibility between grid points and grid values\n\r", fname);
      return 0;
    }

  if ( Rhs == 5 )
    {
      GetRhsVar(5,"d", &mo, &no, &lo);
      if ( (mo != 1 && no != 1)  ||  mo*no != 3 )
	{ 
	  Scierror(999,"%s: the 4 th arg must be a vector with 3 components \r\n", fname);
	  return 0;
	}
      kx = (int)*stk(lo); ky = (int)*stk(lo+1); kz = (int)*stk(lo+2);
      if ( kx < 2  ||  kx >= nx  ||  ky < 2  ||  ky >= ny  ||  kz < 2  ||  kz >= nz )
	{ 
	  Scierror(999,"%s: bad 5 th arg [kx ky kz]\n\r", fname);
	  return 0;
	}
    }
  else
    {
      kx = 4; ky = 4; kz = 4;
    }

  ntx = nx + kx;
  nty = ny + ky;
  ntz = nz + kz;
  mwkx = kx*(nx+1); mwky = ky*(ny+1); mwkz = kz*(nz+1); 
  mwkx = max(mwkx, mwky);
  mwk = nx*ny*nz + 2*(max(mwkx, mwkz));
  nxyz = nx*ny*nz;

  CreateVar(Rhs+1,"t", &seven, &one, &ltlist);
  CreateListVarFromPtr(Rhs+1, 1, "S", &one,  &seven, Str);
  lar = -1; CreateListVarFrom(Rhs+1, 2, "d", &ntx, &one, &ltx, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 3, "d", &nty, &one, &lty, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 4, "d", &ntz, &one, &ltz, &lar);
  lorder = 4; 
  lar = -1; CreateListVarFrom(Rhs+1, 5, "I", &three, &one, &lorder, &lar);
  order = istk(lorder); order[0] = kx; order[1] = ky; order[2] = kz;
  lar = -1; CreateListVarFrom(Rhs+1, 6, "d", &nxyz,  &one, &lbcoef, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 7, "d", &six,  &one, &lxyzminmax, &lar); 
  xyzminmax = stk(lxyzminmax); 
  xyzminmax[0] = x[0]; xyzminmax[1] = x[nx-1];  
  xyzminmax[2] = y[0]; xyzminmax[3] = y[ny-1];  
  xyzminmax[4] = z[0]; xyzminmax[5] = z[nz-1];  
  CreateVar(Rhs+2, "d", &mwk, &one, &lwork);    /* work */

  flag = 0;
  C2F(db3ink) ( stk(lx), &nx, stk(ly), &ny, stk(lz), &nz, V.R,
		&nx, &ny, &kx, &ky, &kz, stk(ltx), stk(lty), stk(ltz), 
		stk(lbcoef), stk(lwork), &flag);

  if ( flag != 1 )
    {
      Scierror(999,"%s: problem : flag = %d \r\n", fname, flag);
      return 0;
    }

  /*  Return only the tlist  */
  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}

static int intinterp3d(char *fname,unsigned long fname_len)  /* a suivre */
{
  /*
   *   [f [, dfdx, dfdy, dfdz]] = interp3d(xp, yp, zp, tlcoef [,outmode])  
   */

  int minrhs=4, maxrhs=5, minlhs=1, maxlhs=4;

  int mxp, nxp, lxp, myp, nyp, lyp, mzp, nzp, lzp, mt, nt, lt, np;
  int zero=0, one=1, kx, ky, kz;
  int nx, ny, nz, nxyz, mtx, mty, mtz, m, n, ltx, lty, ltz, lbcoef, mwork, lwork, lfp;
  int lxyzminmax, nsix, outmode, ns, *str_outmode;
  int /*i,*/ m1, n1, ldfpdx, ldfpdy, ldfpdz;
  double *fp, *xp, *yp, *zp, *dfpdx, *dfpdy, *dfpdz;
  double *xyzminmax, xmin, xmax, ymin, ymax, zmin, zmax;
  SciIntMat Order; int *order;
  char **Str;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mxp, &nxp, &lxp); xp = stk(lxp);
  GetRhsVar(2,"d", &myp, &nyp, &lyp); yp = stk(lyp);
  GetRhsVar(3,"d", &mzp, &nzp, &lzp); zp = stk(lzp);
  if ( mxp != myp  ||  nxp != nyp || mxp != mzp  ||  nxp != nzp) 
    { 
      Scierror(999,"%s: xp, yp and zp must have the same dimensions \r\n", fname);
      return 0;
    }
  np = mxp * nxp;

  GetRhsVar(4,"t",&mt, &nt, &lt);
  GetListRhsVar(4, 1, "S", &m1,  &n1, &Str);
  if ( strcmp(Str[0],"tensbs3d") != 0) 
    {
      FreeRhsSVar(Str);
      Scierror(999,"%s: 4 th argument is not an tensbs3d tlist \r\n", fname);
      return 0;
    }
  FreeRhsSVar(Str);
  GetListRhsVar(4, 2, "d", &mtx, &n,  &ltx);
  GetListRhsVar(4, 3, "d", &mty, &n,  &lty);
  GetListRhsVar(4, 4, "d", &mtz, &n,  &ltz);
  GetListRhsVar(4, 5, "I", &m  , &n,  (int *)&Order);
  GetListRhsVar(4, 6, "d", &nxyz,&n,  &lbcoef);
  GetListRhsVar(4, 7, "d", &nsix,&n,  &lxyzminmax);
  xyzminmax = stk(lxyzminmax);
  xmin = xyzminmax[0];  xmax = xyzminmax[1]; 
  ymin = xyzminmax[2];  ymax = xyzminmax[3]; 
  zmin = xyzminmax[4];  zmax = xyzminmax[5]; 


  /* get the outmode */
  if ( Rhs == 5 ) 
    {
      GetRhsScalarString(5, &ns, &str_outmode);
      outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
      if ( outmode == UNDEFINED || outmode == LINEAR || outmode == NATURAL )
	{
	  Scierror(999,"%s: unsupported outmode type\n\r",fname);
	  return 0;
	};
    }
  else
    outmode = C0;

  CreateVar(Rhs+1, "d", &mxp, &nxp, &lfp); fp = stk(lfp);

  order = (int *)Order.D;
  kx = order[0]; ky = order[1]; kz = order[2];
  nx = mtx - kx; ny = mty - ky; nz = mtz - kz; 

  mwork = ky*kz + 3*max(kx,max(ky,kz)) + kz;
  CreateVar(Rhs+2, "d", &mwork, &one, &lwork);

  if ( Lhs == 1 )
    {
      C2F(driverdb3val)(xp,yp,zp,fp,&np,stk(ltx), stk(lty), stk(ltz),
			&nx, &ny, &nz, &kx, &ky, &kz, stk(lbcoef), stk(lwork),
			&xmin, &xmax, &ymin, &ymax, &zmin, &zmax, &outmode);
      LhsVar(1) = Rhs+1;
    }
  else
    {
      CreateVar(Rhs+3, "d", &mxp, &nxp, &ldfpdx); dfpdx = stk(ldfpdx);
      CreateVar(Rhs+4, "d", &mxp, &nxp, &ldfpdy); dfpdy = stk(ldfpdy);
      CreateVar(Rhs+5, "d", &mxp, &nxp, &ldfpdz); dfpdz = stk(ldfpdz);
      C2F(driverdb3valwithgrad)(xp,yp,zp,fp,dfpdx, dfpdy, dfpdz, &np,
				stk(ltx), stk(lty), stk(ltz),
				&nx, &ny, &nz, &kx, &ky, &kz, stk(lbcoef), stk(lwork),
				&xmin, &xmax, &ymin, &ymax, &zmin, &zmax, &outmode);
      LhsVar(1) = Rhs+1;
      LhsVar(2) = Rhs+3;
      LhsVar(3) = Rhs+4;
      LhsVar(4) = Rhs+5;
    }

  PutLhsVar();
  return 0;
}

static int intbsplin3val(char *fname,unsigned long fname_len)
{
  /*
   *   [fp] = bsplin3val(xp, yp, zp, tlcoef, der)  
   */

  int minrhs=5, maxrhs=5, minlhs=1, maxlhs=1;

  int mxp, nxp, lxp, myp, nyp, lyp, mzp, nzp, lzp, mt, nt, lt, m1, n1, np;
  int zero=0, one=1, kx, ky, kz;
  int nx, ny, nz, nxyz, mtx, mty, mtz, m, n, ltx, lty, ltz, lbcoef, mwork, lwork, lfp;
  int lxyzminmax, nsix;
  int i, mder,nder,lder, ox, oy, oz;
  double *fp, *xp, *yp, *zp, *der;
  double *xyzminmax, xmin, xmax, ymin, ymax, zmin, zmax;
  SciIntMat Order; int *order;
  char **Str;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,"d", &mxp, &nxp, &lxp); xp = stk(lxp);
  GetRhsVar(2,"d", &myp, &nyp, &lyp); yp = stk(lyp);
  GetRhsVar(3,"d", &mzp, &nzp, &lzp); zp = stk(lzp);
  if ( mxp != myp  ||  nxp != nyp || mxp != mzp  ||  nxp != nzp) 
    { 
      Scierror(999,"%s: xp, yp and zp must have the same dimensions \r\n", fname);
      return 0;
    }
  np = mxp * nxp;

  GetRhsVar(4,"t",&mt, &nt, &lt);
  GetListRhsVar(4, 1, "S", &m1,  &n1, &Str);
  if ( strcmp(Str[0],"tensbs3d") != 0) 
    {
      FreeRhsSVar(Str);
      Scierror(999,"%s: 4 th argument is not an tensbs3d tlist \r\n", fname);
      return 0;
    }
  FreeRhsSVar(Str);
  GetListRhsVar(4, 2, "d", &mtx, &n,  &ltx);
  GetListRhsVar(4, 3, "d", &mty, &n,  &lty);
  GetListRhsVar(4, 4, "d", &mtz, &n,  &ltz);
  GetListRhsVar(4, 5, "I", &m  , &n,  (int *)&Order);
  GetListRhsVar(4, 6, "d", &nxyz,&n,  &lbcoef);
  GetListRhsVar(4, 7, "d", &nsix,&n,  &lxyzminmax);
  xyzminmax = stk(lxyzminmax);
  xmin = xyzminmax[0];  xmax = xyzminmax[1]; 
  ymin = xyzminmax[2];  ymax = xyzminmax[3]; 
  zmin = xyzminmax[4];  zmax = xyzminmax[5]; 

  GetRhsVar(5,"d", &mder, &nder, &lder);
  der = stk(lder);
  if (   mder*nder != 3
      || der[0] != floor(der[0]) || der[0] < 0.0 
      || der[1] != floor(der[1]) || der[1] < 0.0 
      || der[2] != floor(der[2]) || der[2] < 0.0 )
    {
      Scierror(999,"%s: bad 5 th argument \r\n", fname);
      return 0;
    }
  ox = (int) der[0];  oy = (int) der[1];  oz = (int) der[2];


  CreateVar(Rhs+1, "d", &mxp, &nxp, &lfp); fp = stk(lfp);

  order = (int *)Order.D;
  kx = order[0]; ky = order[1]; kz = order[2];
  nx = mtx - kx; ny = mty - ky; nz = mtz - kz; 

  mwork = ky*kz + 3*max(kx,max(ky,kz)) + kz;
  CreateVar(Rhs+2, "d", &mwork, &one, &lwork);

  for ( i=0; i<np ; i++ )
    {
      fp[i] = C2F(db3val)(&(xp[i]), &(yp[i]), &(zp[i]), &ox, &oy, &oz, 
			  stk(ltx), stk(lty), stk(lty), &nx, &ny, &nz,
			  &kx, &ky, &kz, stk(lbcoef), stk(lwork));
    }

  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}

static TabF Tab[]={ 
  {intsplin,           "splin"},
  {intlsq_splin,       "lsq_splin"},
  {intinterp1,          "interp"},
  {intlinear_interpn,  "linear_interpn"},
  {intsplin2d,         "splin2d"},
  {intinterp2d,        "interp2d"},
  {intcshep2d,         "cshep2d"},
  {inteval_cshep2d,    "eval_cshep2d" },
  {intsplin3d,         "splin3d"},
  {intinterp3d,        "interp3d"},
  {intbsplin3val,      "bsplin3val"}
};

int C2F(intinterp)(void)
{
	Rhs = Max(0, Rhs);

	#if WIN32
		#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
		#else
			(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		#endif
	#else
		(*(Tab[Fin-1].f))(Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif

	return 0;
}
