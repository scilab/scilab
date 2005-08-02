/* Copyright INRIA/ENPC */
#include "../machine.h"

/** only used to force the linker to load all blas functions **/
#ifndef WIN32

extern int C2F(izamax)(),C2F(dasum)(),C2F(daxpy)(),C2F(dcopy)(),C2F(ddot)(),C2F(dgbmv)(),C2F(dgemm)(),C2F(dgemv)(),C2F(dger)(),C2F(dnrm2)(),C2F(drot)(),C2F(drotg)(),C2F(dsbmv)(),C2F(dscal)(),C2F(dspmv)(),C2F(dspr)(),C2F(dspr2)(),C2F(dswap)(),C2F(dsymm)(),C2F(dsymv)(),C2F(dsyr)(),C2F(dsyr2)(),C2F(dsyr2k)(),C2F(dsyrk)(),C2F(dtbmv)(),C2F(dtbsv)(),C2F(dtpmv)(),C2F(dtpsv)(),C2F(dtrmm)(),C2F(dtrmv)(),C2F(dtrsm)(),C2F(dtrsv)(),C2F(dzasum)(),C2F(dznrm2)(),C2F(idamax)();


int Blas_contents(x) 
     int x;
{
  if ( x== 1) 
    {
      C2F(dasum)();
      C2F(daxpy)();
      C2F(dcopy)();
      C2F(ddot)();
      C2F(dgbmv)();
      C2F(dgemm)();
      C2F(dgemv)();
      C2F(dger)();
      C2F(dnrm2)();
      C2F(drot)();
      C2F(drotg)();
      C2F(dsbmv)();
      C2F(dscal)();
      C2F(dspmv)();
      C2F(dspr)();
      C2F(dspr2)();
      C2F(dswap)();
      C2F(dsymm)();
      C2F(dsymv)();
      C2F(dsyr)();
      C2F(dsyr2)();
      C2F(dsyr2k)();
      C2F(dsyrk)();
      C2F(dtbmv)();
      C2F(dtbsv)();
      C2F(dtpmv)();
      C2F(dtpsv)();
      C2F(dtrmm)();
      C2F(dtrmv)();
      C2F(dtrsm)();
      C2F(dtrsv)();
      C2F(dzasum)();
      C2F(dznrm2)();
      C2F(idamax)();
      C2F(izamax)();
    }
  return 0;
}

#endif
