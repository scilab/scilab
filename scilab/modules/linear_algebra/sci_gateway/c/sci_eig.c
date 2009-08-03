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

#include "api_common.h"
#include "api_double.h"

#include <string.h>
#include "stack-c.h"
#include "issymmetric.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "vfinite.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "eigen.h"
/*
  evals=spec(A)
  [R,diagevals]=spec(A)

  evals=spec(A,B)
  [alpha,beta]=spec(A,B)
  [alpha,beta,Z]=spec(A,B)
  [alpha,beta,Q,Z]=spec(A,B)
*/
extern int C2F(complexify)(int const*num);

/* internal function used to detect imaginary part completly and strictly equal to 0. causing troubles to LAPACK. */  
static int isArrayZero(int size , double * values)
{
  while(--size)
    {
      if(*values!=0.) /* not a bug: only 0. is of interest, not epsilon */
	{
	  return 0;
	}
    }
  return 1;
}

#define STACK3

int C2F(inteig)(char *fname,unsigned long fname_len)
{
  int* arg[2]= {NULL, NULL};
  int ret=0;
  CheckRhs(1,2);
  CheckLhs(1,4);
  if(Rhs >= 1)
    {
      getVarAddressFromPosition(1, &arg[0]);
      if(getVarType(arg[0]) != sci_matrix)
	{
	  OverLoad(1);
	  ret= 0;
	}
      else
	{
	  switch (Rhs) {
	  case 1:{
	    int iRows, iCols;
	    doublecomplex* pData= NULL;
	    double* pDataReal= NULL;
	    double* pDataImg= NULL;
	    int complexArg= isVarComplex(arg[0]);
	    CheckLhs(1,2);
	    if(complexArg)
	      {
#ifdef STACK3
		GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
#else
		getComplexMatrixOfDouble(arg[0], &iRows, &iCols, &pDataReal, &pDataImg);
#endif
		/* c -> z */
		pData=oGetDoubleComplexFromPointer( pDataReal, pDataImg, iRows * iCols);
		if(!pData)
		  {
		    Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		    ret = 1;
		  }
	      }
	    else
	      {
#ifdef STACK3
		GetRhsVarMatrixDouble(1, &iRows, &iCols, &pDataReal);
#else

		getMatrixOfDouble(arg[0], &iRows, &iCols, &pDataReal);
#endif

	      }
	    if( iCols == -1)
	      {
		Scierror(999,_("Size varying argument a*eye(), (arg %d) not allowed here.\n"), 1);
	      }
	    else /* not eye() matrix */
	      {
		if (iCols==0)
		  {
		    if(Lhs == 1)
		      {
			LhsVar(1)= 1;
		      }
		    else /* Lhs == 2 */
		      {
			double* dummy;
			int unused = complexArg 
			  ? allocComplexMatrixOfDouble(2, iCols, iCols, &dummy, &dummy)
			  : allocMatrixOfDouble(2, iCols, iCols, &dummy);

			LhsVar(1)= 2;
			LhsVar(2)= 1;
		      }
		  }
		else
		  {/* now the interesting case, can be either complex or real and symmetric or not */
	    
		    double* pEigenValuesReal= NULL;
		    double* pEigenValuesImg= NULL;
		    double* pEigenVectorsReal= NULL;
		    double* pEigenVectorsImg= NULL;
		    int symmetric= 0; /* bool in fact */
		    int const eigenValuesCols= (Lhs==1) ? 1 : iCols ;
		    int const totalSize= iRows * iCols;
		    if ( !(complexArg 
			   ? C2F(vfiniteComplex)(&totalSize, pData) 
			   : C2F(vfinite)(&totalSize, pDataReal)))
		      {
			Scierror(264,_("Wrong value for argument %d: Must not contain NaN or Inf.\n"),1);
			return 0;
		      }
		    if( (symmetric=C2F(issymmetric)(&Rhs) ) )
		      {
			allocMatrixOfDouble(2, iCols, eigenValuesCols, &pEigenValuesReal);
			/* if matrix is symmetric, the eigenvectors can reuse Rhs because the matrix is of the same type & dimensions */
		      }
		    else
		      {
			allocComplexMatrixOfDouble(2, iCols, eigenValuesCols, &pEigenValuesReal, &pEigenValuesImg);
			if(Lhs==2)
			  {
			    allocComplexMatrixOfDouble(3, iCols, iCols, &pEigenVectorsReal, &pEigenVectorsImg);
			  }
		      }
		    if( complexArg )
 		      {
			if(symmetric)
			  {
			    ret= iEigen1ComplexSymmetricM(pData, iCols, (Lhs==2), pEigenValuesReal);
			    if(Lhs == 2)
			      { /* we reuse memory from rhs1 to put back the resulting eigenvectors */
				vGetPointerFromDoubleComplex(pData, iCols * iCols , pDataReal, pDataImg);
				expandToDiagonalOfMatrix(pEigenValuesReal, iCols);
				LhsVar(1)= 1;
				LhsVar(2)= 2;
			      }
			    else
			      {
				LhsVar(1)= 2;
			      }
			  }
			else
			  { /* !!!!!!!!!!!!!!!!!!!!!!!!  unsymmectri case : bug */
			    doublecomplex* pEigenValues= (doublecomplex*)MALLOC(iCols * sizeof(doublecomplex));
			    doublecomplex* pEigenVectors = (Lhs== 2) ? (doublecomplex*)MALLOC(sizeof(doublecomplex) * iCols * iCols) : NULL ;
			    ret= iEigen1ComplexM(pData, iCols, pEigenValues, pEigenVectors);
			    if(Lhs==2)
			      {
				expandZToDiagonalOfCMatrix(pEigenValues, iCols, pEigenValuesReal, pEigenValuesImg); 
				vGetPointerFromDoubleComplex(pEigenVectors, iCols * iCols, pEigenVectorsReal, pEigenVectorsImg);
				FREE(pEigenVectors);
				LhsVar(1)= 3;
				LhsVar(2)= 2;
			      }
			    else
			      {
				vGetPointerFromDoubleComplex(pEigenValues, iCols, pEigenValuesReal, pEigenValuesImg);
				LhsVar(1)= 2;
			      }
			    FREE(pEigenValues);
			  }
		      }
		    else /* real */
		      {
			if(symmetric)
			  {
			    ret= iEigen1RealSymmetricM(pDataReal, iCols, (Lhs==2), pEigenValuesReal);
			    if(Lhs == 2)
			      {
				expandToDiagonalOfMatrix(pEigenValuesReal, iCols);
				LhsVar(1)= 1;
				LhsVar(2)=2;
			      }
			    else
			      {
				LhsVar(1)=2;
			      }
			  }
			else
			  {
			    ret= iEigen1RealM(pDataReal, iCols, pEigenValuesReal, pEigenValuesImg, pEigenVectorsReal, pEigenVectorsImg);
			    if( Lhs == 2)
			      {
				expandToDiagonalOfMatrix(pEigenValuesReal, iCols);
				expandToDiagonalOfMatrix(pEigenValuesImg, iCols);
				LhsVar(1)= 3;
				LhsVar(2)= 2;
			      }
			    else
			      {
				LhsVar(1)= 2;
			      }
			  }
		      }
		  }
	      }
	    if(complexArg)
	      {
		vFreeDoubleComplexFromPointer(pData);
	      }
	    break;
	  }
	  case 2:{
	    getVarAddressFromPosition(2, &arg[1]);
	    if(getVarType(arg[1]) != sci_matrix)	
	      {
		OverLoad(2);
		ret= 0;
	      }
	    {
	      doublecomplex* pData[2];
	      double* pDataReal[2];
	      double* pDataImg[2];
	      int iCols[2], iRows[2];
	      int i;
	      /* Because of bug #3652 ( http://bugzilla.scilab.org/show_bug.cgi?id=3652 ), complex matrix with both imaginary part null
	       * must be handled as real matrix.
	       */
	      int complexArgs= isVarComplex(arg[0]) || isVarComplex(arg[1]);
	      /* if either matrix is complex, promote the other to complex */
	      if(complexArgs)
		{
		  int byRef=1;
		  C2F(complexify)(&byRef);
		  byRef=2;
		  C2F(complexify)(&byRef);
		}
	      if(complexArgs)
		{
		  for(i=0; i!=2; ++i)
		    {

#ifdef STACK3
		      GetRhsVarMatrixComplex(1+i, &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
#else
		      getComplexMatrixOfDouble(arg[i], &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
#endif
		      if( !(pData[i]= oGetDoubleComplexFromPointer( pDataReal[i], pDataImg[i], iRows[i] * iCols[i])) )
			{
			  Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
			  ret = 1;
			}
		    }
		}
	      else
		{
		  for(i=0; i!=2; ++i)
		    {
#ifdef STACK3
		      GetRhsVarMatrixDouble(1+i, &iRows[i], &iCols[i], &pDataReal[i]);
#else
		      getMatrixOfDouble(arg[i], &iRows[i], &iCols[i], &pDataReal[i]);
#endif
		    }
		}
	      for( i=0; i!=2; ++i)
		{
		  if (iRows[i]!= iCols[i])
		    {
		      Scierror(999,_("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, i+1);
		    }
		}
	      if( iCols[0] != iCols[1])
		{/* /!\ reusing existing error msg, but it could be more explicit :"%s: arguments %d and %d must have equal dimensions.\n" */
		  Scierror(999,_("%s and %s must have equal dimensions.\n"),"A","B");
		}
	      if( iCols[0] == 0)
		{
		  switch (Lhs){
		  case 4:{
		    double* dummy;
		    if(complexArgs)
		      {
			allocComplexMatrixOfDouble(4, 0, 0, &dummy, &dummy);
		      }
		    else
		      {
			allocMatrixOfDouble(4, 0, 0, &dummy) ;
		      }
		    LhsVar(4)= 4; /* no break */
		  }
		  case 3:{
		    double* dummy;
		    if(complexArgs)
		      {
			allocComplexMatrixOfDouble(3, 0, 0, &dummy, &dummy);
		      }
		    else
		      {
			allocMatrixOfDouble(3, 0, 0, &dummy) ;
		      }
		    LhsVar(3)= 3;
		  }/* no break */
		  case 2:
		    LhsVar(2)= 2; /* no break */
		  default: /* case 1: as we have done CheckLhs(1,4)*/
		    LhsVar(1)= 1; 
		  }
		}
	      else
		{
		  int inf=0;
		  int const totalSize= iCols[0] * iCols[0];
		  for(i=0; i!=2 && !inf; ++i)
		    {
		      if( (inf= (inf || !(complexArgs 
					  ? C2F(vfiniteComplex)(&totalSize, pData[i]) 
					  : C2F(vfinite)(&totalSize, pDataReal[i]) ) )))
			{/* /!\ reusing error msg, but could be more explicit ny being prefixed by %s for fname */
			  Scierror(264, _("Wrong value for argument %d: Must not contain NaN or Inf.\n"),i+1);
			}
		    }
		  if(!inf)
		    {
		      doublecomplex* pL= NULL;
		      double* pLReal= NULL;
		      double* pLImg= NULL;
		      doublecomplex* pR= NULL;
		      double* pRReal= NULL;
		      double* pRImg= NULL;
		      doublecomplex* pAlpha= NULL;
		      double* pAlphaReal= NULL;
		      double* pAlphaImg= NULL;
		      doublecomplex* pBeta= NULL;
		      double* pBetaReal= NULL;
		      double* pBetaImg= NULL;
		      /*
			special case for complex matrix with null imaginary parts.
		      */
		      int complexArgsWithZeroImg= isArrayZero(totalSize, pDataImg[0]) &&  isArrayZero(totalSize, pDataImg[1]);
		      /* API could be better wrt c or z format of L and R but with less code ruse from assembleEigenVectors */
		      switch(Lhs){/* for Complex case, it could be possible to reuse var from Rhs1 & Rhs2 for Lhs 3 & Lhs4 */
		      case 4:
			allocComplexMatrixOfDouble(6, iCols[0], iCols[0], &pLReal, &pLImg);  /* nobreak */
		      case 3:
			allocComplexMatrixOfDouble(5, iCols[0], iCols[0], &pRReal, &pRImg); /* nobreak */
		      case 2:{
			if(complexArgs && !complexArgsWithZeroImg)
			  {
			    pBeta= allocComplexMatrixOfDouble(4, iCols[0], 1, &pBetaReal, &pBetaImg)
			      ? NULL /* var alloc failed, no need to try to alloc 'z' vector */
			      : (doublecomplex*)MALLOC(iCols[0] * sizeof(doublecomplex));

			  }
			else
			  {
			    allocMatrixOfDouble(4, iCols[0], 1, &pBetaReal);
			  }
		      } /* nobreak */
		      default : /*case 1 as we have done CheckLhs(1,4) */
			{ /* note that real matrix use alpha in 'c' format while complex matrix use it in 'z' format, cf dggev and zggev */
			  pAlpha = ( allocComplexMatrixOfDouble(3, iCols[0], 1, &pAlphaReal, &pAlphaImg) 
				     || (!complexArgs || complexArgsWithZeroImg) )
			    ? NULL /* var alloc failed or we are handling real matrix */
			    : (doublecomplex*) MALLOC(iCols[0] * sizeof(doublecomplex)) ;
			}
			/* nobreak */
		      }
		      ret= (complexArgs && !complexArgsWithZeroImg)
			? iEigen2ComplexM(pData[0], pData[1], iCols[0], pAlpha, pBeta, pR, pL)
			: iEigen2RealM(pDataReal[0], pDataReal[1], iCols[0], pAlphaReal, pAlphaImg, pBetaReal, pRReal, pRImg, pLReal, pLImg) ;
		      if(ret >0 )
			{
			  sciprint(_("Warning :\n"));
			  sciprint(_("Non convergence in the QZ algorithm.\n"));
			  sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"), ret);
			}
		      if( ret >= 0)
			{
			  switch (Lhs) 
			    {
			    case 4:
			      if(complexArgs && !complexArgsWithZeroImg)
				{
				  vGetPointerFromDoubleComplex(pL, totalSize, pLReal, pLImg);
				}/* nobreak */
			    case 3:
			      {
				if(complexArgs && !complexArgsWithZeroImg)
				  {
				    vGetPointerFromDoubleComplex(pR, totalSize, pRReal, pRImg);
				  }
				if(Lhs == 4)
				  {
				    LhsVar(4)= 5;
				    LhsVar(3)= 6;
				  }
				else /* Lhs == 3 */
				  {
				    LhsVar(3)= 5;
				  }
			      }/* nobreak */
			    case 2:
			      {
				if(complexArgs && !complexArgsWithZeroImg)
				  {
				    vGetPointerFromDoubleComplex(pBeta, iCols[0], pBetaReal, pBetaImg);
				  }
				LhsVar(2)= 4; /* nobreak */
			      }
			    default: /* case 1 */
			      {
				if(complexArgs)
				  {
				    vGetPointerFromDoubleComplex(pAlpha, iCols[0], pAlphaReal, pAlphaImg);
				  }
				LhsVar(1)= 3;
			      }
			    }
			  
			}
		      /* adjust*/
		      FREE(pAlpha);
		      FREE(pBeta);
		    }
		  if(complexArgs)
		    {
		      vFreeDoubleComplexFromPointer(pData[0]);
		      vFreeDoubleComplexFromPointer(pData[1]);
		    }

		}
	    }
	
	  }
	    break;/* end case 2 */
	
	  }/* end switch (Rhs) */
	}
    }
  return 0;
}
/*--------------------------------------------------------------------------*/
