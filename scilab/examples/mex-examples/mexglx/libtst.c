#include <mex.h>
extern Gatefunc mexFunction;
static GenericTable Tab[]={
  {mex_gateway,mexFunction,"errmsg"},
};

int C2F(libtst)()
{
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f))(Tab[Fin-1].name,Tab[Fin-1].F);
  return 0;
}

