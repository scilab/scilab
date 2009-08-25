
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
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
#include "stack3.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "doublecomplex.h"
#include "localization.h"

#include "api_double.h"
#include "api_common.h"


#include "balanc.h"
/*--------------------------------------------------------------------------*/
extern int C2F(complexify)(int const *num);
/*--------------------------------------------------------------------------*/
int C2F(intbalanc)(char *fname,unsigned long fname_len)
{
  int ret=0;
  int* arg[2]= {NULL, NULL};

  doublecomplex* pData[2]= {NULL, NULL};
  double* pDataReal[2]= {NULL, NULL};
  double* pDataImg[2]= {NULL, NULL};
  int iCols[2], iRows[2];
  
  CheckRhs(1,2);
  CheckLhs(2*Rhs, 2*Rhs);
  
  if(Rhs >=1)
    {
      getVarAddressFromPosition(1, &arg[0]);
      if(getVarType(arg[0])!=sci_matrix)
	{
	  OverLoad(1);
	  return 0;
	}
      { /* Getting Rhs, either in pDataReal[] or pData[] in 'z' format */
	int i;
	int complexArgs= isVarComplex(arg[0]);
	if(Rhs==2)
	  {
	    getVarAddressFromPosition(2, &arg[1]);
	    if (getVarType(arg[1])!=sci_matrix) 
	      {
		OverLoad(2);
		return 0;
	      }
	    complexArgs = complexArgs || isVarComplex(arg[1]);
	  }
	for( i =0; i!=Rhs; ++i)
	  {
	    int const iPlus1=i+1;
	    if(complexArgs)
	      {
		C2F(complexify)(&iPlus1);
		getComplexMatrixOfDouble(arg[i], &iRows[i], &iCols[i], &pDataReal[i], &pDataImg[i]);
		if( !(pData[i]= oGetDoubleComplexFromPointer( pDataReal[i], pDataImg[i], iRows[i] * iCols[i])) )
		  {
		    Scierror(999,_("%s: Cannot allocate more memory.\n"),fname);
		    ret = 1;
		  }
	      }
	    else
	      {
	    	getMatrixOfDouble(arg[i],  &iRows[i], &iCols[i], &pDataReal[i]);
	      }
	    if(iCols[i] != iRows[i])
	      {
		Scierror(999,_("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, i+1);
		ret=-1;
	      }
	  }
	if( (Rhs==2) && (iCols[0] != iCols[1]) )
	  {/* /!\ reusing existing error msg, but it could be more explicit :"%s: arguments %d and %d must have equal dimensions.\n" */
	    Scierror(999,_("%s and %s must have equal dimensions.\n"),"A","B");
	    ret= -1;
	  }
	if(!ret)
	  {
	    /* Allocating variables for lhs   */
	    {
	      double* lhsData[2]= {NULL, NULL};
	      for(i=0; i!=Rhs; ++i)
		{
		  allocMatrixOfDouble(Rhs+i+1, iCols[0], iCols[0], &lhsData[i]) ;
		}
	      if(iCols[0] != 0)
		{
		  /* calling function and converting result to 'c' format if needed */
		  if( complexArgs )
		    {
		      ret= iBalancM((double*)(pData[0]), (double*)(pData[1]), iCols[0], complexArgs, lhsData[0], lhsData[1]);
		      for(i=0; i!=Rhs; ++i)
			{
			  vGetPointerFromDoubleComplex(pData[i], iCols[0]* iCols[0], pDataReal[i], pDataImg[i]);
			  vFreeDoubleComplexFromPointer(pData[i]);
			}
		    }
		  else
		    {
		      iBalancM(pDataReal[0], pDataReal[1], iCols[0], complexArgs, lhsData[0], lhsData[1]) ;
		    }
		}
	    }
	  }
	/* putting Lhs back on stack */
	for(i=1; i <= Lhs; ++i)
	  {
	    LhsVar(i)= i;
	  }
      }
    }
  return ret;
}
