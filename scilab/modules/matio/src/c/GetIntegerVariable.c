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

matvar_t *GetIntegerVariable(int iVar, const char *name, int * parent, int item_position)
{
  int rank = 0;
  int *dims = NULL;
  matvar_t *createdVar = NULL;
  int * var_addr = NULL;
  int var_type;
  int integerType;
  SciErr _SciErr;
  char * tmp_int8 = NULL;
  short * tmp_int16 = NULL;
  int * tmp_int32 = NULL;
  int * item_addr = NULL;
  unsigned char * tmp_uint8 = NULL;
  unsigned short * tmp_uint16 = NULL;
  unsigned int * tmp_uint32 = NULL;
#ifdef __SCILAB_INT64__
  long long * tmp_int64 = NULL;
  unsigned long long * tmp_uint64 = NULL;
#endif

  if (parent==NULL)
    {
      _SciErr = getVarAddressFromPosition(pvApiCtx, iVar, &var_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, var_addr, &var_type); MATIO_ERROR;
    }
  else
    {
      _SciErr = getListItemAddress(pvApiCtx, parent, item_position, &item_addr); MATIO_ERROR;
      _SciErr = getVarType(pvApiCtx, item_addr, &var_type); MATIO_ERROR;
    }

  if (var_type == sci_ints) /* 2-D array */
    {
      rank = 2;
      if ((dims = (int*)MALLOC(sizeof(int)*rank)) == NULL)
        {
          Scierror(999, _("%s: No more memory.\n"), "GetIntegerVariable");
          return NULL;
        }

      if (parent==NULL)
	{
	  _SciErr = getMatrixOfIntegerPrecision(pvApiCtx, var_addr, &integerType);
	}
      else
	{
	  _SciErr = getMatrixOfIntegerPrecision(pvApiCtx, item_addr, &integerType);
	}

      switch(integerType)
        {
        case SCI_INT8: /* INT8 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfInteger8(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_int8);
	    }
	  else
	    {
	      _SciErr = getMatrixOfInteger8InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_int8);
	    }
          createdVar = Mat_VarCreate(name, MAT_C_INT8, MAT_T_INT8, rank, dims, tmp_int8, 0);
          break;
        case SCI_INT16: /* INT16 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfInteger16(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_int16);
	    }
	  else
	    {
	      _SciErr = getMatrixOfInteger16InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_int16);
	    }
          createdVar = Mat_VarCreate(name, MAT_C_INT16, MAT_T_INT16, rank, dims, tmp_int16, 0);
          break;
	case SCI_INT32: /* INT32 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfInteger32(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_int32);
	    }
	  else
	    {
	      _SciErr = getMatrixOfInteger32InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_int32);
	    }
	  createdVar = Mat_VarCreate(name, MAT_C_INT32, MAT_T_INT32, rank, dims, tmp_int32, 0);
	  break;
        case SCI_UINT8: /* UINT8 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfUnsignedInteger8(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_uint8);
	    }
	  else
	    {
	      _SciErr = getMatrixOfUnsignedInteger8InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_uint8);
	    }
          createdVar = Mat_VarCreate(name, MAT_C_UINT8, MAT_T_UINT8, rank, dims, tmp_uint8, 0);
          break;
        case SCI_UINT16: /* UINT16 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfUnsignedInteger16(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_uint16);
	    }
	  else
	    {
	      _SciErr = getMatrixOfUnsignedInteger16InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_uint16);
	    }
          createdVar = Mat_VarCreate(name, MAT_C_UINT16, MAT_T_UINT16, rank, dims, tmp_uint16, 0);
	  break;
	case SCI_UINT32: /* UINT32 */
	  if (parent==NULL)
	    {
	      _SciErr = getMatrixOfUnsignedInteger32(pvApiCtx, var_addr, &dims[0], &dims[1], &tmp_uint32);
	    }
	  else
	    {
	      _SciErr = getMatrixOfUnsignedInteger32InList(pvApiCtx, parent, item_position, &dims[0], &dims[1], &tmp_uint32);
	    }
	  createdVar = Mat_VarCreate(name, MAT_C_UINT32, MAT_T_UINT32, rank, dims, tmp_uint32, 0);
	  break;
        default:
          createdVar = NULL;
          break;
        }
    }
  else
    {
      Scierror(999, _("%s: Wrong type for first input argument: Integer matrix expected.\n"), "GetIntegerVariable");
    }

  FREE(dims);

  return createdVar;
}
