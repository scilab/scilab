/*-----------------------------------------------------------------------------------*/
/* INRIA */
/* AUTHOR : Bruno Pincon */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_interpolation.h"
#include "stack-c.h"
#include "interpolation.h"
#include "someinterp.h"
/*-----------------------------------------------------------------------------------*/
#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] = {
	{ "C0"        , C0         },
	{ "by_zero"   , BY_ZERO    },
	{ "natural"   , NATURAL    },
	{ "periodic"  , PERIODIC   },
	{ "by_nan"    , BY_NAN     },
	{ "linear"    , LINEAR     }};
/*-----------------------------------------------------------------------------------*/
int intlinear_interpn(char *fname,unsigned long fname_len)
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
  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l);  /* => lets store an array of pointers  */
  xp = (double **) stk(l);               /*   with size of 4 or 8 bytes */

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp);
  xp[0] = stk(lxp);
  np = mxp*nxp;
  for ( i = 2 ; i <= n ; i++ )
    {
      GetRhsVar(i,MATRIX_OF_DOUBLE_DATATYPE, &mxpn, &nxpn, &lxpn);
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
  CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l);  /* => lets store an array of pointers  */
  x = (double **) stk(l);               /*    with size(void *) = 4 or 8 bytes */

  for ( i = 1 ; i <= n ; i++ )
    {
      GetRhsVar(n+i,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
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
      GetRhsVar(2*n+1,MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
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

  CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE, &n, &one, &l); u = stk(l);
  m = 1; for ( i = 1 ; i <= n ; i++) m = 2*m;
  CreateVar(Rhs+5,MATRIX_OF_DOUBLE_DATATYPE, &m, &one, &l); v = stk(l);
  l = 4; CreateVar(Rhs+6, "I", &m, &one, &l); ad = istk(l);
  l = 4; CreateVar(Rhs+7, "I", &n, &one, &l); k = istk(l);
  CreateVar(Rhs+8,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &l); yp = stk(l);

  nlinear_interp(x, val, dim, n, xp, yp, np, outmode, u, v, ad, k);

  LhsVar(1) = Rhs+8;
  PutLhsVar();
  /* correction Warning Allan CORNET */
  /* warning C4715: 'intlinear_interpn' : not all control paths return a value */
  return 0;
}
/*-----------------------------------------------------------------------------------*/

