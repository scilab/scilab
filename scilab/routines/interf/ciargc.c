int ciargc()
{
#ifndef __ABSC__
  return iargc();
#else
  return IARGC();
#endif
}
