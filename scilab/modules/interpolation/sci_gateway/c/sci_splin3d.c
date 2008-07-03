/*--------------------------------------------------------------------------*/
/* INRIA */
/* AUTHOR : Bruno Pincon */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <string.h>
#include "gw_interpolation.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(db3ink)();
/*--------------------------------------------------------------------------*/
int intsplin3d(char * fname,unsigned long fname_len)
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

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
  CheckVector(1, mx, nx); 
  x = stk(lx);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
  CheckVector(2, my, ny); 
  y = stk(ly);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &mz, &nz, &lz);
  CheckVector(2, mz, nz); 
  z = stk(lz);

  nx = mx*nx; 
  ny = my*ny; 
  nz = mz*nz;

  if ( nx < 3  ||  ny < 3  ||  nz < 3 )
    {
      Scierror(999,_("%s: Wrong size for input arguments: x, y and z grids must have at least %d points.\n"), fname, 3);
      return 0;
    }

  GetRhsRealHMat(4, &V);
  if ( V.dimsize != 3 )
  {
	Scierror(999,_("%s: Wrong size for input argument #%d: A real 3-dimension hypermatrix expected.\n"),fname,4);
    return 0;
  }
  if ( V.dims[0] != nx  ||  V.dims[1] != ny  ||  V.dims[2] != nz  )
    {
      Scierror(999,"%s: Wrong value for input argument: Size incompatibility between grid points and grid values.\n", fname);
      return 0;
    }

  if ( Rhs == 5 )
    {
      GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &mo, &no, &lo);
      if ( (mo != 1 && no != 1)  ||  mo*no != 3 )
	{
	  Scierror(999,_("%s: Wrong value for input argument #%d: Vector with %d components expected.\n"),fname,4,3);
	  return 0;
	}
      kx = (int)*stk(lo); 
	  ky = (int)*stk(lo+1); 
	  kz = (int)*stk(lo+2);
      if ( kx < 2  ||  kx >= nx  ||  ky < 2  ||  ky >= ny  ||  kz < 2  ||  kz >= nz )
	{
	  Scierror(999,_("%s: Wrong values for input argument #%d.\n"), fname, 5);
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
  mwkx = Max(mwkx, mwky);
  mwk = nx*ny*nz + 2*(Max(mwkx, mwkz));
  nxyz = nx*ny*nz;

  CreateVar(Rhs+1,TYPED_LIST_DATATYPE, &seven, &one, &ltlist);
  CreateListVarFromPtr(Rhs+1, 1,MATRIX_OF_STRING_DATATYPE, &one,  &seven, Str);
  lar = -1; CreateListVarFrom(Rhs+1, 2,MATRIX_OF_DOUBLE_DATATYPE, &ntx, &one, &ltx, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 3,MATRIX_OF_DOUBLE_DATATYPE, &nty, &one, &lty, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 4,MATRIX_OF_DOUBLE_DATATYPE, &ntz, &one, &ltz, &lar);
  lorder = 4;
  lar = -1; CreateListVarFrom(Rhs+1, 5,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &three, &one, &lorder, &lar);
  order = istk(lorder); order[0] = kx; order[1] = ky; order[2] = kz;
  lar = -1; CreateListVarFrom(Rhs+1, 6,MATRIX_OF_DOUBLE_DATATYPE, &nxyz,  &one, &lbcoef, &lar);
  lar = -1; CreateListVarFrom(Rhs+1, 7,MATRIX_OF_DOUBLE_DATATYPE, &six,  &one, &lxyzminmax, &lar);
  xyzminmax = stk(lxyzminmax);
  xyzminmax[0] = x[0]; xyzminmax[1] = x[nx-1];
  xyzminmax[2] = y[0]; xyzminmax[3] = y[ny-1];
  xyzminmax[4] = z[0]; xyzminmax[5] = z[nz-1];
  CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &mwk, &one, &lwork);    /* work */

  flag = 0;
  C2F(db3ink) ( stk(lx), &nx, stk(ly), &ny, stk(lz), &nz, V.R,
		&nx, &ny, &kx, &ky, &kz, stk(ltx), stk(lty), stk(ltz),
		stk(lbcoef), stk(lwork), &flag);

  if ( flag != 1 )
    {
      Scierror(999,_("%s: Problem with 'flag' = %d\n"), fname, flag);
      return 0;
    }

  /*  Return only the tlist  */
  LhsVar(1) = Rhs+1;
  PutLhsVar();
  return 0;
}
