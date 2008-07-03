/*--------------------------------------------------------------------------*/
/* INRIA */
/* AUTHOR : Bruno Pincon */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "gw_interpolation.h"
#include "interpolation.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern double C2F(cs2val)(double *px, double *py, int *n, double *x, double *y,double *f, int *nr, int *lcell, int *lnext, double *xmin, double *ymin,double *dx, double *dy, double *rmax, double *rw, double *a);
extern int C2F(cs2grd) (double *px, double *py, int *n, double *x, double *y, double *f, int *nr, int *lcell, int *lnext,  double *xmin, double *ymin,double *dx, double *dy, double *rmax, double *rw, double *a,double *c, double *cx, double *cy, int *ier);
extern int C2F(cs2hes)  (double *px, double *py, int *n, double *x, double *y, double *f, int *nr, int *lcell, int *lnext,  double *xmin, double *ymin, double *dx, double *dy, double *rmax, double *rw, double *a, double *c, double *cx, double *cy,  double *cxx, double *cxy, double *cyy, int *ier);
/*--------------------------------------------------------------------------*/
int inteval_cshep2d(char *fname,unsigned long fname_len)
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

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
  if ( mx != my  ||  nx != ny )
  {
	Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: Same sizes expected.\n"),fname,1,2);
	return 0;
  }

  GetRhsVar(3,TYPED_LIST_DATATYPE,&mt, &nt, &lt);
  GetListRhsVar(3, 1,MATRIX_OF_STRING_DATATYPE, &m1,  &n1, &Str);    /* m1 = 1, n1 = 8 ? a verifier */
  if ( strcmp(Str[0],"cshep2d") != 0)
    {
		/* Free Str */
		if (Str)
		{
			int li=0;
			while ( Str[li] != NULL)
			{
				FREE(Str[li]);
				li++;
			};
			FREE(Str);
			Str=NULL;
		}
	  Scierror(999,_("%s: Wrong type for input argument #%d: %s tlist expected.\n"), fname,2,"cshep2d");
      return 0;
    }
  /* Free Str */
  if (Str)
  {
	  int li=0;
	  while ( Str[li] != NULL)
	  {
		  FREE(Str[li]);
		  li++;
	  };
	  FREE(Str);
	  Str=NULL;
  }
  GetListRhsVar(3, 2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2,  &lxyz);   /* m2 = n , n2 = 3  */
  GetListRhsVar(3, 3,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m3, &n3,  (int *)&Cell);  /* m3 = nr, n3 = nr */
  GetListRhsVar(3, 4,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m4, &n4,  (int *)&Next);  /* m4 = 1 , n4 = n  */
  GetListRhsVar(3, 5,MATRIX_OF_DOUBLE_DATATYPE, &m5, &n5,  &lgrid);  /* m5 = 1 , n5 = 4  */
  GetListRhsVar(3, 6,MATRIX_OF_DOUBLE_DATATYPE, &m6, &n6,  &lrmax);  /* m6 = 1 , n6 = 1  */
  GetListRhsVar(3, 7,MATRIX_OF_DOUBLE_DATATYPE, &m7, &n7,  &lrw);    /* m7 = 1 , n7 = n  */
  GetListRhsVar(3, 8,MATRIX_OF_DOUBLE_DATATYPE, &m8, &n8,  &la);     /* m8 = 9 , n8 = n  */

  cell = (int *)Cell.D; next = (int *)Next.D;
  xp = stk(lx); yp = stk(ly); np = mx*nx;
  n = m2; nr = m3;
  xyz = stk(lxyz); grid = stk(lgrid);

  CreateVar(4,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lf); f = stk(lf);
  if ( Lhs > 1 )
    {
      CreateVar(5,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldfdx); dfdx = stk(ldfdx);
      CreateVar(6,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldfdy); dfdy = stk(ldfdy);
    }
  if ( Lhs > 3 )
    {
      CreateVar(7,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdxx); dffdxx = stk(ldffdxx);
      CreateVar(8,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdxy); dffdyy = stk(ldffdxy);
      CreateVar(9,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &ldffdyy); dffdxy = stk(ldffdyy);
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
/*--------------------------------------------------------------------------*/
