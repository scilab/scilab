#include "../mex.h"
#include "../stack-c.h"

extern Gatefunc C2F(sident);

static GenericTable Tab[]={
{(Myinterfun) fortran_mex_gateway, C2F(sident),"error msg"},
	 };
 
int C2F(intslicot)()
{  Rhs = Max(0, Rhs);
(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
