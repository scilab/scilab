# include "../machine.h"
int C2F(ciargc)()
{
#ifndef __ABSC__
  return iargc();
#else
  return IARGC();
#endif
}

