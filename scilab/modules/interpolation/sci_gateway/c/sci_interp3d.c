/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Bruno Pincon
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/
#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <string.h>
#include "gw_interpolation.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "MALLOC.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(driverdb3valwithgrad)();
extern int C2F(driverdb3val)();
/*--------------------------------------------------------------------------*/
#define NB_OUTMODE 6
static TableType OutModeTable[NB_OUTMODE] = {
	{ "C0"        , C0         },
	{ "by_zero"   , BY_ZERO    },
	{ "natural"   , NATURAL    },
	{ "periodic"  , PERIODIC   },
	{ "by_nan"    , BY_NAN     },
	{ "linear"    , LINEAR     }};
/*--------------------------------------------------------------------------*/
int intinterp3d(char *fname,unsigned long fname_len)
{
  /*
   *   [f [, dfdx, dfdy, dfdz]] = interp3d(xp, yp, zp, tlcoef [,outmode])
   */

  int minrhs=4, maxrhs=5, minlhs=1, maxlhs=4;

  int mxp, nxp, lxp, myp, nyp, lyp, mzp, nzp, lzp, mt, nt, lt, np;
  int one=1, kx, ky, kz;
  int nx, ny, nz, nxyz, mtx, mty, mtz, m, n, ltx, lty, ltz, lbcoef, mwork, lwork, lfp;
  int lxyzminmax, nsix, outmode, ns, *str_outmode;
  int /*i,*/ m1, n1, ldfpdx, ldfpdy, ldfpdz;
  double *fp, *xp, *yp, *zp, *dfpdx, *dfpdy, *dfpdz;
  double *xyzminmax, xmin, xmax, ymin, ymax, zmin, zmax;
  SciIntMat Order; int *order;
  char **Str;

  CheckRhs(minrhs,maxrhs);
  CheckLhs(minlhs,maxlhs);

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp); xp = stk(lxp);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &myp, &nyp, &lyp); yp = stk(lyp);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &mzp, &nzp, &lzp); zp = stk(lzp);
  if ( mxp != myp  ||  nxp != nyp || mxp != mzp  ||  nxp != nzp)
  {
	  Scierror(999,_("%s: Wrong size for input arguments #%d, #%d and #%d: Same sizes expected.\n"),fname,1,2,3);
      return 0;
    }
  np = mxp * nxp;

  GetRhsVar(4,TYPED_LIST_DATATYPE,&mt, &nt, &lt);
  GetListRhsVar(4, 1,MATRIX_OF_STRING_DATATYPE, &m1,  &n1, &Str);
  if ( strcmp(Str[0],"tensbs3d") != 0)
    {
		/* Free Str */
		if (Str)
		{
			int i=0;
			while ( Str[i] != NULL)
			{
				FREE(Str[i]);
				i++;
			};
			FREE(Str);
			Str=NULL;
		}
      Scierror(999,_("%s: Wrong type for input argument #%d: %s tlist expected.\n"), fname,4,"tensbs3d");
      return 0;
    }
  /* Free Str */
  if (Str)
  {
	  int i=0;
	  while ( Str[i] != NULL)
	  {
		  FREE(Str[i]);
		  i++;
	  };
	  FREE(Str);
	  Str=NULL;
  }
  GetListRhsVar(4, 2,MATRIX_OF_DOUBLE_DATATYPE, &mtx, &n,  &ltx);
  GetListRhsVar(4, 3,MATRIX_OF_DOUBLE_DATATYPE, &mty, &n,  &lty);
  GetListRhsVar(4, 4,MATRIX_OF_DOUBLE_DATATYPE, &mtz, &n,  &ltz);
  GetListRhsVar(4, 5,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m  , &n,  (int *)&Order);
  GetListRhsVar(4, 6,MATRIX_OF_DOUBLE_DATATYPE, &nxyz,&n,  &lbcoef);
  GetListRhsVar(4, 7,MATRIX_OF_DOUBLE_DATATYPE, &nsix,&n,  &lxyzminmax);
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
	  Scierror(999,_("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"),fname,5,"outmode");
	  return 0;
	};
    }
  else
    outmode = C0;

  CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lfp); fp = stk(lfp);

  order = (int *)Order.D;
  kx = order[0]; ky = order[1]; kz = order[2];
  nx = mtx - kx; ny = mty - ky; nz = mtz - kz;

  mwork = ky*kz + 3*Max(kx,Max(ky,kz)) + kz;
  CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &mwork, &one, &lwork);

  if ( Lhs == 1 )
    {
      C2F(driverdb3val)(xp,yp,zp,fp,&np,stk(ltx), stk(lty), stk(ltz),
			&nx, &ny, &nz, &kx, &ky, &kz, stk(lbcoef), stk(lwork),
			&xmin, &xmax, &ymin, &ymax, &zmin, &zmax, &outmode);
      LhsVar(1) = Rhs+1;
    }
  else
    {
      CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &ldfpdx); dfpdx = stk(ldfpdx);
      CreateVar(Rhs+4,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &ldfpdy); dfpdy = stk(ldfpdy);
      CreateVar(Rhs+5,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &ldfpdz); dfpdz = stk(ldfpdz);
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
/*--------------------------------------------------------------------------*/
