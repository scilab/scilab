#include <mex.h> 
static int direct_gateway(char *fname,void F(void)) { F();return 0;};
extern Gatefunc mex_fmex1;
extern Gatefunc mex_fmex2;
extern Gatefunc C2F(mexfoof);
extern Gatefunc int_f4;
static GenericTable Tab[]={
  {mex_gateway,mex_fmex1,"f1"},
  {mex_gateway,mex_fmex2,"f2"},
  {(Myinterfun)fortran_mex_gateway,C2F(mexfoof),"f3"},
  {(Myinterfun)sci_gateway,int_f4,"f4"},
};
 
int C2F(libmex)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
