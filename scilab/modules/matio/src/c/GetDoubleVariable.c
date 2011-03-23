/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetMatlabVariable.h"
#include "sci_types.h"
#include "api_scilab.h"

#define MATIO_ERROR if(_SciErr.iErr) \
    {				     \
      printError(&_SciErr, 0);	     \
      return 0;			     \
    }

matvar_t * GetDoubleVariable(int iVar, const char* name, int matfile_version, int * parent, int item_position)
{
  double * realDataAdr = NULL, * complexDataAdr = NULL;
  int rank = 0;
  int *dims = NULL;
  int isComplex = 0;
  struct ComplexSplit mat5ComplexData;
  matvar_t *createdVar = NULL;
  int * var_addr = NULL;
  int * item_addr = NULL;
  int var_type;
  SciErr _SciErr;

  if (parent==NULL)
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;
      isComplex = isVarComplex(pvApiCtx, var_addr);
    }
  else
    {
      _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, item_addr, &var_type); MATIO_ERROR;
      isComplex = isVarComplex(pvApiCtx, item_addr);
    }

  if(var_type == sci_matrix) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*)MALLOC(sizeof(int)*rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetDoubleVariable");
          return NULL;
        }


      if (isComplex)
	{
	  if (parent==NULL)
	    {
	      _SciErr = getComplexMatrixOfDouble(pvApiCtx, var_addr, &dims[0], &dims[1], &realDataAdr, &complexDataAdr); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = getComplexMatrixOfDoubleInList(pvApiCtx, parent, item_position, &dims[0], &dims[1], 
						       &realDataAdr, &complexDataAdr); MATIO_ERROR;
	    }
	}
      else
	{
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfDouble(pvApiCtx, var_addr, &dims[0], &dims[1], &realDataAdr); MATIO_ERROR;
	    }
	  else
	    {
	      _SciErr = getMatrixOfDoubleInList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &realDataAdr); MATIO_ERROR;
	    }
	}

      if (isComplex==0)
        {
          createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, realDataAdr, 0);
        }
      else
        {
          if(matfile_version==MAT_FT_MAT4) /* MATLAB4: data is a table of value */
            {
              createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, realDataAdr, MAT_F_COMPLEX);
            }
          else /* MATLAB5 file: data is a ComplexSplit */
            {
              mat5ComplexData.Re = realDataAdr;
              mat5ComplexData.Im = complexDataAdr;
              createdVar = Mat_VarCreate(name, MAT_C_DOUBLE, MAT_T_DOUBLE, rank, dims, &mat5ComplexData, MAT_F_COMPLEX);
            }
          createdVar->isComplex = 1;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Double matrix expected.\n"), "GetDoubleVariable");
    }

  FREE(dims);

  return createdVar;
}
