/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY 
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "api_double.h"
#include "api_common.h"
#include "api_string.h"

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "localization.h"

#include "svd.h"

/*
  s=svd(X): [R: min(rows, cols) x 1]
  [U,S,V]=svd(X) [U,S,V]:  [ [C|R: rows x rows], [R: rows x cols ],  [R|C: cols x cols ] ]
  [U,S,V]=svd(X,0) (obsolete) [U,S,V]=svd(X,"e"): [ [C|R:rows x min(rows,cols)], [R: min(rows,cols) x min(rows,cols)], [C|R:cols x min(rows,cols)] ]
  [U,S,V,rk]=svd(X [,tol]) : cf. supra, rk[R 1 x 1]

  /!\ Contrary to specifications (@ http://www.scilab.org/product/man/svd.html )
  , previous version was accepting Lhs==2. Worse : tests were using this undocumented behavior.
  implementation and tests have been fixed according to the specification.

*/
static int copyStrArrayArg(int* varAdr, int* pRows, int* pCols, char*** strArrayArg );
static void freeStrArray(char** strArray, int size);

extern int C2F(vfinite)(int *n, double *v);

static int isEconomyMode(int*const arg2);

static int handleEmptyMatrix(void);

static int allocU_S_V(int rows, int cols, int economyRows, int economyCols, int isComplex, double** ptrsU, double** pS, double** ptrsV);


int C2F(intsvd)(char *fname,unsigned long fname_len)
{
  int ret=0, economy=0, complexArg, iRows, iCols;
  double* pData= NULL;

  double* pSV= NULL;

  /* ptrs to data : either
     Real -> ptrsU[0] = ptr do double data
     Complex -> ptrsU[0]= ptr to doublecomplex data, ptrsU[1]= ptr to real (double) data and ptrsU[2]= ptr to imaginary (double) data */
  double* ptrsU[3]= {NULL, NULL, NULL}; 

  double* pS= NULL;
  /* cf. ptrsU */
  double* ptrsV[3]= {NULL, NULL, NULL};

  double tol= 0.;
  double* pRk= NULL;

  int* adr1= NULL;
  int* adr2= NULL;
  if ( Rhs >=1 )
    {
      getVarAddressFromPosition(1, &adr1);
      if(getVarType(adr1) != sci_matrix)
	{
	  OverLoad(1);
	  return 0;
	}
      else
	{
	  CheckRhs(1,2);
	  CheckLhs(1, 4);
	  if(Rhs == 2)
	    {
	      getVarAddressFromPosition(2, &adr2);
	      economy= isEconomyMode(adr2);
	    }
	  if( (complexArg= isVarComplex(adr1)) )
	    {
	      getComplexZMatrixOfDouble(adr1, &iRows, &iCols, ((doublecomplex**)&pData));
	      if(!pData)
		{
		  Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		  ret = 1;
		}
	    }
	  else
	    {
	      getMatrixOfDouble(adr1, &iRows, &iCols, &pData);
	    }
	  if(iRows == 0) /* empty matrix */
	    {
	      handleEmptyMatrix();
	    }
	  else
	    {
	      if((iRows == -1) || (iCols== -1))
		{
		  Scierror(271,_("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 1);
		  ret= 1;
		}
	      else
		{
		  int const totalsize= iRows * iCols * (complexArg ? 2 : 1); 
		  if(!C2F(vfinite)(&totalsize, pData))
		    {
		      Scierror(264,_("Wrong value for argument %d: Must not contain NaN or Inf.\n"), 1);
		      ret= 1;
		    }
		  else
		    {
		      if(Lhs >= 3)
			{
			  int const economyRows= economy ? Min(iRows, iCols) : iRows;
			  int const economyCols= economy ? Min(iRows, iCols) : iCols;

			  allocU_S_V(iRows, iCols, economyRows, economyCols, complexArg, ptrsU, &pS, ptrsV);

			  if(Lhs == 4)
			    {
			      if( (Rhs == 2 ) && (getVarType(adr2) == sci_matrix)) /*  getVarAddressFromPosition(2, &adr2) was already called */
				{
				  int dummy; /* original code does not check iRows == iCols == 1 */
				  double* tmpData;
				  getMatrixOfDouble(adr2, &dummy, &dummy, &tmpData);
				  tol= *tmpData;
				}
			      allocMatrixOfDouble(Rhs+4, 1, 1, &pRk);

			    }
			}
		      else
			{
			  if(Lhs == 2)
			    {
			      Scierror(78,_("%s: Wrong number of output arguments.\n"),fname);
			    }
			  else /* Lhs == 1 */
			    {
			      allocMatrixOfDouble(Rhs+1, Min(iRows, iCols), 1, &pSV);
			    }
			}
		      ret=  iSvdM(pData, iRows, iCols, complexArg, economy, tol, pSV, ptrsU[0], pS, ptrsV[0], pRk);
		      if(ret){
			if( ret == -1)
			  {
			    Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
			  }
			else
			  {
			    Scierror(24,_("Convergence problem...\n"));
			  }
		      }
		      else
			{
			  if(complexArg)
			    {
			      vFreeDoubleComplexFromPointer((doublecomplex*)pData);
			      if(Lhs != 1)
				{ /* TODO voir comment double_api.h gère ça ! */
				  {/* multicore: omp sections */
				    vGetPointerFromDoubleComplex((doublecomplex*)ptrsU[0], iRows* (economy ? Min(iRows, iCols) : iRows)
								 , ptrsU[1], ptrsU[2]);
				    FREE(ptrsU[0]);
				  }
				  {
				    vGetPointerFromDoubleComplex((doublecomplex*)ptrsV[0], iCols* (economy ? Min(iRows, iCols) : iCols)
								 , ptrsV[1], ptrsV[2]);
				    FREE(ptrsV[0]);
				  }
				}
			    }
			  {
			    int i;
			    for(i= Lhs; i != 0; --i)
			      {
				LhsVar(i)= Rhs + i;
			      }
			  }
			}
		    }
		}
	    }
	}
    }
  return ret;
}
/*--------------------------------------------------------------------------*/
int copyStrArrayArg(int* varAdr, int* pRows, int* pCols, char*** strArrayArg )
{
  int * lengths=NULL;
  int ret=0;

  getMatrixOfString(varAdr, pRows, pCols, NULL, NULL);
  {
    int i;
    int size= *pRows * *pCols;
    lengths= MALLOC(size * sizeof(int));
    getMatrixOfString(varAdr, pRows, pCols, lengths, NULL);  
    *strArrayArg= MALLOC(size * sizeof(char*));
    for(i=0; i != size; ++i)
      {
	(*strArrayArg)[i]= MALLOC((lengths[i]+1)*sizeof(char));
      }
  }
  getMatrixOfString(varAdr, pRows, pCols, lengths, *strArrayArg);  
  FREE(lengths);
  return ret;
}

void freeStrArray(char** strArray, int size)
{
  
  int i;
  for(i=0; i != size; ++i)
    {
      FREE(strArray[i]);
    }
  FREE(strArray);
  
}

int isEconomyMode(int*const adr2)
{
  int economy= 0;
  if(adr2)
    {
      switch(getVarType(adr2))
	{
	case sci_strings:
	  {
	    char** strArray;
	    int rows, cols;
	    copyStrArrayArg(adr2, &rows, &cols, &strArray);
	    economy= (strArray[0][0]=='e') && (strArray[0][1]=='\0'); /* "e" */
	    freeStrArray(strArray, rows*cols);
	    break;
	  }
	case sci_matrix:
	  {
	    economy= (Lhs == 3); /* no further testing for "old Economy size:  [U,S,V]=svd(A,0) " */
	    break;
	  }
	}
    }
  return economy;
}

int handleEmptyMatrix(void)
{
  double* data;

  allocMatrixOfDouble(3, 0, 0, &data);
  LhsVar(1)= 3;
  if(Lhs >= 2)
    {
      allocMatrixOfDouble(4, 0, 0, &data);
      LhsVar(2)= 4;
    }
  if(Lhs >=3)
    {
      allocMatrixOfDouble(5, 0, 0, &data);
      LhsVar(3)= 5;
    }
  if(Lhs == 4)
    {
      allocMatrixOfDouble(6, 1, 1, &data);
      *data= 0.;
      LhsVar(4)= 6;
    }
  return 0;
}

static int allocU_S_V(int rows, int cols, int economyRows, int economyCols, int isComplex, double* ptrsU[], double** ptrS, double* ptrsV[])
{
  if(isComplex)
    {
      allocComplexMatrixOfDouble(Rhs+1, rows, economyRows, ptrsU+1, ptrsU+2); 
      ptrsU[0]= (double*)MALLOC(rows * economyRows*(isComplex ? sizeof(doublecomplex): sizeof(double)));
      allocMatrixOfDouble(Rhs+2, economyRows, economyCols, ptrS);
      allocComplexMatrixOfDouble(Rhs+3, cols, economyCols, ptrsV+1, ptrsV+2);
      ptrsV[0]= (double*)MALLOC(cols * economyCols*(isComplex ? sizeof(doublecomplex): sizeof(double)));
    }
  else
    {
      ptrsU[1]= ptrsU[2]= NULL;
      allocMatrixOfDouble(Rhs+1, rows, economyRows, ptrsU+0);
      allocMatrixOfDouble(Rhs+2, economyRows, economyCols, ptrS);
      ptrsV[1]= ptrsV[2]= (double*)NULL;
      allocMatrixOfDouble(Rhs+3, cols, economyCols , ptrsV+0);
    }
  return 0;
}
