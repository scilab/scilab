
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "doublecomplex.h"
#include <stdio.h>
#include "stack-c.h"
#include "MALLOC.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"

#include "sas_schur.h"

/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int *num);
/*--------------------------------------------------------------------------*/


/*
 * [[VS],T] = schur(A, 'type') : R or C matrix VS orthogonal or unitary square matrix 'd'|'z' N x N, T: reuse arg 1
 * [U,dim [,T] ]=schur(A,flag) 'c' or 'd'
 * [Z,dim] = schur(A,E,flag)
 * [As,Es [,Q,Z]]=schur(A,E) : R or C matrix same dimensions reuse arg1 and arg2, create 'd'|'z' NxN for Q, Z
 * 
 * [As,Es [,Q],Z,dim] = schur(A,E,flag) : 


 * [U,dim [,T] ]=schur(A,extern1) Not Implemented
 *
 * [As,Es [,Q],Z,dim]= schur(A,E,extern2)
 * [Z,dim]= schur(A,E,extern2)
 *
 *
 */

int C2F(intschur)(char *fname,unsigned long fname_len)
{
  int ret= 0;
  if( (Rhs >= 1) && (GetType(1) != sci_matrix))
    {
      OverLoad(1);
      return 0;
    }
  if( (Rhs >= 3) && (GetType(2) != sci_matrix))
    {
      OverLoad(2);
      return 0;
    }
  CheckRhs(1, 3);
  CheckLhs(1, 5); /* doc says min Lhs == 2, but unit tests disagree :( */
  {
    int isRealStr=0, isCplxStr=0;
    int twoMatrices= (Rhs >= 2) && (GetType(2)==sci_matrix) ;
    int optionPlace= (Rhs==3) ? 3 : ( ( (Rhs==2) && !twoMatrices) ? 2 : 0 /* no option arg */) ;
    if( optionPlace )
      {
	switch( GetType(optionPlace) )
	  {
	  case sci_c_function :
	    {
	      Scierror(999,(stderr, "custom c function Not Implemented Yet\n"));
	      ret= 1;
	      break;
	    }
	  case sci_strings :
	    {
	      int rows, cols, i;
	      char* str;
	      
	      GetRhsVar(optionPlace, STRING_DATATYPE, &rows, &cols, &i);
	      str=cstk(i);
	      isRealStr= (strcmp(str, "r")==0) || (strcmp(str, "real")==0);
	      isCplxStr= (strcmp(str, "comp")==0) || (strcmp(str, "complex")==0);
	      if(!(isRealStr || isCplxStr))
		{
		  Scierror(999,(stderr, "custom external function Not Implemented Yet\n"));
		  ret= 1;
		}
	      break;
	    }
	  default :
	    {
	      Scierror(999,_( "%s: Wrong type for input argument #%d: A function expected.\n"), fname, optionPlace);
	      ret= 1;
	      break;
	    } 
	  }
      }
    if(!ret)
    {
      double* pData[2]= {NULL, NULL};
      double* pDataReal[2]= {NULL, NULL};
      double* pDataImg[2]= {NULL, NULL};
      int iRows[2], iCols[2];
      int i;
      
      int complexArgs= iIsComplex(1) || ( twoMatrices && iIsComplex(2)) || isCplxStr ;
    
    for(i=0; i != (twoMatrices ? 2 : 1); ++i)
      {
	int iPlus1= i+1;
	if(complexArgs)
	  {
	    C2F(complexify)(&iPlus1);
	    GetRhsVarMatrixComplex(iPlus1, &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
	    pData[i]= iCols[i] ? (double*)oGetDoubleComplexFromPointer( pDataReal[i], pDataImg[i], iRows[i] * iCols[i]) : NULL;
	  }
	else
	  {
	    GetRhsVarMatrixDouble(iPlus1, &iRows[i], &iCols[i], &pData[i]);
	  }
	if(iRows[i] != iCols[i])
	  {
	    Scierror(999,_("%s: Wrong type for input argument #%d: Square matrix expected.\n"), fname, iPlus1);
	    ret= -2;
	  }
	if( (i==1) && (iRows[0] != iRows[1]))
	  {
	    Scierror(999,_("%s: Wrong size for input arguments #%d and #%d: Same dimensions expected.\n"), fname, 1,2);
	    ret= -2;
	  }
      }
    if(!ret)
    {
      if(twoMatrices)
	{
	  CheckLhs(2, 4);
	}
      else
	{
	  CheckLhs(1, 2);
	}
    /*
      check for invalid args.
    */
    if(isRealStr && complexArgs)
      {
	Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),fname, 1);
	ret= 1;
      }
    else
      {
	double* pLhsOpt[2]= {NULL, NULL};
	double* pLhsOptReal[2]= {NULL, NULL};
	double* pLhsOptImg[2]= {NULL, NULL};
	for(i=0; i!= Lhs- (twoMatrices ? 2 : 1); ++i)/* Rhs matrix are reused as Lhs, so we only need to alloc Lhs-Rhs variables */
	  {
	    int dummy= complexArgs 
	      ? ( pLhsOpt[i]= (iCols[0] ? (double*)MALLOC( iCols[0] * iCols[0] * sizeof(doublecomplex)) : NULL)) /* MALLOC(0) is invalid :( */
	      ,iAllocComplexMatrixOfDouble(Rhs+1+i, iCols[0], iCols[0] , &pLhsOptReal[i], &pLhsOptImg[i])
	      : iAllocMatrixOfDouble(Rhs+1+i , iCols[0], iCols[0], &pLhsOpt[i]) ;
	  }
	ret= iCols[0] ? iSchurM(pData[0], pData[1], iCols[0], complexArgs, pLhsOpt[0], pLhsOpt[1]) : 0;
	if(ret == -1)
	  {
	  Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
	  }
	else if( ret > 0)
	  {
	    if(twoMatrices)
	      {
		if(ret <= iCols[0])
		  {
		    sciprint(_("Warning :\n"));
		    sciprint(_("Non convergence in the QZ algorithm.\n"));
		    sciprint(_("The top %d  x %d blocks may not be in generalized Schur form.\n"), ret, ret);
		  }
		else
		  {
		    Scierror(999,_("%s: Convergence error.\n"), fname);
		  }
	      }
	    else
	      {
		sciprint(_("Warning :\n"));
		sciprint(_("%s: Non convergence in QR steps.\n"), fname);
		sciprint(_("The top %d x %d block may not be in Schur form.\n"), ret, ret);
	      }
	  }
      	for(i=0; i!=2; ++i)
	  {
	    if(pDataReal[i]) /* the Rhs was a complex matrix, must put back the complex results to use it as a Lhs */
	      {
		vGetPointerFromDoubleComplex((doublecomplex*)(pData[i]), iCols[0] * iCols[0], pDataReal[i], pDataImg[i]);
		FREE(pData[i]);
	      }
	    if(pLhsOptReal[i]) /* the lhs was allocated as a complex matrix */
	      {
		vGetPointerFromDoubleComplex((doublecomplex*)(pLhsOpt[i]), iCols[0] * iCols[0], pLhsOptReal[i], pLhsOptImg[i]);
		FREE(pLhsOpt[i]);
	      }
	  }
      }
    }
    }
    if( (Lhs==2) && !twoMatrices)/* special case */
      {
	LhsVar(2)= 1;
	LhsVar(1)= Rhs+1;
      }
    else
      {
	switch(Lhs)
	  {
	  case 4: LhsVar(4)= Rhs+2; /* no break */
	  case 3: LhsVar(3)= Rhs+1; /* no break */
	  case 2: LhsVar(2)= twoMatrices ? 2 : Rhs+1 ;/* no break */
	  case 1 : LhsVar(1)= 1;
	  }
      }
  }
  return ret;
}
