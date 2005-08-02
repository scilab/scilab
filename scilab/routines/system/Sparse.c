/* Copyright INRIA/ENPC */
#include "../machine.h"

/** only used to force the linker to load  functions **/

#ifndef WIN32

extern int C2F(dspe2)();
extern int C2F(wspe2)();

int Sparse_contents( int x)
{
  if ( x== 1) 
    {
      C2F(dspe2)();
      C2F(wspe2)();
    }
  return 0;
}

#endif
