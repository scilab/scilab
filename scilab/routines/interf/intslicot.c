#include "../mex.h"
#include "../stack-c.h"

extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);

static GenericTable Tab[]={
{(Myinterfun) fortran_mex_gateway, C2F(sident),"sident msg"},
{(Myinterfun) fortran_mex_gateway, C2F(sorder),"sorder msg"},
{(Myinterfun) fortran_mex_gateway, C2F(findbd),"findbd msg"},
	 };
 
int C2F(intslicot)()
{  Rhs = Max(0, Rhs);
(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
