#include <mex.h> 
extern Gatefunc intscalc;
extern Gatefunc intssom;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intscalc,"calc"},
  {(Myinterfun)sci_gateway,intssom,"som"},
};
 
int C2F(libmyex)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
