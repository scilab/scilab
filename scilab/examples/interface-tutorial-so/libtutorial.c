#include <mex.h> 
extern Gatefunc intview;
extern Gatefunc intmatmul;
static GenericTable Tab[]={
  {(Myinterfun)sci_gateway,intview,"view"},
  {(Myinterfun)sci_gateway,intmatmul,"matmul"},
};
 
int C2F(libtutorial)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}
