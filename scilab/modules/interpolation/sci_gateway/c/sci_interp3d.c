/*-----------------------------------------------------------------------------------*/
/* INRIA */
/* AUTHOR : Bruno Pincon */
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
#include <Windows.h>
#endif
#include <string.h>
#include "stack-c.h"
#include "interpolation.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
extern int get_type(TableType *Tab, int dim_table, int *scistr, int strlength);
extern int C2F(driverdb3valwithgrad)();
extern int C2F(driverdb3val)();
/*-----------------------------------------------------------------------------------*/ 
int intinterp3d(char *fname,unsigned long fname_len)
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
/*-----------------------------------------------------------------------------------*/ 
