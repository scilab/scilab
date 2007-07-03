/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_interpolation.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]={ 
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
/*-----------------------------------------------------------------------------------*/
int C2F(gw_interpolation)(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
