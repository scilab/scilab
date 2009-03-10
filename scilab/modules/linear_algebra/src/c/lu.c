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
#include "machine.h"
#include "core_math.h"
#include "MALLOC.h"
#include "lu.h"
#include "doublecomplex.h"

extern int F2C(zgetrf)(int*, int*, double*, int*, int*, int*);
extern int F2C(dgetrf)(int*, int*, double*, int*, int*, int*);

int iLuM(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pdblEData )
{
  int ret;
  int* piPivot;
  int* piWork;
  int iMinRowsCols;
  double* pdblWork;
  iMinRowsCols= Min( iRows, iCols);
  piPivot= (int*) MALLOC(iMinRowsCols * sizeof(int));
  piWork= NULL;
  pdblWork= NULL;
  if(pdblEData == NULL) /* must allocate more tmp memory when not computing E */
    {
      piWork= (int*) MALLOC(iRows * sizeof(int));
      pdblWork= (double*) MALLOC(iRows * iMinRowsCols * ( complexArg ? sizeof(doublecomplex) : sizeof(double)));
    }
  ret = iLu(pData, iRows, iCols, complexArg, pdblLData, pdblUData, pdblEData, piPivot, piWork, pdblWork);
  if(pdblEData == NULL)
    {
      FREE( piWork);
      FREE( pdblWork );
    }
  return ret;
}
int iLu(double* pData, int iRows, int iCols, int complexArg, double* pdblLData, double* pdblUData, double* pdblEData
	, int* piPivot, int* piWork, double* pdblWork)
{
  int ret;
  int iInfo;
  if(complexArg)
    {
      C2F(zgetrf)(&iRows, &iCols, pData, &iRows, piPivot, &iInfo);
    }
  else
    {
      C2F(dgetrf)(&iRows, &iCols, pData, &iRows, piPivot, &iInfo);
    }
  if (iInfo <0)
    {
      ret= iInfo;
    }
  else
    {
      int iMinRowsCols;
      int i, j;
      double* pdCurrentL;
      double* pdCurrentU;
      double* pdCurrentArg;
      iMinRowsCols = Min(iRows, iCols);
      /* fill L matrix with 1.[+0i] on the diagonal */
      /*
	/!\ if E was not requested, we will have to swap rows before returning the result, so we do not
	fill the real L matrix, but pdblWork instead. We will then copy and swap rows from pdblWork to the result matrix.
       */
      pdCurrentL= (pdblWork == NULL) ? pdblLData : pdblWork ;
      pdCurrentArg= pData;
      for( j= 0; j != iMinRowsCols; ++j)
	{
	  for ( i=0; i!= iRows; ++i, ++pdCurrentL, ++pdCurrentArg)
	    {
	      if(i > j)
		{
		  *pdCurrentL = *pdCurrentArg;
		  if(complexArg)
		    { /* copy imaginary part */
		      *++pdCurrentL = *++pdCurrentArg;
		    }
		}
	      else
		{
		  *pdCurrentL= (i==j) ? 1. : 0.;
		  if(complexArg)
		    { /* set imaginary part to 0. and advance pdCurrentArg */
		      *++pdCurrentL= 0.;
		      ++pdCurrentArg;
		    }
		}
	    }
	}
      /* init U */
      pdCurrentU= pdblUData;
      pdCurrentArg= pData;
      for( j= 0; j != iCols; ++j)
	{
	  pdCurrentArg= pData + j * iMinRowsCols * (complexArg ? 2 : 1);
	  for ( i=0; i!= iMinRowsCols; ++i, ++pdCurrentU, ++pdCurrentArg)
	    {
	      if( i <= j)
		{
		  *pdCurrentU = *pdCurrentArg;
		  if(complexArg)
		    { /* copy imaginary part */
		      *++pdCurrentU = *++pdCurrentArg;
		    }
		}
	      else
		{
		  *pdCurrentU= 0.;
		  if(complexArg)
		    {
		      *++pdCurrentU= 0.;
		      ++pdCurrentArg;
		    }
		}
	    }
	}
      if( pdblEData == NULL)
	{
	  for(i=0; i != iRows; ++i)
	    {
	      piWork[i]=i;
	    }
	  for(i=0; i != iMinRowsCols; ++i)
	    {
	      int ip;
	      ip= piPivot[i]-1; /* -1 because piPivot contains Fortran index*/
	      if( ip != i) 
		{
		  int swapTmp;
		  swapTmp= piWork[i];
		  piWork[i]= piWork[ip];
		  piWork[ip]= swapTmp;
		}
	    }
	  for(i= 0; i != iRows; ++i)
	    {
	      int ip;
	      ip=piWork[i];
	      if(complexArg)
		{
		  C2F(zcopy)(&iCols, pdblWork+(i*2), &iRows, pdblLData+(ip*2), &iRows); 
		}
	      else
		{
		  C2F(dcopy)(&iCols, pdblWork+i, &iRows, pdblLData+ip, &iRows);
		}
	    }
	}/* end if E was not requested */
      else
	{
	  double dblZero=0., dblOne=1.;
	  int iOne=1;
	  F2C(dlaset)("F", &iRows, &iRows, &dblZero, &dblOne, pdblEData, &iRows);
	  F2C(dlaswp)(&iRows, pdblEData, &iRows, &iOne, &iMinRowsCols, piPivot, &iOne);
	}
    }/* end if iInfo reported an error in [z|d]getrf */
  return ret;
}
