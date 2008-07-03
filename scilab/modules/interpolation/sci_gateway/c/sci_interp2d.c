/*--------------------------------------------------------------------------*/
/* INRIA */
/* AUTHOR : Bruno Pincon */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_interpolation.h"
#include "stack-c.h"
#include "interpolation.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(bicubicinterp)(double *x, double *y, double *C, int *nx, int *ny,double *x_eval, double *y_eval, double *z_eval, int *m,int *outmode);
extern int C2F(bicubicinterpwithgradandhes)();
extern int C2F(bicubicinterpwithgrad)();
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
int intinterp2d(char *fname,unsigned long fname_len)
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

  GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &mxp, &nxp, &lxp);
  GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &myp, &nyp, &lyp);
  GetRhsVar(3,MATRIX_OF_DOUBLE_DATATYPE, &mx, &nx, &lx);
  GetRhsVar(4,MATRIX_OF_DOUBLE_DATATYPE, &my, &ny, &ly);
  GetRhsVar(5,MATRIX_OF_DOUBLE_DATATYPE, &mc, &nc, &lc);

  if ( mxp != myp || nxp != nyp || mx != 1 || my != 1 || nc != 1 || nx < 2 || ny < 2
       || mc != 16*(nx-1)*(ny-1) )
    {
	  Scierror(999,_("%s: Wrong value for input arguments #%d and #%d: Same sizes expected.\n"),fname,1,2);
      return 0;
    }

  /* get the outmode */
  if ( Rhs == 6 )
    {
      GetRhsScalarString(6, &ns, &str_outmode);
      outmode =  get_type(OutModeTable, NB_OUTMODE, str_outmode, ns);
      if ( outmode == UNDEFINED || outmode == LINEAR )
	{
	  Scierror(999,_("%s: Wrong values for input argument #%d: Unsupported '%s' type.\n"),fname,6,"outmode");
	  return 0;
	};
    }
  else
    outmode = C0;

  /* memory for zp */
  CreateVar( Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &lzp);
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
      CreateVar( Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ldzdxp);
      CreateVar( Rhs+3,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ldzdyp);

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
	  CreateVar( Rhs+4,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdx2p);
	  CreateVar( Rhs+5,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdxyp);
	  CreateVar( Rhs+6,MATRIX_OF_DOUBLE_DATATYPE, &mxp,  &nxp, &ld2zdy2p);
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
/*--------------------------------------------------------------------------*/

