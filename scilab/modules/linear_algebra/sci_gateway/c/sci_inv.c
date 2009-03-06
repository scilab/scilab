
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
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "localization.h"

#include "stack3.h" /* */
#include "Scierror.h"
#include "invert_matrix.h"


int C2F(intinv)(char *fname,unsigned long fname_len)
{
	int ret = 0;
	if(
	/*   inv(A)  */
	if ( Rhs >=1 && GetType(1)!=sci_matrix) 
	{
		OverLoad(1);
		return 0;
	}
	fprintf(stdout, "rhs 1:@%u 2:@%u 3:@%u\n",GetData(1), GetData(2), GetData(3));
	
	// from now on, we have an sci_matrix, so we can use iIsComplex
	CheckRhs(1,1); /* one and only one arg */
	CheckLhs(1,1); /* one and only one returned value */
	{
	int iRows, iCols;
	double* pData;
	double* pDataReal;
	double* pDataImg;
	int complexArg=iIsComplex(1);
	if(complexArg){
	  /* original code in intzgetri uses getrhsvar('c'), NOT getrhscvar ! */
	  /* on crée une copie en format 'z' de la rhs var */
	  GetRhsVarMatrixComplex(1, &iRows, &iCols, &pDataReal, &pDataImg);
	  /* c -> z */
	  pData=(double*)oGetDoubleComplexFromPointer( pDataReal, pDataImg, iRows * iCols);
	}else{
	  GetRhsVarMatrixDouble(1, &iRows, &iCols, &pData);
	}
	if(iRows != iCols){
	  Err = 1;
	  ret=20;
	}else{
	  if(iCols != 0){
	    if( iCols == -1){
	      *pData = 1./(*pData);
	    }else{
	      double dblRcond;
	      ret = iInvertMatrixM(iRows, iCols, pData, complexArg, &dblRcond);
	      if(complexArg){
		/* z -> c */
		vGetPointerFromDoubleComplex((doublecomplex*)pData, iCols * iRows, pDataReal, pDataImg);
		vFreeDoubleComplexFromPointer((doublecomplex*)pData);
	      }

	      if(ret ==-1){// warning   "ill conditionned problem" & ' matrix is close to singular or badly scaled"
		Scierror(999, _("%s : matrix is close to singular or badly scaled. rcond = %s\n"), fname, dblRcond);
	      }
	    }
	  }
	}
	if(ret >0){ Error(ret) ; }
	else{
	  LhsVar(1) = 1;
	  // TODO rajouter le PutLhsVar(); quand il sera enlevé du gw_
	}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
