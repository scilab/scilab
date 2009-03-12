/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bernard HUGUENEY
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h> // debug
#include <math.h> //sqrt
#include "invert_matrix.h"
#include "MALLOC.h"
#include "core_math.h" // Max
#include "machine.h" // C2F


extern double F2C(dlamch)(char* , unsigned long int);
/*
  C Abs and Max macros are not ieee nan compliant but neither are the fortran77
  intrinsics used in the original dlange.f code.
*/

#define C_LANGE

#ifdef PRINT_DEBUG
static void dumpMatrix(int n_row, int n_col, double* data){
  int i, j;
  for(i=0; i != n_col; ++i){
    for(j=0; j!= n_row; ++j, ++data)
      { fprintf(stderr, "%f ", *data); }
    fprintf(stderr, "\n");
  }
  return;
}
#endif

static double dlange_1(int n_row, int n_col, double* data){
  int i,j;
  double res=0.;
  for(i=0; i != n_col; ++i){
    double sum=0.;
    for(j=0; j!= n_row; ++j, ++data)
      { sum += Abs(*data); } /* should be C99 fabs */
    res = Max(res, sum); /* should be C99 fmax */
  }
  return res;

}

static double zlange_1(int n_row, int n_col, double* data){
  int i,j;
  double res=0.;
  for(i=0; i != n_col; ++i){
    double sum=0.;
    for(j=0; j!= n_row; ++j){
      const double tmp=*data * *data;
      ++data;
      sum += sqrt(tmp + (*data * *data));
      ++data;
    }
    res = Max(res, sum); /* should be C99 fmax */
  }
  return res;

}


/*
   TODO replace ugly constant 17 with properly #define d (enum ed ? ) constant
   for malloc error.
*/
int iInvertMatrixM(int iRows, int iCols, double* pData, int complexArg
		   , double* pdblRcond){
  int ret=0;
  int* piPivot=(int*)MALLOC(iCols*sizeof(int));
  if(piPivot){
    unsigned long lWorkSize= Max(1, 4*iCols);
    void* pWork= complexArg
      ? MALLOC(lWorkSize*sizeof(double)*2)
      : MALLOC(iCols*sizeof(int));
    if(pWork){
      double* pdblWork=(double*)MALLOC(lWorkSize*sizeof(double) *(complexArg  ? 2: 1));

      if(pdblWork){
	ret = iInvertMatrix(iRows, iCols, pData, complexArg, pdblRcond, piPivot
			    , pWork, lWorkSize, pdblWork);
	FREE(pdblWork);
      }else{
	ret = 17;// TODO: this is not a stack allocation pb anymore because we (tried to) use the heap
      }
      FREE(pWork);
    }else{// pWork alloc did not succeed
      ret = 17;
    }
    FREE(piPivot);
  }else{ // piPivot alloc did not succeed
    ret = 17;
  }
  return ret;
}

int iInvertMatrix(int iRows, int iCols, double* pData, int complexArg
		  , double * pdblRcond, int* piPivot, void* pWork
		  , unsigned long lWorkSize, double* pdblWork){
#ifdef PRINT_DEBUG
  printf("in iInvertMatrix iRows=%i iCols=%i, *pData=%f, complexArg=%i\n", iRows, iCols, *pData, complexArg);
  dumpMatrix(iRows, iCols, pData);
#endif
  int ret = 0; // >0 erreur <0 warning
  /* ANORM = dlange( '1', M, N, stk(lA), M, stk(lDWORK) )
     computes  one norm of a matrix (maximum column sum)
     last work area is not used for norm '1'
     see http://publib.boulder.ibm.com/infocenter/clresctr/vxrx/topic/com.ibm.cluster.essl44.guideref.doc/am501_llange.html
  */
  /* using "1" to pass '1' to Fortran ("by ref"->pointer to char)*/
#ifdef C_LANGE
  double dblAnorm = complexArg
    ?  zlange_1(iRows, iCols, pData)
    : dlange_1(iRows, iCols, pData);
#else
  double dblAnorm = complexArg
    ?  C2F(zlange)("1", &iRows, &iCols, pData, &iRows,pdblWork /* see comment above */)
    : C2F(dlange)("1", &iRows, &iCols, pData, &iRows,pdblWork/*see above*/);
#endif

#ifdef PRINT_DEBUG
  fprintf(stderr,"afterdlange\n");
  fprintf(stderr,"dblAnorm=%f\n",dblAnorm);
#endif
  int iInfo;
  if(complexArg){
    C2F(zgetrf)(&iRows, &iCols, pData, &iCols, piPivot, &iInfo);
  }else{
    C2F(dgetrf)(&iRows, &iCols, pData, &iCols, piPivot, &iInfo);
  }
  if(iInfo !=0){
    if(iInfo >0){
      ret = 19;
    }
  }else{
    *pdblRcond = 0.;
    if(complexArg){
      C2F(zgecon)("1", &iCols, pData, &iCols, &dblAnorm, pdblRcond, pdblWork
		  , (double*)pWork, &iInfo);
    }else{
      C2F(dgecon)("1", &iCols, pData, &iCols, &dblAnorm, pdblRcond, pdblWork
		  , (int*)pWork, &iInfo);
    }
    if(*pdblRcond <= sqrt(F2C(dlamch)("e",1L))){ ret = -1; }
    if(complexArg){
      C2F(zgetri)( &iCols, pData, &iCols, piPivot, pdblWork, &lWorkSize
		   , &iInfo);
    }else{
      C2F(dgetri)( &iCols, pData, &iCols, piPivot, pdblWork, &lWorkSize
		   , &iInfo);
    }
    /* surprisingly enough, the original Fortran code does not check returned iInfo ...*/
  }
  return ret;
}
