#include "../mex.h"
#include "../stack-c.h"

extern Gatefunc C2F(sident);
extern Gatefunc C2F(sorder);
extern Gatefunc C2F(findbd);

int intrankqr(fname)
     char* fname;
{
  int *header1;int *header2;
  int Cmplx;int ret;

  header1 = (int *) GetData(1);
  Cmplx=header1[3];
  if (Cmplx==0) {
    ret = C2F(intmb03od)("rankqr",6L);
    return; } else
      {
	/*	ret = C2F(intzb03od)("rankqr",6L); */
	return; } 
}

static GenericTable Tab[]={
{(Myinterfun) fortran_mex_gateway, C2F(sident),"sident"},
{(Myinterfun) fortran_mex_gateway, C2F(sorder),"sorder"},
{(Myinterfun) fortran_mex_gateway, C2F(findbd),"findbd"},
{(Myinterfun) sci_gateway, intrankqr,"rankqr"},
	 };
 
int C2F(intslicot)()
{  Rhs = Max(0, Rhs);
(*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
