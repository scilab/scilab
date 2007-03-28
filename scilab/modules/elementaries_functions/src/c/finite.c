#include "machine.h"

#if !(defined HAVE_FINITE) && (defined hppa)

#include <math.h>

int finite(double x)
{
  if (isfinite(x)==0) return 0;
  return 1;
}

#else
#if !(defined HAVE_FINITE)

typedef unsigned int __uint32_t;
typedef union 
{
  struct 
  {
    __uint32_t lsw;
    __uint32_t msw;
  } parts;
  double value;
} ieee_double_shape_type;

int finite(double x)
{
  int hx;
  ieee_double_shape_type gh_u;
  
  gh_u.value = x;
  hx = gh_u.parts.msw;
  return  (int)((__uint32_t)((hx&0x7fffffff)-0x7ff00000)>>31);
}
#endif
#endif
